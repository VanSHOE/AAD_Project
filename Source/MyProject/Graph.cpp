// Fill out your copyright notice in the Description page of Project Settings.

#include "GraphNode.h"

#include "DrawDebugHelpers.h"
#include "Text3DComponent.h"
#include "Kismet/GameplayStatics.h"
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



void AGraph::BeginPlay()
{
	Super::BeginPlay();
	fib_n = mn;
	groot = nullptr;
	i = j = k = 0;
	cur_step = 0;
	cnodes = 0;
	last = 0;
	SpawnParamenters.Owner = this;
	SpawnPosition = GetActorLocation();
	ox = SpawnPosition.X;
	oy = SpawnPosition.Y;
	oz = SpawnPosition.Z;
	cur_bfs = 0;
	Store.clear();
	if (nodes >= size_z * size_y * size_x)
	{
		nodes = size_z * size_y * size_x;
	}
	g_index = nodes - 1;
	skip = false;
	c_val = 0;
	me.assign(fib_n + 1, false);
	mem = false;
	grid3d.assign(size_x, std::vector<std::vector<AGraphNode*>>(size_y, std::vector<AGraphNode*>(size_z, nullptr)));
	//TArray<AActor*> a;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrid::StaticClass(), a);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *a[0]->GetName());
	//mat = Cast<AGrid>(a[0]);
	second.clear();

}

// Called every frame


