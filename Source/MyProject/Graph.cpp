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
	r_store.clear();
	Edge_Store.clear();
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
				grid3d[i][j][k]->Text->SetText(FText::FromString(FString::FromInt(c_val++)));
				grid3d[i][j][k]->my_i = i;
				grid3d[i][j][k]->my_j = j;
				grid3d[i][j][k]->my_k = k;
				cnodes++;
				Store.push_back(grid3d[i][j][k]);
				r_store.push_back(grid3d[i][j][k]);
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
			j = 0;
		}
		if (i >= nodes - 1)
		{
			i = j = 0;
			cur_step = 2;
			cur = Store[0];
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
					q.edge->head = GetWorld()->SpawnActor<AEdgeHead>(Head, hlocl, pointTo, SpawnParamenters);
					FVector scale = { 1,1,dir.Size() - (100 + headsize)};
					q.edge->SetActorScale3D(scale);
					q.i = Store[j]->my_i;
					q.j = Store[j]->my_j;
					q.k = Store[j]->my_k;
					q.nbor = Store[j];
					Store[i]->edges.push_back(q);
					EdgeStorage qq;
					qq.edge = q.edge;
					qq.from = Store[i];
					qq.to = Store[j];
					Edge_Store.push_back(qq);
				}
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
		//UE_LOG(LogTemp, Warning, TEXT("G, %d"), first.size());
		//
		UE_LOG(LogTemp, Warning, TEXT("At: %s"), *cur->GetName());
		
		
		if (!skip)
			first.push_front(cur);
		if (cur->visited && !skip)
		{
			next = skip = true;
			UE_LOG(LogTemp, Warning, TEXT("Already visited: %s, now returning."), *cur->GetName());
			first.pop_front();
			//node_color(cur, 0);
			if (first.size() == 0)
			{
				cur_step = -999;
				return;
			}
			cur = first[0];
			return;
		}
		node_color(cur, 1);
		//UE_LOG(LogTemp, Warning, TEXT("new G, %d"), first.size());
		//if (!skip)
			
		skip = false;
		cur->visited = true;

		if (cur->ct >= cur->edges.size())
		{
			UE_LOG(LogTemp, Warning, TEXT("All children over at: %s"), *cur->GetName());
			skip = true;
			first.pop_front();
			node_color(cur, 0);
			if (first.size() == 0)
			{
				skip = false;
				cur_step = 3;
				return;
			}
			cur = first[0];
			return;
		}
		
		cur = cur->edges[cur->ct++].nbor;
		if (cur->visited)
		{
			next = true;
		}
		UE_LOG(LogTemp, Warning, TEXT("Going to: %s"), *cur->GetName());

	}
	else if (cur_step == 3)
	{
		if (!next && !AUTO)
		{
			return;
		}
		next = false;
		bool found = false;
		UE_LOG(LogTemp, Warning, TEXT("We are now searching"));
		for (int ii = 0; ii < Store.size(); ii++)
		{
			if (Store[ii]->visited == false)
			{
				cur = Store[ii];
				node_color(cur, 1);
				cur_step = 2;
				return;
			}
		}
		first.clear();
		cur_step = 4;
	
	}	
	else if (cur_step == 4)
	{
		for (auto x : Store)
		{
			x->edges.clear();
			x->visited = false;
			x->ct = 0;
			node_color(x, false, 0.35f);
		}
		for (int ii = 0; ii < Edge_Store.size(); ii++)
		{
			EdgeStorage cur_rev = Edge_Store[ii];
			cur_rev.edge->head->Destroy();
			cur_rev.edge->Destroy();
			
			AGraphNode::edge_to q;
			//UE_LOG(LogTemp, Warning, TEXT("%d is store size, cnodes is %d we need %d and %d"), Store.size(), cnodes, i, j);
			FVector Myloc = cur_rev.to->GetActorLocation();
			Myloc.Z += 50;
			FVector Tarloc = cur_rev.from->GetActorLocation();
			Tarloc.Z += 50;
			FVector dir = Tarloc - Myloc;
			FRotator pointTo = dir.Rotation();
			Myloc += (50.f * dir / dir.Size());
			//UE_LOG(LogTemp, Warning, TEXT("Between %s and %s, vector is %s, rotator is %s"), *Store[i]->GetName(), *Store[j]->GetName(), *dir.ToString(), *pointTo.ToString());
			FVector ForwardVector = UKismetMathLibrary::GetDirectionUnitVector(Myloc, Tarloc);
			q.edge = GetWorld()->SpawnActor<AGraphEdge>(Edge, Myloc, pointTo, SpawnParamenters);
			auto hlocl = Tarloc;
			hlocl -= (50.f + headsize) * ForwardVector;
			q.edge->head = GetWorld()->SpawnActor<AEdgeHead>(Head, hlocl, pointTo, SpawnParamenters);
			FVector scale = { 1,1,dir.Size() - (100 + headsize) };
			q.edge->SetActorScale3D(scale);
			q.i = cur_rev.from->my_i;
			q.j = cur_rev.from->my_j;
			q.k = cur_rev.from->my_k;
			q.nbor = cur_rev.from;
			cur_rev.to->edges.push_back(q);
		}
		cur = Store[0];
		cur_step = 2;
	}
}


void AGraph::r_graph(bool& retflag)
{
	retflag = true;


	retflag = false;
}

int64 AGraph::min(int64 a, int64 b)
{
	if (a < b) return a;
	return b;
}

void AGraph::node_color(AGraphNode* n, bool green, float op)
{
	UStaticMeshComponent* comp = Cast<UStaticMeshComponent>(n->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	auto mt = n->mt;
	auto dyn = UMaterialInstanceDynamic::Create(mt, NULL);
	dyn->SetScalarParameterValue(TEXT("Opacity"), op);
	dyn->SetScalarParameterValue(TEXT("col"), green);
	comp->SetMaterial(0, dyn);
}
void AGraph::edge_color(AGraphEdge* n, bool green, bool shine)
{
	UStaticMeshComponent* comp = Cast<UStaticMeshComponent>(n->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	auto mt = n->mt;
	auto dyn = UMaterialInstanceDynamic::Create(mt, NULL);
	if (shine)
	{
		dyn->SetScalarParameterValue(TEXT("light"), 3.f);
	}
	else dyn->SetScalarParameterValue(TEXT("light"), 0.f);

	if (green) //|| inPath.find(n) != inPath.end())
	{
		dyn->SetScalarParameterValue(TEXT("colsel"), 1.f);
	}
	else dyn->SetScalarParameterValue(TEXT("colsel"), 0.f);

	comp->SetMaterial(0, dyn);
}
FString AGraph::c2s(int32 l, int32 r)
{
	return "[" + FString::FromInt(l) + ", " + FString::FromInt(r) + ")";
}
void AGraph::setpq()
{
	int ii = 0;
	for (auto x : pq)
	{	
		if (ii >= pqmat->size_y) return;
		FString pqstring = c2s(x.second->id, x.first);
		pqmat->grid3d[ii++][0]->Text->SetText(FText::FromString(pqstring));
	}
	while (ii < pqmat->size_y)
	{
		pqmat->grid3d[ii][0]->Text->SetText(FText::FromString("-"));
		ii++;
	}

}