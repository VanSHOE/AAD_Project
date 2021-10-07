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
	UPROPERTY(EditAnywhere)
		int size_x;
	UPROPERTY(EditAnywhere)
		int size_y;
	UPROPERTY(EditAnywhere)
		int size_z;
	UPROPERTY(EditAnywhere)
		int nodes;
	UPROPERTY(EditAnywhere)
		int edges;	
	UPROPERTY(EditAnywhere)
		int DistanceBetweenNodes;
	UPROPERTY(EditAnywhere)
		int ProbabilityIn1byX;
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
		TSubclassOf<AGraphNode> Node;
	int i = 0;
	int j = 0;
	int k = 0;
	std::vector<std::vector<std::vector<AGraphNode*>>> grid3d;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector SpawnPosition;
	FActorSpawnParameters SpawnParamenters;
};