void AGraph::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (last < delay)
	{
		last += DeltaTime;
		return;
	}
	last = 0;
	
	if (cur_step == 0)
	{
		if (cnodes >= nodes)
		{
			i = k = 0;
			j = 1;
			cur_step++;
			return;
		}
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
			i = j = k = 0;
			SpawnPosition.X = ox;
			SpawnPosition.Y = oy;
			SpawnPosition.Z = oz;
			return;
		}

		if (grid3d[i][j][k] == nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Spawning"));
			//UE_LOG(LogTemp, Warning, TEXT("%d %d %d"), i, j, k);
			if (rand() % ProbabilityIn1byX == 0)
			{
				grid3d[i][j][k] = GetWorld()->SpawnActor<AGraphNode>(Node, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
				grid3d[i][j][k]->val = c_val;
				grid3d[i][j][k]->Text->SetText(FText::FromString(FString::FromInt(c_val++)));
				grid3d[i][j][k]->my_i = i;
				grid3d[i][j][k]->my_j = j;
				grid3d[i][j][k]->my_k = k;
				cnodes++;
				Store.push_back(grid3d[i][j][k]);
			}
		}
		SpawnPosition.Z += DistanceBetweenNodes;
		k++;
	}
	else if (cur_step == 1)
	{

		if (j >= nodes)
		{
			i++;
			j = i + 1;
		}
		if (i >= nodes - 1)
		{
			i = j = 0;
			cur_step = 2;
			second.push_back(Store[0]);
			return;
		}
		if (i == j)
		{
			j++;
			return;
		}
		AGraphNode::edge_to q;
		//UE_LOG(LogTemp, Warning, TEXT("%d is store size, cnodes is %d we need %d and %d"), Store.size(), cnodes, i, j);
		FVector Myloc = Store[i]->GetActorLocation();
		Myloc.Z += 50;
		FVector Tarloc = Store[j]->GetActorLocation();
		Tarloc.Z += 50;
		FVector dir = Tarloc - Myloc;
		FRotator pointTo = dir.Rotation();
		Myloc += (50.f * dir / dir.Size());
		//UE_LOG(LogTemp, Warning, TEXT("Between %s and %s, vector is %s, rotator is %s"), *Store[i]->GetName(), *Store[j]->GetName(), *dir.ToString(), *pointTo.ToString());

		FHitResult* HitResult = new FHitResult();
		FVector StartTrace = Myloc;
		FVector EndTrace = Tarloc;
		FVector ForwardVector = UKismetMathLibrary::GetDirectionUnitVector(StartTrace, EndTrace);
		FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
		TraceParams->AddIgnoredActor(Store[i]);

		if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
		{

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *HitResult->Actor->GetName()));
			auto who = HitResult->GetActor();
			bool make = false;
			if (who->IsA(AGraphNode::StaticClass()) && rand() % Ep == 0)
			{
				AGraphNode* hit_node = Cast<AGraphNode>(who);
				if (hit_node->my_i == Store[j]->my_i && hit_node->my_j == Store[j]->my_j && hit_node->my_k == Store[j]->my_k)
				{
					q.edge = GetWorld()->SpawnActor<AGraphEdge>(Edge, Myloc, pointTo, SpawnParamenters);
					auto hlocl = Tarloc;
					hlocl -= (50.f + headsize) * ForwardVector;
					GetWorld()->SpawnActor<AEdgeHead>(Head, hlocl, pointTo, SpawnParamenters);
					FVector scale = { 1, 1, dir.Size() - (100 + headsize) };
					q.edge->SetActorScale3D(scale);
					FVector wtpos = Myloc + ForwardVector * (dir.Size() / 2);
					GetWorld()->SpawnActor<AEWeight>(WtText, wtpos, pointTo, SpawnParamenters);










					q.i = Store[j]->my_i;
					q.j = Store[j]->my_j;
					q.k = Store[j]->my_k;
					q.nbor = Store[j];
					Store[i]->edges.push_back(q);
					//q.i = Store[i]->my_i;
					//q.j = Store[i]->my_j;
					//q.k = Store[i]->my_k;
					//q.nbor = Store[i];
					//Store[j]->edges.push_back(q);
					make = true;
				}
			}
			if (!make)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Edge not made from %s to %s because %s was in the way!"), *Store[i]->GetName(), *Store[j]->GetName(), *who->GetName());
				//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true);
			}
		}

		j++;
	}
	else if (cur_step == 2)
	{
		if (!next && !AUTO)
		{
			return;
		}
		next = false;

		if (second.size() == 0)
		{
			cur_step = 4;
			return;
		}
		cur = second[0];
		cur->visited = true;
		UE_LOG(LogTemp, Warning, TEXT("Visited: %s"), *cur->GetName());
		node_color(cur, 0);
		second.pop_front();
		cur_bfs = 0;
		cur_step = 3;
		if (cur->edges.size() == 0) skip = true;
	}
	else if (cur_step == 3)
	{
		if (!next && !AUTO && !skip)
		{
			return;
		}
		next = false;
		skip = false;
		if (cur_bfs >= cur->edges.size())
		{
			cur_step--;
			cur_bfs = 0;
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("On  %s, child of %s"), *cur->edges[cur_bfs].nbor->GetName(), *cur->GetName());
		if (cur->edges[cur_bfs].nbor->visited == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Adding %s, child of %s"), *cur->edges[cur_bfs].nbor->GetName(), *cur->GetName());
			node_color(cur->edges[cur_bfs].nbor, 1);
			cur->edges[cur_bfs].nbor->visited = true;
			second.push_back(cur->edges[cur_bfs].nbor);
		}
		else skip = true;
		cur_bfs++;
	}
	else if (cur_step == 4)
	{
		if (!next && !AUTO)
		{
			return;
		}
		next = false;
		UE_LOG(LogTemp, Warning, TEXT("We are now searching"));
		for (int ii = 0; ii < Store.size(); ii++)
		{
			if (Store[ii]->visited == false)
			{
				//cur = Store[ii];
				second.push_back(Store[ii]);
				node_color(cur, 0);
				cur_step = 2;
				return;
			}
		}
		cur_step = 99;
	
	}
	





	
	
	
	

	
	
}


void AGraph::r_graph(bool& retflag)
{
	retflag = true;


	retflag = false;
}

uint64 AGraph::min(uint64 a, uint64 b)
{
	if (a < b) return a;
	return b;
}

void AGraph::node_color(AGraphNode* n, bool green)
{
	UStaticMeshComponent* comp = Cast<UStaticMeshComponent>(n->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	auto mt = n->mt;
	auto dyn = UMaterialInstanceDynamic::Create(mt, NULL);
	if (green == 0)
	{
		dyn->SetScalarParameterValue(TEXT("Opacity"), 0.75f);
		dyn->SetScalarParameterValue(TEXT("col"), 1);
	}
	else dyn->SetScalarParameterValue(TEXT("Opacity"), 0.75f);
	comp->SetMaterial(0, dyn);
}

FString AGraph::c2s(int l, int r)
{
	return "[" + FString::FromInt(l) + ", " + FString::FromInt(r) + ")";
}