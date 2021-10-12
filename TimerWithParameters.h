FTimerDelegate TimerDel;
//Binding the function with specific values
TimerDel.BindUFunction(this,FName(TEXT("ChangeCurrentFOV")), 40.0f);
GetWorldTimerManager().SetTimer(ThFovChanged,TimerDel,0.01f,true,1.0f);