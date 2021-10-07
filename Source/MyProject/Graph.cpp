// Fill out your copyright notice in the Description page of Project Settings.

#include "GraphNode.h"
#include "DrawDebugHelpers.h"
#include <stdlib.h>
#include "Graph.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGraph::AGraph()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	srand(time(0));
}

// Called when the game starts or when spawned
int cur_step = 0;
int ox, oy, oz;
void AGraph::BeginPlay()
{
	Super::BeginPlay();
	i = 0; j = 0; k = 0;
	cur_step = 0;
	SpawnParamenters.Owner = this;
	SpawnPosition = GetActorLocation();
	ox = SpawnPosition.X;
	oy = SpawnPosition.Y;
	oz = SpawnPosition.Z;
	grid3d.assign(size_z, std::vector<std::vector<AGraphNode*>>(size_y, std::vector<AGraphNode*>(size_x)));

	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = GetActorLocation();
	FVector EndTrace = { 100000, 100000, 100000 };
	EndTrace = StartTrace + EndTrace;
	FVector ForwardVector = UKismetMathLibrary::GetDirectionUnitVector(StartTrace, EndTrace);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
	{
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *HitResult->Actor->GetName()));
	}


}

// Called every frame

void AGraph::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (cur_step == 0)
	{
		if (k >= size_z)
		{
			k = 0;
			j++;
			SpawnPosition.Z = oz;
			SpawnPosition.Y += DistanceBetweenNodes;
		}
		if (j >= size_y)
		{
			j = 0;
			i++;
			SpawnPosition.Y = oy;
			SpawnPosition.X += DistanceBetweenNodes;
		}
		if (i >= size_x)
		{
			i = size_x;
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Spawning"));
		if (rand() % ProbabilityIn1byX == 0)
			grid3d[i][j][k] = GetWorld()->SpawnActor<AGraphNode>(Node, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
		else grid3d[i][j][k] = nullptr;
		
		SpawnPosition.Z += DistanceBetweenNodes;
		k++;
	}


}

