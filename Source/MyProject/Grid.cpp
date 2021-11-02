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
	//std::vector<std::vector<uint32>> grid(size_y, std::vector<uint32>(size_x, 0));
	size_y = start.Len() + 1;
	size_x = end.Len() + 1;
	grid3d.assign(size_y, std::vector<AGrid_Cell*>(size_x, nullptr));
	grid.assign(size_y, std::vector<int64>(size_x, INT_MAX));
	stringState.assign(size_y, std::vector<std::deque<FString>>(size_x));
	SpawnParamenters.Owner = this;
	SpawnPosition = GetActorLocation();

}

// Called every frames

void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("%d %d"), p, q);
	/*
	last += DeltaTime;
	if (last < delay)
	{
		return;
	}
	last = 0;
	*/
	// bool retflag;
	// BubbleSort(retflag);
	// Sel(retflag);
	// SeqSearch(retflag);
	// bsearch(retflag);
	// Frac_K(retflag);
	// if (retflag) return;
	if (cur_step == -1)
	{
		if(next)
			cur_step++;
	}
	else if (cur_step == 0)
	{
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
			next = true;
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

		grid3d[I][j]->Text->SetText(FText::FromString(FString::FromInt(grid[I][j])));
		j++;
		SpawnPosition.X += 400;
		p = 0;
		q = size_x - 1;
	}
	else if (cur_step == 1)
	{
		if (next || AUTO)
		{
			for (int i = 0; i < size_x; i++)
			{
				grid3d[0][i]->Text->SetText(FText::FromString(FString::FromInt(grid[0][i])));
			}
			next = false;
		}
	}
}


void AGrid::up(int ii, int jj, int val)
{
	grid[ii][jj] = val;
	grid3d[ii][jj]->Text->SetText(FText::FromString(FString::FromInt(grid[ii][jj])));
}

void AGrid::Frac_K(bool& retflag)
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
			cur_step++;
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

		grid3d[I][j]->Text->SetText(FText::FromString(TEXT(" ")));
		j++;
		SpawnPosition.X += 400;
		p = 0;
		q = size_x - 1;
	}
	else if (cur_step == 1)
	{
		if (j >= size_x)
		{
			j = 0;
			cur_step++;
			return;
		}
		if (j == 0)
			grid[2][j] = rand() % (4) + 1;
		else
			grid[2][j] = rand() % (4) + grid[2][j - 1];
		grid3d[2][j]->Text->SetText(FText::FromString(FString::FromInt(grid[2][j])));
		j++;
	}
	else if (cur_step == 2)
	{
		if (j >= size_x)
		{
			cur_step++;
			j = size_x - 1;
			return;
		}
		grid[1][j] = rand() % (size_x * size_y) + 1;
		grid[0][j] = grid[1][j] * grid[2][j];
		grid3d[0][j]->Text->SetText(FText::FromString(FString::FromInt(grid[0][j])));
		grid3d[1][j]->Text->SetText(FText::FromString(FString::FromInt(grid[1][j])));
		j++;
	}
	else if (cur_step == 3)
	{
		if (!next && !AUTO)
		{
			return;
		}
		next = false;
		if (j < 0)
		{
			cur_step = -2;
			return;
		}
		text_color(0, j, 1);
		text_color(1, j, 1);
		text_color(2, j, 1);
		cur_step++;
	}
	else if (cur_step == 4)
	{
		if (!next && !AUTO)
		{
			return;
		}
		if (space_left >= grid[1][j])
		{
			space_left -= grid[1][j];
			total_value += grid[0][j];
			text_color(0, j, 2);
			text_color(1, j, 2);
			text_color(2, j, 2);
			cur_step--;
			j--;
		}
		else
		{
			total_value += (1.f * grid[0][j] * space_left / (grid[1][j]));
			cur_step = -2;
			text_color(0, j, 3);
			text_color(1, j, 3);
			text_color(2, j, 3);
		}
	}
	retflag = false;
}

