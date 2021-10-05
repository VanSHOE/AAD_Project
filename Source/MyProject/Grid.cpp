// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Text3DComponent.h"
#include <stdlib.h>
#if WITH_EDITOR
#include "UnrealEd.h"
#endif
// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	srand(time(0));
}
double last = 0;
int p = 0;
int q = 0;
int I = 0;
int j = 0;
int cur = 1;
FVector SpawnPosition;
FActorSpawnParameters SpawnParamenters;

// Called when the game starts or when spawned

void AGrid::BeginPlay()
{
	Super::BeginPlay();
	p = 0;
	q = 0;
	I = 0;
	j = 0;
	cur = 1;
	last = 0;
	next = false;
	cur_step = -1;
	//std::vector<std::vector<uint32>> grid(size_y, std::vector<uint32>(size_x, 0));
	grid3d.assign(size_y, std::vector<AGrid_Cell*>(size_x));
	grid.assign(size_y, std::vector<int>(size_x));
	SpawnParamenters.Owner = this;
	SpawnPosition = GetActorLocation();
	SpawnPosition.X = 0;
	//for (int i = 0; i < size_y; i++)
	//{
	//	for (int j = 0; j < size_x; j++)
	//	{
	//		grid3d[i][j] = GetWorld()->SpawnActor<AGrid_Cell>(CellBP, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
	//		SpawnPosition.X += 400;
//		}
	//	SpawnPosition.Y += 400;
//	}

}

// Called every frame
int required = 0;
int sort_step = 0;
int mini = 0;
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("%d %d"), p, q);




	last += DeltaTime;
	if (last < delay)
	{
		return;
	}
	last = 0;
	//bool retflag;
	//BubbleSort(retflag);
	//if (retflag) return;

	if (cur_step == -1)
	{
		if (next || AUTO)
		{
			cur_step++;
			next = false;
		}
	}
	else if (cur_step == 0)
	{
		if (j >= size_x)
		{
			j = 0;
			I++;
			SpawnPosition.Y += 400;
			SpawnPosition.X = 0;
		}
		if (I >= size_y)
		{
			if (next || AUTO)
			{
				cur_step++;
				next = false;
			}
			return;
		}
		grid3d[I][j] = GetWorld()->SpawnActor<AGrid_Cell>(CellBP, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
		grid[I][j] = rand() % (size_x * size_y) + 1;
		grid3d[I][j]->Text->SetText(FText::FromString(FString::FromInt(grid[I][j])));
		if (j == size_x - 1)
		{
			grid3d[I][j]->Wall_pX->ToggleVisibility();
		}
		if (I == size_y - 1)
		{
			grid3d[I][j]->Wall_pY->ToggleVisibility();
		}
		j++;
		SpawnPosition.X += 400;

	}
	else if (cur_step == 1)
	{
		if (!AUTO) pause();
		if (q >= size_x)
		{
			int tp = grid[0][mini];
			grid[0][mini] = grid[0][p];
			grid[0][p] = tp;
			grid3d[0][p]->Text->SetText(FText::FromString(FString::FromInt(grid[0][p])));
			text_color(0, p, 0);
			text_color(0, mini, 0);
			grid3d[0][mini]->Text->SetText(FText::FromString(FString::FromInt(grid[0][mini])));
			p++;
			mini = p;
			text_color(0, mini, 2);
			q = p + 1;
		}
		if (p >= size_x - 1)
		{
			cur_step = -2;
			return;
		}
		text_color(0, q, 1);
		UE_LOG(LogTemp, Warning, TEXT("Red? %d"), q);
		if (!AUTO) pause();
		if (grid[0][q] < grid[0][mini])
		{
			text_color(0, mini, 0);
			mini = q;
			text_color(0, mini, 2);
		}
		else text_color(0, q, 0);
		UE_LOG(LogTemp, Warning, TEXT("NOO"));
		q++;
		if (!AUTO) pause();
	}



}

void AGrid::pause()
{
	GetWorld()->bDebugPauseExecution = true;

}

void AGrid::BubbleSort(bool& retflag)
{
	retflag = true;
	if (cur_step == -1)
	{
		if (next || AUTO)
		{
			cur_step++;
			next = false;
		}
	}
	else if (cur_step == 0)
	{
		if (j >= size_x)
		{
			j = 0;
			I++;
			SpawnPosition.Y += 400;
			SpawnPosition.X = 0;
		}
		if (I >= size_y)
		{
			if (next || AUTO)
			{
				cur_step++;
				next = false;
			}
			return;
		}
		grid3d[I][j] = GetWorld()->SpawnActor<AGrid_Cell>(CellBP, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
		grid[I][j] = rand() % (size_x * size_y) + 1;
		grid3d[I][j]->Text->SetText(FText::FromString(FString::FromInt(grid[I][j])));
		if (j == size_x - 1)
		{
			grid3d[I][j]->Wall_pX->ToggleVisibility();
		}
		if (I == size_y - 1)
		{
			grid3d[I][j]->Wall_pY->ToggleVisibility();
		}
		j++;
		SpawnPosition.X += 400;

	}
	else if (cur_step == 1)
	{
		if (!next && !AUTO) return;
		next = false;
		if (q >= size_x - p - 1)
		{
			q = 0;
			p++;
		}
		if (p >= size_x - 1)
		{
			cur_step = -2;
			return;
		}
		text_color(0, q, 1);
		text_color(0, q + 1, 1);
		cur_step = 2; return;
		//UE_LOG(LogTemp, Warning, TEXT("%d/%d %d/%d"), p, size_y, q, size_x);

	}
	else if (cur_step == 2)
	{
		if (!next && !AUTO) return;
		next = false;

		if (grid[0][q] > grid[0][q + 1])
		{
			int temp = grid[0][q];
			grid[0][q] = grid[0][q + 1];
			grid[0][q + 1] = temp;
			grid3d[0][q]->Text->SetText(FText::FromString(FString::FromInt(grid[0][q])));
			grid3d[0][q + 1]->Text->SetText(FText::FromString(FString::FromInt(grid[0][q + 1])));
		}
		cur_step = 1;
		text_color(0, q, 0);
		if (q + 1 == size_x - 1 - p)
		{
			text_color(0, q + 1, 3);
		}
		else text_color(0, q + 1, 0);
		q++;
	}
	retflag = false;
}

void AGrid::text_color(int c, int r, int col)
{
	auto mat = grid3d[c][r]->Text->FrontMaterial->GetMaterial();
	auto dyn = UMaterialInstanceDynamic::Create(mat, NULL);
	if (col == 0 || col == 1)
	{
		dyn->SetScalarParameterValue(TEXT("Blend1"), col);
		dyn->SetScalarParameterValue(TEXT("Blend2"), 0);
		UE_LOG(LogTemp, Warning, TEXT("Should be %d"), col);
	}
	else
	{
		dyn->SetScalarParameterValue(TEXT("Blend1"), 0);
		dyn->SetScalarParameterValue(TEXT("Blend2"), 1);
		UE_LOG(LogTemp, Warning, TEXT("Should be green"));
	}	
	grid3d[c][r]->Text->SetFrontMaterial(dyn);
}

