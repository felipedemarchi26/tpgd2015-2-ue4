// Fill out your copyright notice in the Description page of Project Settings.

#include "PrimeiroCodigo.h"
#include "Potion.h"

APotion::APotion() {
	ConstructorHelpers::FObjectFinder<UStaticMesh>
		Mesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Trim_90_In.Shape_Trim_90_In'"));
	if (Mesh.Succeeded()) {
		GetMeshComp()->SetStaticMesh(Mesh.Object);
	}
	GetMeshComp()->SetWorldScale3D(FVector(0.4f, 0.4f, 0.4f));
}

int APotion::Use() {
	return FMath::RandRange(1,10);
}


