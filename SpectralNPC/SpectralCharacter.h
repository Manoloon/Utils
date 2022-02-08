// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Occ_AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Interfaces/IEnemies.h"
#include "DAP_BaseItem.h"
#include "SpectralCharacter.generated.h"
/**
UENUM()
enum class EStateName
{
	Attack,
	Search,
	Suffer
};*/
UCLASS(Abstract)
class PENTAKILL_CHARACTER_API ASpectralCharacter : public ACharacter, public IIEnemies
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ASpectralCharacter();

protected:
	UPROPERTY(EditAnywhere,Category="Settings/Animations")
		UAnimMontage* AttackAnimation;
	UPROPERTY(EditAnywhere,Category="Settings/Animations")
		UAnimMontage* InterruptAnimation;
	UPROPERTY(EditAnywhere,Category="Settings")
		float WalkSpeed =60.f;
	UPROPERTY(EditAnywhere,Category="Settings")
		float ChasingSpeed = 500.f;
	UPROPERTY(BlueprintReadWrite)
		int32 CurrentHealth;
	UPROPERTY(BlueprintReadWrite)
		bool bIsDead=false;
	UPROPERTY(BlueprintReadOnly)
		bool bIsInCinematic =false;
	UPROPERTY(BlueprintReadWrite)
		bool bIsAlerted=false;
	UPROPERTY(BlueprintReadOnly)
		class AOcc_AIController* MyController=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Settings")
		TSoftObjectPtr<UBehaviorTree> BTToUse =nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Settings/Audio")
		TArray<TSoftObjectPtr<USoundBase>>TalkingAudio;
	//Add subtitles its a struct of DAP_BaseItem
    UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Settings/Audio")
	TArray<FE_Subitulos>TalkingSubtitles;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Settings/Audio")
	TArray<FE_Subitulos>DiscoveredSubtitles;
	
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Settings",meta = (ExposeOnSpawn=true,AllowPrivateAccess = "true"))
		int32 MaxHealth =3;

	FVector TargetToMove;
	
	UPROPERTY(EditDefaultsOnly,Category = "Settings",meta = (AllowPrivateAccess = "true"))
		float InteractiveRayLength=100;
	UPROPERTY()
		class UNPCAnimInstance* PawnAnimInstRef =nullptr;
	UPROPERTY()
		UAnimMontage* AnimMontage=nullptr;
protected:
	UFUNCTION(BlueprintCallable)
	void IterationRay() const;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;
	virtual void PostInitializeComponents() override;

	void AttackPlayerAction();
//cinematics
	UFUNCTION()
	void MoveToCinematic(FVector TargetLoc,FRotator TargetRot);
	//UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	//void PlayCinematic(UAnimMontage* NewAnimMontage);
	UFUNCTION()
	void StartCinematic() const;
	UFUNCTION(BlueprintCallable)
	void TurnLogic(const bool bNewOn) const;
	UFUNCTION(BlueprintCallable)
	void ResetLogic()const;
public:
	void EndPlayCinematic(const bool bInterrupted);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// INTERFACE
	UFUNCTION()
	virtual void I_SetNewAnimation(const FVector MoveLocation,
			const FRotator TargetRotation,UAnimMontage* NewAnimMontage,const float MoveSpeed) override;
	virtual void I_SetMovementSpeed_Implementation(const bool bAlertedMovementSpeed) override;
	virtual void I_SetAttack_Implementation() override;
	virtual void I_InterruptCinematic_Implementation() override;
	UFUNCTION(BlueprintCallable)
		virtual bool I_GetIsDead_Implementation() const override;
	UFUNCTION(BlueprintCallable)
		virtual void I_FoundTarget_Implementation() override;
};




