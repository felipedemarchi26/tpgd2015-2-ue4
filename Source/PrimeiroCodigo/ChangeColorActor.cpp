// Fill out your copyright notice in the Description page of Project Settings.

#include "PrimeiroCodigo.h"
#include "ChangeColorActor.h"
#include "MyCharacter.h"


// Sets default values
AChangeColorActor::AChangeColorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	Root->bGenerateOverlapEvents = true;
	Root->SetCollisionProfileName("OverlapAllDynamic");
	Root->OnComponentBeginOverlap.AddDynamic(this, &AChangeColorActor::OnOverlapBegin);
	Root->OnComponentEndOverlap.AddDynamic(this, &AChangeColorActor::OnOverlapEnd);
	Root->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	RootComponent = Root;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName("NoCollision");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (Mesh.Succeeded()) {
		MeshComp->SetStaticMesh(Mesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Materials/LifeMaterial.LifeMaterial'"));
	if (Material.Succeeded()) {
		MeshComp->SetMaterial(0, Material.Object);
	}
	MeshComp->SetWorldLocation(FVector(0.0f, 0.0f, -30.0f));
	MeshComp->SetWorldScale3D(FVector(0.6f, 0.6f, 0.6f));
	MeshComp->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AChangeColorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChangeColorActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AChangeColorActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) &&
		(OtherComp != nullptr)) {

		UE_LOG(LogTemp, Warning, TEXT("OverlapBegin"));
		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/Game/Materials/DamageMaterial.DamageMaterial'")));
		if (NewMaterial != nullptr) {
			MeshComp->SetMaterial(0, NewMaterial);
		}

	}
}

void AChangeColorActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if ((OtherActor != nullptr) && (OtherActor != this) &&
		(OtherComp != nullptr)) {

		UE_LOG(LogTemp, Warning, TEXT("OverlapEnd"));
		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/Game/Materials/LifeMaterial.LifeMaterial'")));
		if (NewMaterial != nullptr) {
			MeshComp->SetMaterial(0, NewMaterial);
		}

		UWorld* World = GetWorld();
		if (World != nullptr) {
			UGameplayStatics::OpenLevel(World, "Level1");
		}


	}
}