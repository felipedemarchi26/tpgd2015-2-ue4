// Fill out your copyright notice in the Description page of Project Settings.

#include "PrimeiroCodigo.h"
#include "BTTask_MoveToPlayer.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MyCharacter.h"

EBTNodeResult::Type UBTTask_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AMyAIController *PC = Cast<AMyAIController>
		(OwnerComp.GetAIOwner());

	AMyCharacter *Player = Cast<AMyCharacter>
		(OwnerComp.GetBlackboardComponent()->
			GetValue<UBlackboardKeyType_Object>
			(PC->EnemyKeyID));

	if (Player != nullptr) {
		PC->MoveToActor(Player, 20.0f, true, true, true,
			0, true);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;

}



