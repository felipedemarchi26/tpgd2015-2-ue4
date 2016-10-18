// Fill out your copyright notice in the Description page of Project Settings.

#include "PrimeiroCodigo.h"
#include "MyCharacter.h"
#include "ProjectileActor.h"
#include "Item.h"
#include "Alavanca.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->bGenerateOverlapEvents = true;

	//MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	//MeshComp->SetCollisionProfileName("NoCollision");
	//MeshComp->AttachTo(GetCapsuleComponent());

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SkeletalMesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (SkeletalMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
	}
	GetMesh()->SetWorldLocation(FVector(0.0f, 0.0f, -80.0f));
	GetMesh()->SetWorldScale3D(FVector(0.9f, 0.9f, 0.9f));
	GetMesh()->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	ConstructorHelpers::FObjectFinder<UAnimBlueprint>
		AnimObj(TEXT("AnimBlueprint'/Game/Blueprints/WalkBP.WalkBP'"));
	if (AnimObj.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(AnimObj.Object->GetAnimBlueprintGeneratedClass());
	}

	ConstructorHelpers::FObjectFinder<UAnimSequence>
		AnimJumpLoad(TEXT("AnimSequence'/Game/AnimStarterPack/Jump_From_Stand.Jump_From_Stand'"));
	if (AnimJumpLoad.Succeeded()) {
		JumpAnim = AnimJumpLoad.Object;
	}

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	PlayerCamera->AttachTo(CameraBoom);

	//ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	//ArrowComp->SetHiddenInGame(false);
	//ArrowComp->ArrowSize = 2.0f;
	//ArrowComp->AttachTo(GetMesh());

	CollectCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollectCollision"));
	CollectCollisionComp->InitSphereRadius(200.0f);
	CollectCollisionComp->AttachTo(RootComponent);

	GetCharacterMovement()->MaxWalkSpeed = 400;

	ConstructorHelpers::FClassFinder<UUserWidget>
		Widget(TEXT("WidgetBlueprint'/Game/Blueprints/Pause.Pause_C'"));
	if (Widget.Succeeded()) {
		UserWidget = Widget.Class;
	}

	//AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Exemplo!"));
	
}

// Called every frame
void AMyCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (GetMesh()->GetAnimationMode() == EAnimationMode::AnimationSingleNode
		&& GetCharacterMovement()->IsMovingOnGround()) {
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	
	InputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
	InputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("Run", IE_Pressed, this, &AMyCharacter::StartRun);
	InputComponent->BindAction("Run", IE_Released, this, &AMyCharacter::StopRun);

	InputComponent->BindAction("Drop", IE_Pressed, this, &AMyCharacter::DropProjectile);
	InputComponent->BindAction("Collect", IE_Pressed, this, &AMyCharacter::OnCollect);

	InputComponent->BindAction("Pause", IE_Pressed, this, &AMyCharacter::Pause);
}

void AMyCharacter::MoveForward(float Value) {
	//FVector Forward(0, 1, 0);
	//AddMovementInput(Forward, Value);

	if (Controller != nullptr && Value != 0) {
		FRotator Rotation = Controller->GetControlRotation();
		if (GetCharacterMovement()->IsMovingOnGround()) {
			Rotation.Pitch = 0.0f;
		}
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyCharacter::MoveRight(float Value) {
	//FVector Right(1, 0, 0);
	//AddMovementInput(Right, Value);

	if (Controller != nullptr && Value != 0.0f) {
		FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMyCharacter::StartRun() {
	GetCharacterMovement()->MaxWalkSpeed = 800;
}

void AMyCharacter::StopRun() {
	GetCharacterMovement()->MaxWalkSpeed = 400;
}

void AMyCharacter::SetLife(int NewLife) {
	Life = NewLife;
}

int AMyCharacter::GetLife() {
	return Life;
}

void AMyCharacter::OnDeath() {
	if (Life <= 0) {
		FVector InitialLocation(-60.0f, 30.0f, 350.0f);
		Life = 100;
		SetActorLocation(InitialLocation);
	}
}

void AMyCharacter::DropProjectile() {
	FActorSpawnParameters SpawnParameters;
	UWorld* World = GetWorld();
	if (World != nullptr) {
		FRotator Rotation = GetMesh()->GetComponentRotation();
		AProjectileActor* Proj = World->SpawnActor<AProjectileActor>
			(GetActorLocation(), Rotation, 
				SpawnParameters);
		if (Proj != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Spawn OK!"));
		}
	}
}

void AMyCharacter::Turn(float Value) {
	//AddControllerYawInput(Value);
	FRotator NewRotation = GetMesh()->GetComponentRotation();
	NewRotation.Yaw += Value;
	GetMesh()->SetWorldRotation(NewRotation);
}

void AMyCharacter::OnCollect() {
	TArray<AActor*> AtoresColetados;
	CollectCollisionComp->GetOverlappingActors(AtoresColetados);

	for (int i = 0; i < AtoresColetados.Num(); i++) {
		if (AtoresColetados[i]->IsA(AItem::StaticClass())) {
			AItem* ItemColetado = Cast<AItem>(AtoresColetados[i]);
			Inventory.Add(ItemColetado);
			ItemColetado->Destroy();
			UE_LOG(LogTemp, Warning, TEXT("%d"), Inventory.Num());
		} else if (AtoresColetados[i]->IsA(AAlavanca::StaticClass())) {
			AAlavanca* Alavanca = Cast<AAlavanca>(AtoresColetados[i]);
			Alavanca->OnPressed();
		}
	}
}

void AMyCharacter::Jump() {
	Super::Jump();

	if (JumpAnim != nullptr) {
		GetMesh()->PlayAnimation(JumpAnim, false);
	}
}

void AMyCharacter::Pause() {
	UWorld* World = GetWorld();
	if (World != nullptr) {
		APlayerController* PlayerController =
			UGameplayStatics::GetPlayerController(World, 0);
		if (PlayerController != nullptr && UserWidget != NULL) {
			PlayerController->SetPause(true);
			UUserWidget* UserW =
				UWidgetBlueprintLibrary::Create
				(World, UserWidget, PlayerController);
			if (UserW != nullptr) {
				UserW->AddToViewport();
				PlayerController->bShowMouseCursor = true;
			}

		}
	}
}