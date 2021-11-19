// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EWeight.generated.h"

UCLASS()
class MYPROJECT_API AEWeight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEWeight();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EW")
		class USceneComponent* RootScene;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EW")
		class USceneComponent* TextScene;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EW")
		class UText3DComponent* Text;
	int64 used = 0;
	int64 cap;
	void Set();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
