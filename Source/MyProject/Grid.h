// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include "Grid_Cell.h"
#include "Grid.generated.h"

UCLASS()
class MYPROJECT_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
		TSubclassOf<AGrid_Cell> CellBP;
	std::vector<std::vector<AGrid_Cell*>> grid3d;
	std::vector<std::vector<int>> grid;
	UPROPERTY(EditAnywhere)
		double delay;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
		uint64 size_x = 100;
	UPROPERTY(EditAnywhere)
		uint64 size_y = 100;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void pause();
	void BubbleSort(bool& retflag);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool next = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AUTO = false;
	void text_color(int c, int r, int col);
	UPROPERTY(VisibleAnywhere)
	int cur_step = 0;
};
