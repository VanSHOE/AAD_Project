// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Text3DComponent.h"
#include "GameFramework/Actor.h"
#include "Word_3D.generated.h"

UCLASS()
class MYPROJECT_API AWord_3D : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWord_3D();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GraphNode")
		class USceneComponent* RootScene;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GridCell")
		class UText3DComponent* Text;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
