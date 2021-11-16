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
	prev_floyd.clear();
	prev_floyd.assign(nodes, std::vector < std::vector<EdgeStorage> >(nodes));
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
			cur_step = 2;
			for (int ii = 0; ii < nodes; ii++)
			{
				mat->up(ii, ii, 0);
			}
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
					if (!allowNegative)
						q.edge->Text->val = rand() % (MaxWT);
					else
						q.edge->Text->val = rand() % (2 * MaxWT) - MaxWT;
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
					mat->up(qq.from->val, qq.to->val, qq.edge->Text->val);
					edges++;
					prev_floyd[qq.from->val][qq.to->val].push_back(qq);
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
		
		if (bfc.j >= nodes)
		{
			bfc.j = 0;
			bfc.i++;
		}
		if (bfc.i >= nodes)
		{
			for (int ii = 0; ii < nodes; ii++)
			{
				for (int jj = 0; jj < nodes; jj++)
				{
					mat->text_color(ii, jj, 0);
				}
			}
			bfc.i =	bfc.j = 0;
			bfc.k++;
		}
		if (bfc.k >= nodes)
		{
			for (int ii = 0; ii < nodes; ii++)
			{
				for (int jj = 0; jj < nodes; jj++)
				{
					mat->text_color(ii, jj, 2);
				}
			}
			cur_step = -99;
			return;
		}
		reset_colors();
		for (int ii = 0; ii <= bfc.k; ii++)
		{
			node_color(Store[ii], false);
		}

		node_color(Store[bfc.j], true);
		for (auto x : prev_floyd[bfc.i][bfc.j])
		{
			node_color(x.from, true);
			edge_color(x.edge, true, true);
			node_color(x.to, true);
		}
		node_color(Store[bfc.i], true, 0.35f);
		UE_LOG(LogTemp, Log, TEXT("%d %d %d"), bfc.k, bfc.i, bfc.j);
		mat->text_color(bfc.i, bfc.j, 1);
		cur_step++;
	}
	else if (cur_step == 3)
	{
		if (!next && !AUTO)
		{
			return;
		}
		next = true;

		if (mat->grid[bfc.i][bfc.k] != INT_MAX && mat->grid[bfc.k][bfc.j] != INT_MAX && mat->grid[bfc.i][bfc.j] > mat->grid[bfc.i][bfc.k] + mat->grid[bfc.k][bfc.j])
		{
			next = false;
			mat->up(bfc.i, bfc.j, mat->grid[bfc.i][bfc.k] + mat->grid[bfc.k][bfc.j]);
			prev_floyd[bfc.i][bfc.j].clear();
			for (auto x : prev_floyd[bfc.i][bfc.k])
			{
				prev_floyd[bfc.i][bfc.j].push_back(x);
			}
			for (auto x : prev_floyd[bfc.k][bfc.j])
			{
				prev_floyd[bfc.i][bfc.j].push_back(x);
			}
			reset_colors();
			for (int ii = 0; ii <= bfc.k; ii++)
			{
				node_color(Store[ii], false);
			}
			for (auto x : prev_floyd[bfc.i][bfc.j])
			{
				node_color(x.from, true);
				edge_color(x.edge, true, true);
				node_color(x.to, true);
			}
			node_color(Store[bfc.i], true, 0.35f);
		}
		bfc.j++;
		cur_step--;
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
void AGraph::reset_colors()
{
	for (auto x : Edge_Store)
	{
		edge_color(x.edge, false, false);
	}
	for (auto x : Store)
	{
		node_color(x, false, 0.35f);
	}
}