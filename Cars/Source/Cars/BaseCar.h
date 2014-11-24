// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseCar.generated.h"

/**
 * 
 */
UCLASS()
class CARS_API ABaseCar : public AActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Car)
	TSubobjectPtr<UStaticMeshComponent> CarMesh;
};
