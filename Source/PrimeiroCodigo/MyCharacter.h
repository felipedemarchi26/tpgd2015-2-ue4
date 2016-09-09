// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class PRIMEIROCODIGO_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void SetLife(int NewLife);
	int GetLife();

	FORCEINLINE int GetNewLife() const { return Life; }
	FORCEINLINE void AMyCharacter::SetNewLife(int NewLife) { Life = NewLife; }

	void OnDeath();

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere)
		UCameraComponent* PlayerCamera;
	UPROPERTY(EditAnywhere)
		USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere)
		UArrowComponent* ArrowComp;
	UPROPERTY(EditAnywhere)
		int Life = 100;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void StartRun();
	void StopRun();
	void DropProjectile();
	void Turn(float Value);

	
};
