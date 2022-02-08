// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/SpectralCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "NPC/Occ_AIController.h"
#include "PentaKill_Character.h"
#include "NPC/NPCAnimInstance.h"
#include "BrainComponent.h"
#include "Interfaces/IActivables.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/IPlayerComm.h"

// Sets default values
ASpectralCharacter::ASpectralCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 2.0f;

	bUseControllerRotationYaw =false;
	GetCharacterMovement()->bOrientRotationToMovement =true;
	GetCharacterMovement()->bEnablePhysicsInteraction=false;
	//GetCharacterMovement()->BrakingDecelerationWalking=20.f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->DefaultLandMovementMode=MOVE_Walking;
}

// Called when the game starts or when spawned
void ASpectralCharacter::BeginPlay()
{
	Super::BeginPlay();
	MyController = Cast<AOcc_AIController>(GetController());
	CurrentHealth = MaxHealth;
}

void ASpectralCharacter::PostInitProperties()
{
	Super::PostInitProperties();
}

void ASpectralCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(GetMesh()->GetAnimInstance()!=nullptr)
	{
		PawnAnimInstRef = Cast<UNPCAnimInstance>(GetMesh()->GetAnimInstance());
	}
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	if(GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemies"));
	}
}

void ASpectralCharacter::AttackPlayerAction()
{
	const auto PlayerPawn = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	if(PlayerPawn)
	{
		const FRotator RotLookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),PlayerPawn->GetActorLocation());
		SetActorRotation(RotLookAt);
		const auto I = Cast<IIPlayerComm>(PlayerPawn);
		I->I_IsBeenAttacked(GetActorLocation());
	}
}

// SET cinematics
void ASpectralCharacter::I_SetNewAnimation(const FVector MoveLocation,
			const FRotator TargetRotation,UAnimMontage* NewAnimMontage,const float MoveSpeed)
{
	if(!bIsAlerted)
	{
		// parar la logica del BT
		AnimMontage = NewAnimMontage;
		bIsInCinematic =true;
		MoveToCinematic(MoveLocation,TargetRotation);
	}
}

void ASpectralCharacter::MoveToCinematic(FVector TargetLoc,FRotator TargetRot)
{
	if(PawnAnimInstRef)
	{
		PawnAnimInstRef->SetIsMoving(true);
	}
	// latentInfo --> correr tal funcion cuando llegue a destino. 
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "StartCinematic";
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = 11;
	UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), FVector(TargetLoc.X, TargetLoc.Y, GetActorLocation().Z),
										TargetRot, false, false, 2.0f,
										true, EMoveComponentAction::Move, LatentInfo);
}

void ASpectralCharacter::StartCinematic() const
{
	if(AnimMontage)
	{
		TurnLogic(false);
		if(PawnAnimInstRef)
		{
			PawnAnimInstRef->SetIsMoving(false);
			PawnAnimInstRef->PlayAnimationMontage(AnimMontage);
		}
	}
}
/**
void ASpectralCharacter::PlayCinematic_Implementation(UAnimMontage* NewAnimMontage)
{
	if(AnimMontage)
	{
		TurnLogic(false);
		if(PawnAnimInstRef)
		{
			 PawnAnimInstRef->PlayAnimationMontage(AnimMontage);
		}
	}
}
*/
void ASpectralCharacter::EndPlayCinematic(const bool bInterrupted)
{
	// Hago este delay para esperar a que la animacion haya terminado y no solapar.
	
	if(AnimMontage)
	{
		FTimerHandle EndMontageTH;
		FTimerDelegate TimerDel;
		//Binding the function with specific values
		TimerDel.BindUFunction(this,FName(TEXT("TurnLogic")), true);
		GetWorldTimerManager().SetTimer(EndMontageTH,TimerDel,4.0f,false);
	}
	Execute_I_SetMovementSpeed(this,bInterrupted);
	AnimMontage =nullptr;
}

void ASpectralCharacter::TurnLogic(const bool bNewOn) const
{
	if(IsValid(MyController) && IsValid(MyController->GetBrainComponent()))
	{
		if(bNewOn)
		{
			MyController->GetBrainComponent()->RestartLogic();
		}
		else
		{
			MyController->GetBrainComponent()->StopLogic("cinematic");
		}
	}
}

void ASpectralCharacter::ResetLogic() const
{
	if(MyController)
	{
		if(MyController->GetBrainComponent()->IsResourceLocked())
		{
			MyController->GetBrainComponent()->RequestLogicRestartOnUnlock();
		}
		else
		{
			MyController->GetBrainComponent()->RestartLogic();
		}
	}
}

// Called every frame
void ASpectralCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// check if is on animation state.
	if(PawnAnimInstRef && !PawnAnimInstRef->IsAnyMontagePlaying())
	{
		IterationRay();
	}	
}

void ASpectralCharacter::I_SetMovementSpeed_Implementation(const bool bAlertedMovementSpeed)
{
	if(bAlertedMovementSpeed)
	{
		GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
		bIsAlerted =true;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		bIsAlerted =false;
	}
}
// Set attack
void ASpectralCharacter::I_SetAttack_Implementation()
{
	TurnLogic(false);
	Execute_I_CallWalkingAudio(this,false);
	
	//FTimerHandle ThAttack;
	//GetWorldTimerManager().SetTimer(ThAttack,this,&ASpectralCharacter::AttackPlayerAction,0.0f,false,1.0f);
	const auto PlayerPawn = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	if(PlayerPawn)
	{
		const FRotator RotLookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),PlayerPawn->GetActorLocation());
		SetActorRotation(RotLookAt);
		const auto I = Cast<IIPlayerComm>(PlayerPawn);
		I->I_IsBeenAttacked(GetActorLocation());
	}
	
	if(AttackAnimation)
	{
		const float AnimTime = PawnAnimInstRef->PlayAnimationMontage(AttackAnimation);
		FTimerHandle ResetLogicTH;
		GetWorldTimerManager().SetTimer(ResetLogicTH,this,&ASpectralCharacter::ResetLogic,AnimTime,false);
	}
}

void ASpectralCharacter::I_InterruptCinematic_Implementation()
{
	if(AnimMontage && PawnAnimInstRef)
	{
		PawnAnimInstRef->Montage_Stop(1.0f,AnimMontage);
	}
}

bool ASpectralCharacter::I_GetIsDead_Implementation() const
{
	return bIsDead;
}

void ASpectralCharacter::I_FoundTarget_Implementation()
{
	IIEnemies::I_FoundTarget_Implementation();
}

void ASpectralCharacter::IterationRay() const
{
	const FVector START_LOCATION = GetActorLocation();
	const FVector END_LOCATION = START_LOCATION + (GetActorForwardVector() * InteractiveRayLength);
	FHitResult Hit;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if (GetWorld()->SweepSingleByChannel(Hit, START_LOCATION, END_LOCATION, FQuat::Identity, Puntero,
		FCollisionShape::MakeCapsule(20.f,
			GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), QueryParams))
	if (Hit.Actor->Implements<UIActivables>())
	{
		auto I = Cast<IIActivables>(Hit.Actor);
		float const DIRECTION_MOD = UKismetMathLibrary::Dot_VectorVector(GetActorForwardVector(), Hit.Actor->GetActorForwardVector());
		if(DIRECTION_MOD ==0){return;}
		float Feed = 1 * DIRECTION_MOD;
		I->I_Activate(true,Feed);
	}
}
