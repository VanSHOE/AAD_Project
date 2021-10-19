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

	double last = 0;
	int I = 0;
	int j = 0;
	int cur = 1;
	int mid = -1;
	int mini = 0;

	FVector SpawnPosition;
	FActorSpawnParameters SpawnParamenters;


	AGrid();
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
		TSubclassOf<AGrid_Cell> CellBP;
	std::vector<std::vector<AGrid_Cell*>> grid3d;
	std::vector<std::vector<int>> grid;
	UPROPERTY(EditAnywhere)
		double delay;

	//UPROPERTY(EditAnywhere)
		uint64 space_left = 500;
	UPROPERTY(EditAnywhere)
		uint64 size_x = 100;
	UPROPERTY(EditAnywhere)
		uint64 size_y = 100;
	UPROPERTY(VisibleAnywhere)
		uint64 total_value = 0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Frac_K(bool& retflag);

	void bsearch(bool& retflag);

	void SeqSearch(bool& retflag);

	void Sel(bool& retflag);

	//UPROPERTY(VisibleAnywhere)
	int p = 0;
	//UPROPERTY(VisibleAnywhere)
	int q = 0;
	
	//UPROPERTY(EditAnywhere)
		bool pq = false;

	void BubbleSort(bool& retflag);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool next = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AUTO = false;
	void text_color(int c, int r, int col);
	UPROPERTY(VisibleAnywhere)
		int cur_step = 0;
	//UPROPERTY(EditAnywhere)
		int ToSearch = -1;
};
