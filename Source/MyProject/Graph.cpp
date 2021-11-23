// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION
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
	groot = nullptr;
	i = j = k = 0;
	cur_step = 0;
	cnodes = 0;
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
	//me.assign(fib_n + 1, false);
	grid3d.assign(size_x, std::vector<std::vector<AGraphNode*>>(size_y, std::vector<AGraphNode*>(size_z, nullptr)));
	second.clear();
	totalFlow = cFlow = bfsFlow = 0;
	started = false;
	prev.clear();
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
				grid3d[i][j][k] = GetWorld()->SpawnActor<AGraphNode>(Node, SpawnPosition, FRotator::ZeroRotator, SpawnParameters);
				grid3d[i][j][k]->val = grid3d[i][j][k]->id = c_val;
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
			cur_step = 4;
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
					auto hlocl = Tarloc;
					auto hlocme = Myloc;
					hlocl -= (50.f + headsize) * ForwardVector;
					hlocme += (headsize) * ForwardVector;
					q.edge = GetWorld()->SpawnActor<AGraphEdge>(Edge, hlocme, pointTo, SpawnParameters);
					GetWorld()->SpawnActor<AEdgeHead>(Head, hlocl, pointTo, SpawnParameters);
					GetWorld()->SpawnActor<AEdgeHead>(Head, hlocme, (-dir).Rotation(), SpawnParameters);
					FVector scale = { 1, 1, dir.Size() - (100 + 2 * headsize) };
					q.edge->SetActorScale3D(scale);
					FVector wtpos = Myloc + ForwardVector * (dir.Size() / 2);
					q.edge->Text = GetWorld()->SpawnActor<AEWeight>(WtText, wtpos, pointTo, SpawnParameters);
					q.edge->RText = GetWorld()->SpawnActor<AEWeight>(WtText, wtpos, pointTo, SpawnParameters);
					FRotator reverseTextRot = { 0,180,180 };
					q.edge->Text->AddActorLocalRotation(reverseTextRot.Quaternion());
					if (!allowNegative)
						q.edge->Text->cap = rand() % (MaxWT) + (!zeroWT);
					else
						q.edge->Text->cap = rand() % (2 * MaxWT) - MaxWT;
					//q.edge->Text->Text->SetText(FText::FromString(FString::FromInt(q.edge->Text->val)));
					q.edge->Text->Set();
					q.edge->RText->cap = 0;
					q.edge->RText->Set();
					q.reverse = false;
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
				//	qq.reverse = 
					Edge_Store.push_back(qq);
					//mat->up(qq.from->val, qq.to->val, qq.edge->Text->val);
					edges++;
					//prev_floyd[qq.from->val][qq.to->val].push_back(qq);
					q.i = Store[i]->my_i;
					q.j = Store[i]->my_j;
				    q.k = Store[i]->my_k;
					q.nbor = Store[i];
					q.reverse = true;
					Store[j]->edges.push_back(q);
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

		if (second.size() == 0)
		{
			cur_step = 4;
			return;
		}
		cur = second[0].node;
		cFlow = second[0].cur_flow;
		cur->visited = true;
		UE_LOG(LogTemp, Warning, TEXT("Visited: %s"), *cur->GetName());
		node_color(cur, 1);
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
		UE_LOG(LogTemp, Warning, TEXT("On %s, child of %s"), *cur->edges[cur_bfs].nbor->GetName(), *cur->GetName());
		if (cur->edges[cur_bfs].nbor->visited == false && ((cur->edges[cur_bfs].reverse && (cur->edges[cur_bfs].edge->RText->used < cur->edges[cur_bfs].edge->RText->cap)) || (!cur->edges[cur_bfs].reverse && (cur->edges[cur_bfs].edge->Text->used < cur->edges[cur_bfs].edge->Text->cap)))) // TODO Add reverse checking
		{
			UE_LOG(LogTemp, Warning, TEXT("Adding %s, child of %s"), *cur->edges[cur_bfs].nbor->GetName(), *cur->GetName());
			node_color(cur->edges[cur_bfs].nbor, 0);
			cur->edges[cur_bfs].nbor->visited = true;
			int64 newflow;
			if(!cur->edges[cur_bfs].reverse)
				newflow = min(cFlow, cur->edges[cur_bfs].edge->Text->cap - cur->edges[cur_bfs].edge->Text->used);
			else
				newflow = min(cFlow, cur->edges[cur_bfs].edge->RText->cap - cur->edges[cur_bfs].edge->RText->used);
			if (cur->edges[cur_bfs].nbor == Store.back())
			{
				bfsFlow = newflow;
				cur_step = 4;
				//return;
			}
			EdgeStorage qqqq;
			qqqq.from = cur;
			qqqq.to = cur->edges[cur_bfs].nbor;
			qqqq.edge = cur->edges[cur_bfs].edge;
			qqqq.reverse = cur->edges[cur_bfs].reverse;
			prev[cur->edges[cur_bfs].nbor->id] = qqqq;
			second.push_back({ cur->edges[cur_bfs].nbor, newflow });
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
		/*
		UE_LOG(LogTemp, Warning, TEXT("We are now searching"));
		for (int ii = 0; ii < Store.size(); ii++)
		{
			if (Store[ii]->visited == false)
			{
				//cur = Store[ii];
				second.push_back(Store[ii]);
				node_color(cur, 1);
				cur_step = 2;
				return;
			}
		}
		cur_step = 99;
		*/
		reset_colors();
		for (auto x : Store)
		{
			x->visited = false;
		}
		cur_bfs = 0;
		if (started)
		{
			if (bfsFlow == 0)
			{
				cur_step = -99;
				return;
			}
			auto it = prev[nodes - 1];
			while (it.edge)
			{
				if (it.reverse == false)
				{
					it.edge->Text->used += bfsFlow;
					it.edge->Text->Set();
					it.edge->RText->used -= bfsFlow;
					it.edge->RText->Set();
				}
				else
				{
					it.edge->Text->used -= bfsFlow;
					it.edge->Text->Set();
					it.edge->RText->used += bfsFlow;
					it.edge->RText->Set();
				}
				edge_color(it.edge, true, true);
				if (it.from == Store[0]) break;
				it = prev[it.from->id];
			}
		}
		started = true;
		prev.clear();
		prev.resize(nodes);
		second.clear();
		second.push_back({ Store[0], INT_MAX });
		totalFlow += bfsFlow;
		bfsFlow = cFlow = 0;
		cur_step = 2;
	}
	//UE_LOG(LogTemp, Warning, TEXT("GOT OUT w"));

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