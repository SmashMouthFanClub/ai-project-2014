// Fill out your copyright notice in the Description page of Project Settings.

#include "Cars.h"
#include "BaseCar.h"


ABaseCar::ABaseCar(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	CarMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("CarMesh"));
	CarMesh->SetSimulatePhysics(true);
	RootComponent = CarMesh;
}


