// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include "GraphNode.generated.h"

UCLASS()
class MYPROJECT_API AGraphNode : public AActor
{   
	GENERATED_BODY()
    
public:
	// Sets default values for this actor's properties
	AGraphNode();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GridCell")
		class USceneComponent* RootScene;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GridCell")
		class UStaticMeshComponent* Sphere;
	struct edge_to
	{
		int x;
		int y;
		int z;
	};

	std::vector<edge_to> edges;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};