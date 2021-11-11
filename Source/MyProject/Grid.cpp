// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Text3DComponent.h"
#include "Kismet/GameplayStatics.h"
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

// Called when the game starts or when spawned



void AGrid::BeginPlay()
{
	Super::BeginPlay();
	p = 0;
	q = 0;
	I = 0;
	j = 0;
	cur = 1;
	mid = -1;
	last = 0;
	next = false;
	cur_step = -1;
	mini = 0;
	ToSearch = -1;
	setSize();
	SpawnParameters.Owner = this;
	SpawnPosition = GetActorLocation();
}

// Called every frames
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (cur_step == -1)
	{
		if(next || AUTO)
			cur_step++;
	}
	else if (cur_step == 0)
	{
		/*
		if (j >= size_x)
		{
			j = 0;
			I++;
			SpawnPosition.Y += 400;
			SpawnPosition.X = GetActorLocation().X;
		}
		if (I >= size_y)
		{
			cur_step++;
			for (int ii = 0; ii < size_y; ii++)
			{
				grid[ii][0] = 0;
				grid3d[ii][0]->Text->SetText(FText::FromString(TEXT("0")));
			}
			//next = true;
			I = j = 0;
			return;
		}
		grid3d[I][j] = GetWorld()->SpawnActor<AGrid_Cell>(CellBP, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
		if (j == size_x - 1)
		{
			grid3d[I][j]->Wall_pX->ToggleVisibility();
		}
		if (I == size_y - 1)
		{
			grid3d[I][j]->Wall_pY->ToggleVisibility();
		}
		grid3d[I][j]->Text->SetText(FText::FromString(TEXT("Inf")));
		j++;
		SpawnPosition.X += 400;
		*/
		p = 0;
		q = size_x - 1;
		for (int jj = 0; jj < size_y; jj++)
		{
			for (int ii = 0; ii < size_x; ii++)
			{
				grid3d[jj][ii] = GetWorld()->SpawnActor<AGrid_Cell>(CellBP, SpawnPosition, FRotator::ZeroRotator, SpawnParameters);

				if (ii == size_x - 1)
					grid3d[jj][ii]->Wall_pX->ToggleVisibility();
				if(jj == size_y - 1)
					grid3d[jj][ii]->Wall_pY->ToggleVisibility();
				SpawnPosition.X += 400;
			}
			SpawnPosition.X = GetActorLocation().X;
			SpawnPosition.Y += 400;
		}
		SpawnPosition = GetActorLocation();
		cur_step++;
		next = true;
	}
	else if (cur_step == 1)
	{
		if (next || AUTO)
		{
			for (int ii = 0; ii < size_y; ii++)
			{
				for (int jj = 0; jj < size_x; jj++)
					up(ii, jj, grid[ii][jj]);
			}
			next = false;
		}
	}
}


void AGrid::up(int ii, int jj, int val)
{
	grid[ii][jj] = val;
	if(val != INT_MAX)
		grid3d[ii][jj]->Text->SetText(FText::FromString(FString::FromInt(grid[ii][jj])));
	else
		grid3d[ii][jj]->Text->SetText(FText::FromString("Inf"));
}

/// 0: White, 1: Purple, 2: Green, 3: Blue
void AGrid::text_color(int c, int r, int col)
{
	auto mat = grid3d[c][r]->Text->FrontMaterial->GetMaterial();
	auto dyn = UMaterialInstanceDynamic::Create(mat, NULL);
	//UE_LOG(LogTemp, Warning, TEXT("%d should be %d"), r, col);
	if (col == 0 || col == 1)
	{
		
		dyn->SetScalarParameterValue(TEXT("Blend1"), col);
		dyn->SetScalarParameterValue(TEXT("Blend2"), 0);
		dyn->SetScalarParameterValue(TEXT("Blend3"), 0);
		//UE_LOG(LogTemp, Warning, TEXT("Should be %d"), col);
	}
	else if(col == 2)
	{
		dyn->SetScalarParameterValue(TEXT("Blend1"), 0);
		dyn->SetScalarParameterValue(TEXT("Blend2"), 1);
		dyn->SetScalarParameterValue(TEXT("Blend3"), 0);
		//UE_LOG(LogTemp, Warning, TEXT("Should be green"));
	}	
	else
	{
		dyn->SetScalarParameterValue(TEXT("Blend1"), 0);
		dyn->SetScalarParameterValue(TEXT("Blend2"), 0);
		dyn->SetScalarParameterValue(TEXT("Blend3"), 1);
	}
	grid3d[c][r]->Text->SetFrontMaterial(dyn);
}

int64 AGrid::min(int64 a, int64 b)
{
	if (a < b)
		return a;
	return b;
}

bool AGrid::diff(TCHAR a, TCHAR b)
{
	if (a != b)
		return true;
	return false;
}

void AGrid::setSize()
{
	grid3d.assign(size_y, std::vector<AGrid_Cell*>(size_x, nullptr));
	grid.assign(size_y, std::vector<int64>(size_x, INT_MAX));
}