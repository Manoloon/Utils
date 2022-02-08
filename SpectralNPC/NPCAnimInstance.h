// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NPCAnimInstance.generated.h"

/**
 * 
 */

UCLASS(Abstract,Blueprintable)
class PENTAKILL_CHARACTER_API UNPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	void FunctionToExecuteOnAnimationBlendOut(UAnimMontage* AnimMontage, bool bInterrupted);
	void FunctionToExecuteOnAnimationEnd(UAnimMontage* AnimMontage, bool bInterrupted);
	void SetIsMoving(bool bNewMove);
	/**
	 * return Animation Length
	 */
	float PlayAnimationMontage(UAnimMontage* NewAnimMontage);
protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	UPROPERTY(BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
		bool bIsMoving=false;
};
