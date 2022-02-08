// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/NPCAnimInstance.h"
#include "NPC/SpectralCharacter.h"


void UNPCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UNPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}


void UNPCAnimInstance::FunctionToExecuteOnAnimationBlendOut(UAnimMontage* AnimMontage, const bool bInterrupted)
{
	if (bInterrupted)
	{
		const auto MyActor = Cast<ASpectralCharacter>(GetOwningActor());
		if(MyActor)
		{
			MyActor->EndPlayCinematic(true);
		}
	}
	else
	{
		const auto MyActor = Cast<ASpectralCharacter>(GetOwningActor());
		if(MyActor)
		{
			MyActor->EndPlayCinematic(false);
		}
	}
}

void UNPCAnimInstance::FunctionToExecuteOnAnimationEnd(UAnimMontage* AnimMontage, const bool bInterrupted)
{
	const auto MyActor = Cast<ASpectralCharacter>(GetOwningActor());
	if(MyActor)
	{
		MyActor->EndPlayCinematic(bInterrupted);
	}
}

void UNPCAnimInstance::SetIsMoving(bool bNewMove)
{
	bIsMoving = bNewMove;
}

float UNPCAnimInstance::PlayAnimationMontage(UAnimMontage* NewAnimMontage)
{
	if(NewAnimMontage)
	{
		Montage_Play(NewAnimMontage);

		FOnMontageEnded BlendOutDelegate;
		BlendOutDelegate.BindUObject(this, &UNPCAnimInstance::FunctionToExecuteOnAnimationBlendOut);
		Montage_SetBlendingOutDelegate(BlendOutDelegate, NewAnimMontage);

		FOnMontageEnded CompleteDelegate;
		CompleteDelegate.BindUObject(this, &UNPCAnimInstance::FunctionToExecuteOnAnimationEnd);
		Montage_SetEndDelegate(CompleteDelegate, NewAnimMontage);
		return NewAnimMontage->GetPlayLength();
	}
	return 0.0f;
}
