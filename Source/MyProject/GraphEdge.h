// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EWeight.h"
#include "EdgeHead.h"
#include "GraphEdge.generated.h"

UCLASS()
class MYPROJECT_API AGraphEdge : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GraphEdge")
		class USceneComponent* RootScene;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GraphEdge")
		class UStaticMeshComponent* Cylinder;
	
	AEdgeHead* head = nullptr;
	AEWeight* Text = nullptr;
	AEWeight* dp = nullptr;
	bool g = false;
	AGraphEdge();
	UMaterialInterface* mt;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
