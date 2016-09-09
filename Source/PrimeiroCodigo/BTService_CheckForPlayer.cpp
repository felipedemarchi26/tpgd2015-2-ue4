// Fill out your copyright notice in the Description page of Project Settings.

#include "PrimeiroCodigo.h"
#include "BTService_CheckForPlayer.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MyCharacter.h"

void UBTService_CheckForPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {

	AMyAIController *PC = Cast<AMyAIController>
		(OwnerComp.GetAIOwner());

	if (PC != nullptr) {

		AMyCharacter *Player = Cast<AMyCharacter>
			(GetWorld()->
			GetFirstPlayerController()->
			GetPawn());

		if (Player != nullptr) {
			OwnerComp.GetBlackboardComponent()->
			SetValue<UBlackboardKeyType_Object>
			(PC->EnemyKeyID, Player);

			GEngine->AddOnScreenDebugMessage(
				-1, 2.0f, FColor::Green,
				"Player na área!");
		}


	}

}


