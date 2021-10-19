// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GraphEdge.h"
#include <vector>
#include "GraphNode.generated.h"

UCLASS()
class MYPROJECT_API AGraphNode : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGraphNode();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GraphNode")
		class USceneComponent* RootScene;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GraphNode")
		class USceneComponent* TextScene;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GraphNode")
		class UStaticMeshComponent* Sphere;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GridCell")
		class UText3DComponent* Text;

	struct msort
	{
		uint64 l;
		uint64 r;
	} val;


	APawn* player = nullptr;
	AGraphNode* parent = nullptr;
	AGraphNode* left = nullptr;
	AGraphNode* right = nullptr;
	struct edge_to
	{
		AGraphEdge* edge;
		AGraphNode* nbor;
		int i;
		int j;
		int k;
	};
	int my_i;
	int my_j;
	int my_k;

	FVector SpawnPosition;
	bool visited = false;

	UPROPERTY(VisibleAnywhere)
	int ct = 0;
	//UPROPERTY(VisibleAnywhere)
	std::vector<edge_to> edges;
	UMaterialInterface* mt;
	class AGrid* marray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
