// Fill out your copyright notice in the Description page of Project Settings.


#include "PressableActor.h"

void APressableActor::OnInteract(AMyCharacter* player)
{
	NotifyOnPressed();
}
