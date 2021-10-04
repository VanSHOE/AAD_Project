// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Text3DComponent.h"
// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
double last = 0;
int p = 0;
int q = 0;
int i = 0;
int j = 0;
int cur = 1;
FVector SpawnPosition;
FActorSpawnParameters SpawnParamenters;
bool build = true;
// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	p = 0;
	q = 0;
	i = 0;
	j = 0;
	cur = 1;
	last = 0;
	build = true;
	//std::vector<std::vector<uint32>> grid(size_y, std::vector<uint32>(size_x, 0));
	grid3d.assign(size_y, std::vector<AGrid_Cell*>(size_x));
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
	if (build)
	{
		if (j >= size_x)
		{
			j = 0;
			i++;
			SpawnPosition.Y += 400;
			SpawnPosition.X = 0;
		}
		if (i >= size_y)
		{
			build = false;
			return;
		}
		grid3d[i][j] = GetWorld()->SpawnActor<AGrid_Cell>(CellBP, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
		if (j == size_x - 1)
		{
			grid3d[i][j]->Wall_pX->ToggleVisibility();
		}
		if (i == size_y - 1)
		{
			grid3d[i][j]->Wall_pY->ToggleVisibility();
		}
		j++;
		SpawnPosition.X += 400;
		UE_LOG(LogTemp, Warning, TEXT("%d/%d %d/%d"), i, size_y,j, size_x);
	}
	if (build) return;

	if (q >= size_x)
	{
		p++;
		q = 0;
	}
	if (p >= size_y)
	{
		q = p = 0;
		return;
	}

	FString send = send.FromInt(cur++);
	FText fin = fin.FromString(send);
	auto mat = grid3d[p][q]->Text->FrontMaterial->GetMaterial();
	auto dyn = UMaterialInstanceDynamic::Create(mat, NULL);
	dyn->SetScalarParameterValue(TEXT("Blend"), 1);
	grid3d[p][q]->Text->SetFrontMaterial(dyn);
	grid3d[p][q++]->Text->SetText(fin);

}

