///////////////////////////////////////////
PlayerCharacter.h

// deben asignarle una curvefloat de lo contrario no funcionara.
UPROPERTY(EditAnywhere)
	ucurvefloat* Curvefloat; 
	
Ftimeline DodgeTimeLine

UFUNCTION()
void Dodge();

UFUNCTION()
void AllowDodge();
UFUNCTION()
void StartDodging();

//////////////////////////////////////////
PlayerCharacter.cpp


void APlayerCharacter::BeginPlay()
{
if(Curvefloat)
	{
	// definir timeline callback ( mientras recorre la curva)
		FOnTimelineFloat DodgeTimelineCallback;
	// Callback que llama a un evento cuando finaliza el timeline.
		FOnTimelineEventStatic DodgeTimelineFinishCallback;
	// bind de una funcion con el callback (la funcion que se ira actualizando mientras dure la curva)
		DodgeTimelineCallback.BindUFunction(this, FName("Dodge")); 
	// bind de una funcion al finalizar la timeline
		DodgeTimelineFinishCallback.BindUFunction(this, FName{ TEXT("AllowDodge") });
	// definimos que tipo de lerp usaremos en la callback
		DodgeTimeLine.AddInterpFloat(Curvefloat, DodgeTimelineCallback);
	// definimos que funcion usaremos al finalizar la timeline (aqui podriamos reutilizar la timeline con otras funciones)
		DodgeTimeLine.SetTimelineFinishedFunc(DodgeTimelineFinishCallback);
	}
}	
	
void APlayerCharacter::tick(float DeltaTime)
{
if(bIsDodging)
	{
	// importante definir el tick para la timeline , de lo contrario nunca avanzara.
		DodgeTimeLine.TickTimeline(DeltaTime);
	}	
}	

//funcion donde se define la timeline y se hace el lerp.
void APlayerCharacter::Dodge()
{
	float DodgeTimelineValue = Timeline_Dodging.GetPlaybackPosition();
	float DodgeCurveFloatValue = Curvefloat->GetFloatValue(DodgeTimelineValue);
	FVector ForwardDodgeDir = ((GetActorForwardVector()*MoveForwardValue)*DodgeDistance);
	FVector SideDodgeDir = ((GetActorRightVector()*MoveRightValue)*DodgeDistance);
	FVector FinalDodgeLocation = FMath::Lerp(GetActorLocation(), (ForwardDodgeDir + SideDodgeDir + GetActorLocation()), DodgeCurveFloatValue);
	SetActorLocation(FinalDodgeLocation, true);
}

// Funcion que se llamara al finalizar la timeline.
void APlayerCharacter::AllowDodge()
{
	bIsDodging = false;
}

// esta funcion activa la timeline desde el principio y es llamada por el input (bind a la accion : Dodge)
void APlayerCharacter::StartDodging()
{
	if(DodgingCurve && !bIsDodging)
	{
		bIsDodging = true;
		DodgeTimeLine.PlayFromStart();
	}
}