void AGrid::bsearch(bool& retflag)
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

		if (j == 0)
		{
			grid[I][j] = -1e7;
			grid3d[I][j]->Text->SetText(FText::FromString(TEXT("-INF")));
		}
		else if (j == 1)
		{
			grid[I][j] = rand() % (size_x * size_y) + 1;
			grid3d[I][j]->Text->SetText(FText::FromString(FString::FromInt(grid[I][j])));
		}
		else if (j == size_x - 1)
		{
			grid[I][j] = 1e7;
			grid3d[I][j]->Text->SetText(FText::FromString(TEXT("INF")));
		}
		else
		{
			grid[I][j] = rand() % (size_x * size_y) + grid[I][j - 1];
			grid3d[I][j]->Text->SetText(FText::FromString(FString::FromInt(grid[I][j])));
		}
		text_color(0, j, 2);
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
		p = 0;
		q = size_x - 1;
	}
	else if (cur_step == 1)
	{
		if (!next && !AUTO)
		{
			return;
		}
		next = false;
		if (q - p == 1) // a[q] >= what we want. a[p] < what we want 
		{
			cur_step = -2;
			return;
		}

		mid = (p + q) / 2;
		text_color(0, mid, 3);
		cur_step = 2;
		return;
	}
	else if (cur_step == 2)
	{
		if (!next && !AUTO) return;
		next = false;
		UE_LOG(LogTemp, Warning, TEXT("%d at %d and we want %d"), grid[0][mid], mid, ToSearch);
		if (grid[0][mid] == ToSearch)
		{
			cur_step = -2;
			for (int l = 0; l < size_x; l++)
			{
				text_color(0, l, 0);
			}
			text_color(0, mid, 2);
			return;
		}
		else if (grid[0][mid] < ToSearch)
		{
			p = mid;
			for (int l = 0; l <= p; l++)
			{
				text_color(0, l, 0);
			}
		}
		else
		{
			q = mid;
			for (int l = size_x - 1; l >= q; l--)
			{
				text_color(0, l, 0);
			}
		}
		cur_step = 1;
	}
	retflag = false;
}

void AGrid::SeqSearch(bool& retflag)
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
		q = 0;
	}
	else if (cur_step == 1)
	{
		if (q >= size_x)
		{
			cur_step = -2;
			return;
		}
		if (!next && !AUTO)
		{
			return;
		}
		next = false;
		text_color(0, q, 1);
		cur_step = 2;
		return;
	}
	else if (cur_step == 2)
	{
		if (!next && !AUTO) return;
		next = false;

		if (grid[0][q] == ToSearch)
		{
			cur_step = -2;
			text_color(0, q, 2);
			return;
		}
		else
		{
			text_color(0, q, 0);
		}
		q++;
		cur_step = 1;
	}
	retflag = false;
}

void AGrid::Sel(bool& retflag)
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
				text_color(0, 0, 3);
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
		q = 1;
	}
	else if (cur_step == 1)
	{
		if (!next && !AUTO)
		{
			return;
		}
		next = false;
		if (q >= size_x)
		{
			UE_LOG(LogTemp, Warning, TEXT("A pass is done"))
			int tp = grid[0][mini];
			grid[0][mini] = grid[0][p];
			grid[0][p] = tp;
			grid3d[0][p]->Text->SetText(FText::FromString(FString::FromInt(grid[0][p])));
			text_color(0, mini, 0);
			text_color(0, p, 2);
			grid3d[0][mini]->Text->SetText(FText::FromString(FString::FromInt(grid[0][mini])));
			p++;
			mini = p;
			text_color(0, mini, 3);
			q = p + 1;
		}
		if (p >= size_x - 1)
		{
			cur_step = -2;
			return;
		}
		text_color(0, q, 1);
		cur_step = 2;
		return;
	}
	else if (cur_step == 2)
	{
		if (!next && !AUTO) return;
		next = false;
		if (grid[0][q] < grid[0][mini])
		{
			text_color(0, mini, 0);
			UE_LOG(LogTemp, Warning, TEXT("%d was mini, now its %d"), mini, q);
			mini = q;
			text_color(0, mini, 3);
		}
		else text_color(0, q, 0);
		q++;
		cur_step = 1;
	}
	retflag = false;
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


// 0: White, 1: Purple, 2: Green, 3: Blue
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