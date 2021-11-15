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
	SpawnParameters.Owner = this;
	SpawnPosition = GetActorLocation();
	ox = SpawnPosition.X;
	oy = SpawnPosition.Y;
	oz = SpawnPosition.Z;
	cur_bfs = 0;
	Store.clear();
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
	TArray<AActor*> a;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrid::StaticClass(), a);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *a[0]->GetName());
	mat = Cast<AGrid>(a[0]);
	mat->size_y = mat->size_x = nodes;
	mat->setSize();
	mat->next = true;
	second.clear();
	edges = 0;
	bfc.reset();
	bfo.clear();
	bfo.assign(nodes, INT_MAX);
	bfknown.clear();
	bfknown.assign(nodes, INT_MAX);
	prev.clear();
	for (int ii = 0; ii < nodes; ii++)
	{
		EdgeStorage q;
		prev.push_back(q);
	}
	bfo[0] = 0;
	bfknown[0] = 0;
	inPath.clear();
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
				grid3d[i][j][k] = GetWorld()->SpawnActor<AGraphNode>(Node, SpawnPosition, FRotator::ZeroRotator, SpawnParameters);
				grid3d[i][j][k]->val = c_val;
				grid3d[i][j][k]->Text->SetText(FText::FromString(FString::FromInt(c_val++)));
				grid3d[i][j][k]->my_i = i;
				grid3d[i][j][k]->my_j = j;
				grid3d[i][j][k]->my_k = k;
				grid3d[i][j][k]->id = grid3d[i][j][k]->val;
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
			j = 0;
		}
		if (i >= nodes - 1)
		{
			i = j = 0;
			cur_step = 3;
			for (int ii = 0; ii < Edge_Store.size(); ii++)
			{
				int swap2 = rand() % Edge_Store.size();
				auto temp = Edge_Store[ii];
				Edge_Store[ii] = Edge_Store[swap2];
				Edge_Store[swap2] = temp;
			}
			//second.push_back(Store[0]);
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
			if (who->IsA(AGraphNode::StaticClass()) && rand() % Ep == 0)
			{
				AGraphNode* hit_node = Cast<AGraphNode>(who);
				if (hit_node->my_i == Store[j]->my_i && hit_node->my_j == Store[j]->my_j && hit_node->my_k == Store[j]->my_k)
				{
					q.edge = GetWorld()->SpawnActor<AGraphEdge>(Edge, Myloc, pointTo, SpawnParameters);
					auto hlocl = Tarloc;
					hlocl -= (50.f + headsize) * ForwardVector;
					GetWorld()->SpawnActor<AEdgeHead>(Head, hlocl, pointTo, SpawnParameters);
					FVector scale = { 1, 1, dir.Size() - (100 + headsize) };
					q.edge->SetActorScale3D(scale);
					FVector wtpos = Myloc + ForwardVector * (dir.Size() / 2);
					q.edge->Text = GetWorld()->SpawnActor<AEWeight>(WtText, wtpos, pointTo, SpawnParameters);
					q.edge->Text->val = rand() % (2 * MaxWT);// -MaxWT;
					q.edge->Text->Text->SetText(FText::FromString(FString::FromInt(q.edge->Text->val)));
					//	q.edge->dp = GetWorld()->SpawnActor<AEWeight>(WtText, wtpos, pointTo, SpawnParamenters);
					//	q.edge->dp->val = INT_MAX;
					//	q.edge->dp->Text->SetText(FText::FromString(TEXT("Inf")));

					q.i = Store[j]->my_i;
					q.j = Store[j]->my_j;
					q.k = Store[j]->my_k;
					q.nbor = Store[j];
					Store[i]->edges.push_back(q);
					EdgeStorage qq;
					qq.from = Store[i];
					qq.to = Store[j];
					qq.edge = q.edge;
					Edge_Store.push_back(qq);

					edges++;
					//q.i = Store[i]->my_i;
					//q.j = Store[i]->my_j;
					//q.k = Store[i]->my_k;
					//q.nbor = Store[i];
					//Store[j]->edges.push_back(q);
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
		if (bfc.j >= edges)
		{
			bfc.j = 0;
			bfc.i++;
			for (int qq = 0; qq < Edge_Store.size(); qq++)
			{
				edge_color(Edge_Store[qq].edge, false, false);
			}
		}
		if (bfc.i >= nodes)
		{
			cur_step = -2;
			return;
		}
		auto cedge = Edge_Store[bfc.j];
		edge_color(cedge.edge, false, true);
		UE_LOG(LogTemp, Warning, TEXT("%d: %d %d %d"), bfc.i, cedge.from->id, cedge.to->id, cedge.edge->Text->val);
		/*	if (mat->grid[bfc.i][cedge.from->id] != INT_MAX && mat->grid[bfc.i][cedge.from->id] + cedge.edge->Text->val < mat->grid[bfc.i][cedge.to->id])
		{
			mat->grid[bfc.i][cedge.to->id] = mat->grid[bfc.i][cedge.from->id] + cedge.edge->Text->val;
			UE_LOG(LogTemp, Warning, TEXT("in"));
			mat->up(bfc.i, cedge.to->id);
		}*/
		UE_LOG(LogTemp, Warning, TEXT("%d < %d and from edge is %d "), bfo[cedge.from->id] + cedge.edge->Text->val, bfo[cedge.to->id], bfo[cedge.from->id]);
		if (bfo[cedge.from->id] != INT_MAX && bfo[cedge.from->id] + cedge.edge->Text->val < bfo[cedge.to->id])
		{
			bfo[cedge.to->id] = bfo[cedge.from->id] + cedge.edge->Text->val;
			UE_LOG(LogTemp, Warning, TEXT("in"));
		}
		if (bfo[cedge.to->id] != INT_MAX)
		{
			//mat->grid[bfc.i][cedge.to->id] = bfo[cedge.to->id];
			mat->up(bfc.i, cedge.to->id, bfo[cedge.to->id]);
		}
		bfc.j++;
	}
	else if (cur_step == 3)
	{

		for (int ii = 0; ii < nodes - 1; ii++)
		{
			bool over = true;
			for (int jj = 0; jj < edges; jj++)
			{
				auto cedge = Edge_Store[jj];
				if (bfknown[cedge.from->id] != INT_MAX && bfknown[cedge.from->id] + cedge.edge->Text->val < bfknown[cedge.to->id])
				{
					over = false;
					bfknown[cedge.to->id] = bfknown[cedge.from->id] + cedge.edge->Text->val;
					prev[cedge.to->id] = cedge;
				}
			}
			if (!over) MaxIt++;
		}

		for (int jj = 0; jj < edges; jj++)
		{
			auto cedge = Edge_Store[jj];
			if (bfknown[cedge.from->id] != INT_MAX && bfknown[cedge.from->id] + cedge.edge->Text->val < bfknown[cedge.to->id])
			{
				bfknown[cedge.to->id] = bfknown[cedge.from->id] + cedge.edge->Text->val;
				prev[cedge.to->id] = cedge;
				NegativeCycle = true;
				cur_step = 2;
				return;
			}
		}

		int curr = -1;
		int mxx = -1;
		for (int ii = 1; ii < nodes; ii++)
		{
			int len = 0;
			//auto cedge = Edge_Store[ii];

			if (bfknown[ii] == INT_MAX) continue;
			int temp = ii;
			while (temp)
			{
				len++;
				temp = prev[temp].from->id;
			}
			if (len >= mxx)
			{
				mxx = len;
				curr = ii;
			}
		}
		if (curr == -1)
		{
			cur_step = 2;
			return;
		}

		auto temp = curr;
		while (temp)
		{
			auto e = prev[temp];
			edge_color(e.edge, true, false);
			inPath.insert(e.edge);
			temp = e.from->id;
		}
		cur_step = 2;
	}
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
/*
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
*/




