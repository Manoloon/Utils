.h

UENUM(BlueprintType)
enum class EUtilityType : uint8
{
Action_Null,
Action_Cover,
Action_Heal,
Action_Reload,
Action_Attack,
Action_Patrol
};

USTRUCT(BlueprintType)
struct FUtilityPair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUtilityType Type = EUtilityType::Action_Null;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UtilityValue;

	__forceinline static FUtilityPair GetHighestUtility(const TArray<FUtilityPair>& Utilities)
	{
		
		{
			FUtilityPair OutUtility;
			if (Utilities.Num() == 0)
			{
				return OutUtility;
			}
			else if (Utilities.Num() == 1)
			{
				return Utilities[0];
			}
			OutUtility = Utilities[0];
			for (int32 i = 1; i < Utilities.Num(); i++)
			{
				const FUtilityPair& Utility = Utilities[i];
				if (Utility.UtilityValue > OutUtility.UtilityValue)
				{
					OutUtility = Utilities[i];
				}
			}
			return OutUtility;
		}
	};
};

UCLASS()
AiController : public AAIController
{
	GENERATED_BODY()
	
public:
	AiController();

protected:

	float DeltaTimer;

	const float BASE_ATTACK_SCORE = 0.4;
	const float FLEE_SCORE_EXP_COEFFICIENT = 0.25;
	const float FLEE_SCORE_TEAMMATES_EFFECT_EXP = 3.0;
	const float HEAL_SCORE_LOGISTIC_RANGE = 6.0;
	const float HEAL_SCORE_STEEPNESS = 1.848432; // e euler number * 0.68.
	const float RELOAD_SCORE_LOGISTIC_RANGE = 4.0;
	const float RELOAD_SCORE_STEEPNESS = 1.848432;

	// Utility AI
	UPROPERTY(BlueprintReadWrite)
	TArray<FUtilityPair>UtilitiesGroup;

private:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float ThreatUtility(float MaxDamage,float OwnerHealth);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float HealUtility(float ThreatScore, float OwnerHealth, float OwnerMaxHealth);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float AttackUtility(AActor* EnemyTarget,float EnemyHP, float OwnerWeaponMinDamage);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float ReloadUtility(float ThreatScore, float OwnerCurrentAmmo, float OwnerMaxAmmoInMag);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float CoverUtility(float HealScore, float threatScore, float ReloadScore);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float PatrolUtility(AActor* EnemyTarget);
		

	UFUNCTION(BlueprintCallable)
		FUtilityPair CalculateUtilities();

	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;	


---------------------------------------------------------------
cpp


AiController::AiController()
{
	UtilitiesGroup = {};
}


void AiController::BeginPlay()
{
	Super::BeginPlay();
}


void AiController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DeltaTimer = DeltaTime;
}

float AiController::ThreatUtility(float MaxDamage,float OwnerHealth)
{
	float ThreatWeight = FMath::Min((MaxDamage / OwnerHealth), 1.0f);

	return ThreatWeight;
}

float AiController::HealUtility(float ThreatScore, float OwnerHealth, float OwnerMaxHealth)
{
	float LocalHealth = OwnerHealth;
	float LocalMaxHealth = OwnerMaxHealth;
	float exponent = -((LocalHealth / LocalMaxHealth) * (HEAL_SCORE_LOGISTIC_RANGE * 2)) + HEAL_SCORE_LOGISTIC_RANGE;
	float expBase = 1 + FMath::Pow(HEAL_SCORE_STEEPNESS, exponent);
	float score = 1 - (1 / expBase);
	return score * ThreatScore;
}

float AiController::AttackUtility(AActor* EnemyTarget,float EnemyHP,float OwnerWeaponMinDamage)
{
	if(EnemyTarget)
	{
		float LocalMinDamage = OwnerWeaponMinDamage;
		float LocalMaxDamage = 100;

		float InverseRatio = 1 - (EnemyHP - LocalMinDamage) / (LocalMaxDamage - LocalMinDamage);
		float Score = (InverseRatio * (1 - BASE_ATTACK_SCORE)) + BASE_ATTACK_SCORE;
		float AttackScore = FMath::Max<float>(FMath::Min(Score, 1.0f), BASE_ATTACK_SCORE);
		return AttackScore;
	}
	else 
	{
		return 0.0f;
	}	
}

float AiController::ReloadUtility(float ThreatScore, float OwnerCurrentAmmo, float OwnerMaxAmmoInMag)
{
	float LocalAmmo = OwnerCurrentAmmo;
	float LocalMaxAmmo = OwnerMaxAmmoInMag;
	float exponent = -((LocalAmmo / LocalMaxAmmo) * (RELOAD_SCORE_LOGISTIC_RANGE * 2)) + RELOAD_SCORE_LOGISTIC_RANGE;
	float expBase = 1 + FMath::Pow(RELOAD_SCORE_STEEPNESS, exponent);
	float BaseScore = 1 - (1 / expBase);
	float ReloadScore = BaseScore * ThreatScore;
	return ReloadScore;
}

float AiController::CoverUtility(float HealScore, float threatScore, float ReloadScore)
{
	float CoverCalc = ((0.2 + ReloadScore) + (HealScore * 1.5)) * (threatScore * 1.3);
	float CoverScore = UKismetMathLibrary::FClamp(CoverCalc, 0.0, 1.0);
	return CoverScore;
}

float AiController::PatrolUtility(AActor* EnemyTarget)
{
	if(!EnemyTarget)
	{
		return 1.0f;
	}
	else 
	{
	return 0.01f;
	}
}

FUtilityPair AiController::CalculateUtilities()
{	
	return FUtilityPair::GetHighestUtility(UtilitiesGroup);
}