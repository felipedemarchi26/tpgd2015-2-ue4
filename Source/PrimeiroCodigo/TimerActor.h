// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TimerActor.generated.h"

UCLASS()
class PRIMEIROCODIGO_API ATimerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimerActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

private:

	UShapeComponent* Root;
	UStaticMeshComponent* MeshComp;
	FTimerHandle CountdownTimerHandle;
	float CountdownTime;

	void TimerManager();
};
