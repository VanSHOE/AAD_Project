// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "GraphNode.h"
#include "GameFramework/Actor.h"
#include "Graph.generated.h"



UCLASS()
class MYPROJECT_API AGraph : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGraph();
	uint64 min(uint64 a, uint64 b);

	UPROPERTY(EditAnywhere)
		uint64 size_x;
	UPROPERTY(EditAnywhere)
		uint64 size_y;
	UPROPERTY(EditAnywhere)
		uint64 size_z;
	UPROPERTY(EditAnywhere)
		uint64 nodes;
	UPROPERTY(EditAnywhere)
		uint64 edges;
	UPROPERTY(EditAnywhere)
		int DistanceBetweenNodes;
	UPROPERTY(EditAnywhere)
		int ProbabilityIn1byX;
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
		TSubclassOf<AGraphNode> Node;
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
		TSubclassOf<AGraphEdge> Edge;
	uint64 i = 0;
	uint64 j = 0;
	uint64 k = 0;
	std::vector<std::vector<std::vector<AGraphNode*>>> grid3d;
	std::vector<AGraphNode*> Store;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector SpawnPosition;
	FActorSpawnParameters SpawnParamenters;
};
