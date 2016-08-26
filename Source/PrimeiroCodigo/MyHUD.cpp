// Fill out your copyright notice in the Description page of Project Settings.

#include "PrimeiroCodigo.h"
#include "MyHUD.h"
#include "Engine/Font.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"

AMyHUD::AMyHUD() {
	static ConstructorHelpers::FObjectFinder<UFont>
		Font(TEXT("Font'/Engine/EngineFonts/RobotoDistanceField.RobotoDistanceField'"));
	if (Font.Succeeded()) {
		HUDFont = Font.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D>
		Texture(TEXT("Texture2D'/Game/Textures/Green_Texture.Green_Texture'"));
	if (Texture.Succeeded()) {
		MyTexture = Texture.Object;
	}
}

void AMyHUD::DrawHUD() {
	Super::DrawHUD();

	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX,
		Canvas->SizeY);
		
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(
		UGameplayStatics::GetPlayerPawn(this, 0));

	FString LifeString = FString::Printf(TEXT("Life: %d"),
		MyCharacter->GetLife());
	DrawText(LifeString, FColor::Red, 50, 50, HUDFont);

	DrawTextureSimple(MyTexture, 200, 50, 1.0f, false);

	DrawTexture(MyTexture, 200, 200, MyCharacter->GetLife()*4,
		MyTexture->GetSizeY(), 0, 0, MyCharacter->GetLife()*4,
		MyTexture->GetSizeY(), FLinearColor::White,
		EBlendMode::BLEND_Translucent, 1.0f, false, 0.0f,
		FVector2D::ZeroVector);
}


