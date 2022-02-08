void APersonaje::SetCurrentEquipableItem(uint8 NewEquipCode)
{
	if(LoadOutItems[static_cast<ELoadOut>(NewEquipCode)])
	{
		if(NewEquipCode != CurrentEquippedCode)
		{
			// detach only if is attached //and is not the lantern.
			if(IsValid(CurrentEquipableItem) && CurrentEquippedCode !=2 && CurrentEquippedCode !=0)
			{
				CurrentEquipableItem->I_DetachFromOwner();
			}
			CurrentEquippedCode = NewEquipCode;
			AttachEquipable();
		}
		else
		{
			if(IsValid(CurrentEquipableItem))
			{
				CurrentEquipableItem->I_DetachFromOwner();
				I_StopPrimaryAction();
				CurrentEquipableItem = nullptr;
				CurrentEquippedCode = 4;			
			}		
		}
	}
}

bool APersonaje::IsItemAvailable(const uint8 ItemCode)
{
	return (CurrentLoadOut[ItemCode] >0);
}




void APersonaje::RemoveEquipableItem(const uint8 EquipCodeItem)
{
	if(IsItemAvailable(EquipCodeItem))
	{
		SetCurrentEquipableItem(EquipCodeItem);
		const uint8 NewValue = FMath::Clamp(CurrentLoadOut[EquipCodeItem] -1,0,250);
		CurrentLoadOut.Add(EquipCodeItem,NewValue);
		if(NewValue ==0)
		{
			CurrentEquipableItem->I_DetachFromOwner();
			CurrentEquipableItem = nullptr;
			CurrentEquippedCode = 4;
		}
	}
}

bool APersonaje::IsItemAvailable(const uint8 ItemCode)
{
	return (CurrentLoadOut[ItemCode] >0);
}

void APersonaje::AddEquipableItem(const uint8 NewEquipCode,bool bSetFromLevel)
{
	uint8 NewValue;
	// Pendulo o Linterna -just 1 item per loadOut
	if(NewEquipCode ==0 || NewEquipCode ==2)
	{
		NewValue = FMath::Clamp(CurrentLoadOut[NewEquipCode] +1,0,1);
		CurrentLoadOut.Add(NewEquipCode,NewValue);
		if(NewEquipCode ==2 && bSetFromLevel)
		{
			SetCurrentEquipableItem(2);
		}
		return;
	}
	// door seal and amulets -> no limits
	NewValue = CurrentLoadOut[NewEquipCode] +1;
	CurrentLoadOut.Add(NewEquipCode,NewValue);
}

void APersonaje::I_AddEquipableItem(const uint8 Item,bool bSetFromLevel)
{
	AddEquipableItem(Item,bSetFromLevel);
}

void APersonaje::I_RemoveEquipableItem(const uint8 Item)
{
	RemoveEquipableItem(Item);
}