// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Occ_AIController.generated.h"

/**
 * 
 */
UCLASS()
class PENTAKILL_CHARACTER_API AOcc_AIController : public AAIController
{
	GENERATED_BODY()

	virtual void OnPossess(APawn* InPawn) override;
	/**
	 * 5 = Friend
	 * 10 = Neutral
	 */
	UPROPERTY(EditAnywhere,Category = Settings)
	uint8 TeamId=5;
	UPROPERTY(EditAnywhere,Category = Settings)
	uint8 EnemyTeamId=10;
public:
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
