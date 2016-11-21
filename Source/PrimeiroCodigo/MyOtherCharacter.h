// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "MyOtherCharacter.generated.h"

UCLASS()
class PRIMEIROCODIGO_API AMyOtherCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyOtherCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void Jump() override;

	void SetLife(int NewLife);
	int GetLife();

	FORCEINLINE int GetNewLife() const { return Life; }
	FORCEINLINE void AMyOtherCharacter::SetNewLife(int NewLife) { Life = NewLife; }
	FORCEINLINE TArray<class AItem*> GetInventory() const { return Inventory; }
	FORCEINLINE int GetPontuacao() const { return Pontuacao; }
	FORCEINLINE void AMyOtherCharacter::SetPontuacao(int NewPontuacao) { Pontuacao = NewPontuacao; }

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
	UPROPERTY(EditAnywhere)
		UAnimSequence* JumpAnim;
	UPROPERTY(EditAnywhere)
		int Pontuacao;

	void ShowPontuacao();

	USphereComponent* CollectCollisionComp;
	TArray<class AItem*> Inventory;
	TSubclassOf<class UUserWidget> UserWidget;

	USoundCue* FireSound;
	UAudioComponent* AudioComp;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void StartRun();
	void StopRun();
	void DropProjectile();
	void Turn(float Value);
	void OnCollect();
	void Pause();

	UFUNCTION(Reliable, Server, WithValidation)
		void DropProjectileServer();
	void DropProjectileServer_Implementation();
	bool DropProjectileServer_Validate();
	
};
