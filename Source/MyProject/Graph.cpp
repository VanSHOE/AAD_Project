// Fill out your copyright notice in the Description page of Project Settings.

#include "GraphNode.h"
#include "DrawDebugHelpers.h"
#include "Text3DComponent.h"
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
int cnodes = 0;
AGraphNode* groot;
void AGraph::BeginPlay()
{
	Super::BeginPlay();
	groot = nullptr;
	size_x = 1;
	size_y = 3 * fib_n;
	size_z = 2 * fib_n;
	i = 0; j = size_y / 2; k = size_z - 1;
	cur_step = 0;
	cnodes = 0;
	SpawnParamenters.Owner = this;
	SpawnPosition = GetActorLocation();
	ox = SpawnPosition.X;
	oy = SpawnPosition.Y;
	oz = SpawnPosition.Z;
	Store.clear();
	if (nodes >= size_z * size_y * size_x)
	{
		nodes = size_z * size_y * size_x;
	}

	me.assign(fib_n + 1, false);

	grid3d.assign(size_x, std::vector<std::vector<AGraphNode*>>(size_y, std::vector<AGraphNode*>(size_z, nullptr)));
}

// Called every frame
AGraphNode* cur;
void AGraph::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//bool retflag;
	//r_graph(retflag);
	//if (retflag) return;

	if (cur_step == 0)
	{
		/*
		if (grid3d[i][j][k] == nullptr)
		{
			grid3d[i][j][k] = GetWorld()->SpawnActor<AGraphNode>(Node, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
			grid3d[i][j][k]->val = fib_n;

			grid3d[i][j][k]->my_i = i;
			grid3d[i][j][k]->my_j = j;
			grid3d[i][j][k]->my_k = k;

			grid3d[i][j][k]->SpawnPosition.X = SpawnPosition.X;
			grid3d[i][j][k]->SpawnPosition.Y = SpawnPosition.Y;
			grid3d[i][j][k]->SpawnPosition.Z = SpawnPosition.Z;

			grid3d[i][j][k]->Text->SetText(FText::FromString(FString::FromInt(grid3d[i][j][k]->val)));
			first.push_front(grid3d[i][j][k]);
		}
		cur_step++;
		cur = first[0];
		if (grid3d[i][j][k]->val <= 1)
		{
			cur_step = -1;
		}*/
		groot = GetWorld()->SpawnActor<AGraphNode>(Node, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
		groot->val = fib_n;
		groot->SpawnPosition = SpawnPosition;
		groot->Text->SetText(FText::FromString(FString::FromInt(groot->val)));
		groot->ct = 2;
		cur = groot;
		cur_step++;
	}
	else if (cur_step == 1)
	{
		
		if (!next && !AUTO)
		{
			return;
		}
		if (mem && me[cur->val])
		{
			cur->ct = 0;
		}
		
		next = false;
		/*
		//auto cur = first[0];
		//cur->ct--;
		if (cur->ct == 0)
			first.pop_front();
		SpawnPosition = cur->SpawnPosition;
		j = cur->my_j;
		k = cur->my_k;
		SpawnPosition.Z -= DistanceBetweenNodes;
		SpawnPosition.Y -= DistanceBetweenNodes;
		if (grid3d[i][j][k]->val - 1 < 0)
		{
			cur->ct = 0;
			cur_step++;
			return;
		}
		grid3d[i][j - 1][k - 1] = GetWorld()->SpawnActor<AGraphNode>(Node, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
		grid3d[i][j - 1][k - 1]->val = grid3d[i][j][k]->val - 1;
		grid3d[i][j - 1][k - 1]->Text->SetText(FText::FromString(FString::FromInt(grid3d[i][j - 1][k - 1]->val)));
		grid3d[i][j - 1][k - 1]->SpawnPosition = SpawnPosition;
		grid3d[i][j - 1][k - 1]->my_i = i;
		grid3d[i][j - 1][k - 1]->my_j = j - 1;
		grid3d[i][j - 1][k - 1]->my_k = k - 1;
		grid3d[i][j - 1][k - 1]->parent = grid3d[i][j][k];
		grid3d[i][j][k]->left = grid3d[i][j - 1][k - 1];


		if (grid3d[i][j][k]->val - 2 < 0)
		{
			cur->ct = 1;
			cur_step++;
			first.push_front(grid3d[i][j - 1][k - 1]);
			return;
		}
		SpawnPosition.Y += 2 * DistanceBetweenNodes;
		grid3d[i][j + 1][k - 1] = GetWorld()->SpawnActor<AGraphNode>(Node, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
		grid3d[i][j + 1][k - 1]->val = grid3d[i][j][k]->val - 2;
		grid3d[i][j + 1][k - 1]->Text->SetText(FText::FromString(FString::FromInt(grid3d[i][j + 1][k - 1]->val)));
		grid3d[i][j + 1][k - 1]->SpawnPosition = SpawnPosition;
		grid3d[i][j + 1][k - 1]->my_i = i;
		grid3d[i][j + 1][k - 1]->my_j = j + 1;
		grid3d[i][j + 1][k - 1]->my_k = k - 1;
		grid3d[i][j + 1][k - 1]->parent = grid3d[i][j][k];
		grid3d[i][j][k]->right = grid3d[i][j + 1][k - 1];


		first.push_front(grid3d[i][j + 1][k - 1]);
		first.push_front(grid3d[i][j - 1][k - 1]);
		cur_step++;
		*/
		if (cur->ct == 2)
		{
			SpawnPosition = cur->SpawnPosition;
			SpawnPosition.Z -= DistanceBetweenNodes;
			SpawnPosition.Y -= cur->val * cur->val * DistanceBetweenNodes;
			cur->left = GetWorld()->SpawnActor<AGraphNode>(Node, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
			cur->left->val = cur->val - 1;
			cur->left->SpawnPosition = SpawnPosition;
			cur->left->Text->SetText(FText::FromString(FString::FromInt(cur->left->val)));
			cur->left->parent = cur;
			if (cur->left->val <= 1)
			{
				cur->left->ct = 0;
			}
			else cur->left->ct = 2;
		

			AGraphNode::edge_to q;
			FVector Myloc = cur->GetActorLocation();
			Myloc.Z += 50;
			FVector Tarloc = cur->left->GetActorLocation();
			Tarloc.Z += 50;
			FVector dir = Tarloc - Myloc;
			FRotator pointTo = dir.Rotation();
			Myloc += (50.f * dir / dir.Size());
			q.edge = GetWorld()->SpawnActor<AGraphEdge>(Edge, Myloc, pointTo, SpawnParamenters);
			FVector scale = { 1,1,dir.Size() - 100.f };
			q.edge->SetActorScale3D(scale);
			q.i = cur->left->my_i;
			q.j = cur->left->my_j;
			q.k = cur->left->my_k;
			cur->edges.push_back(q);
			cur = cur->left;
			//cur_step++;
			return;
		}
		else if (cur->ct == 1)
		{
			SpawnPosition = cur->SpawnPosition;
			SpawnPosition.Z -= DistanceBetweenNodes;
			SpawnPosition.Y += cur->val * cur->val * DistanceBetweenNodes;
			cur->right = GetWorld()->SpawnActor<AGraphNode>(Node, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
			cur->right->val = cur->val - 2;
			cur->right->SpawnPosition = SpawnPosition;
			cur->right->Text->SetText(FText::FromString(FString::FromInt(cur->right->val)));
			cur->right->parent = cur;
			if (cur->right->val <= 1)
			{
				cur->right->ct = 0;
			}
			else cur->right->ct = 2;


			AGraphNode::edge_to q;
			FVector Myloc = cur->GetActorLocation();
			Myloc.Z += 50;
			FVector Tarloc = cur->right->GetActorLocation();
			Tarloc.Z += 50;
			FVector dir = Tarloc - Myloc;
			FRotator pointTo = dir.Rotation();
			Myloc += (50.f * dir / dir.Size());
			q.edge = GetWorld()->SpawnActor<AGraphEdge>(Edge, Myloc, pointTo, SpawnParamenters);
			FVector scale = { 1,1,dir.Size() - 100.f };
			q.edge->SetActorScale3D(scale);
			q.i = cur->right->my_i;
			q.j = cur->right->my_j;
			q.k = cur->right->my_k;
			cur->edges.push_back(q);
			cur = cur->right;

			//cur_step++;
			return;

		}
		else
		{
			me[cur->val] = true;
			if (cur == groot)
			{
				cur_step = -2;
				return;
			}
			cur = cur->parent;
			cur->ct--;
			return;
		}

	}
	else if (cur_step == 2)
	{
		if (!next && !AUTO)
		{
			return;
		}
		next = false;
		/*
		// remove current cur thing
		cur = first[0];
		cur->parent->ct--;







		cur_step--;
		*/



	}
}

void AGraph::r_graph(bool& retflag)
{
	retflag = true;

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
			UE_LOG(LogTemp, Warning, TEXT("%d %d %d"), i, j, k);
			if (rand() % ProbabilityIn1byX == 0)
			{
				grid3d[i][j][k] = GetWorld()->SpawnActor<AGraphNode>(Node, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
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
	if (cur_step == 1)
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
		Myloc += (52.f * dir / dir.Size());
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
			if (who->IsA(AGraphNode::StaticClass()))
			{
				AGraphNode* hit_node = Cast<AGraphNode>(who);
				if (hit_node->my_i == Store[j]->my_i && hit_node->my_j == Store[j]->my_j && hit_node->my_k == Store[j]->my_k)
				{
					q.edge = GetWorld()->SpawnActor<AGraphEdge>(Edge, Myloc, pointTo, SpawnParamenters);
					FVector scale = { 1,1,dir.Size() - 100.f };
					q.edge->SetActorScale3D(scale);
					q.i = Store[j]->my_i;
					q.j = Store[j]->my_j;
					q.k = Store[j]->my_k;
					Store[j]->edges.push_back(q);
					q.i = Store[i]->my_i;
					q.j = Store[i]->my_j;
					q.k = Store[i]->my_k;
					Store[i]->edges.push_back(q);
					make = true;
				}
			}
			if (!make)
			{
				UE_LOG(LogTemp, Warning, TEXT("Edge not made from %s to %s because %s was in the way!"), *Store[i]->GetName(), *Store[j]->GetName(), *who->GetName());
				DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true);
			}
		}

		j++;
	}
	retflag = false;
}

uint64 AGraph::min(uint64 a, uint64 b)
{
	if (a < b) return a;
	return b;
}