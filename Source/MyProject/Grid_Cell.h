// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid_Cell.generated.h"

UCLASS()
class MYPROJECT_API AGrid_Cell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid_Cell();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GridCell")
		class USceneComponent* RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GridCell")
		class UStaticMeshComponent* Floor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GridCell")
		class UText3DComponent* Text;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GridCell")
		class UStaticMeshComponent* Wall_0; // Front Wall

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GridCell")
		class UStaticMeshComponent* Wall_1; // Right Wall

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GridCell")
		class UStaticMeshComponent* Wall_2; // Left Wall

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GridCell")
		class UStaticMeshComponent* Wall_3; // Back Wall
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void stext(FString s);
};
