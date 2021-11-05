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

		groot = GetWorld()->SpawnActor<AGraphNode>(Node, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
		auto sp = SpawnPosition;
		sp.Z -= 400.f;
		sp.Y -= 200.f;
		sp.X -= 200.f * fib_n;
		groot->marray = GetWorld()->SpawnActor<AGrid>(GBP, sp, FRotator::ZeroRotator, SpawnParamenters);
		groot->marray->size_x = groot->val.r = fib_n;

		groot->marray->grid3d.assign(1, std::vector<AGrid_Cell*>(fib_n));
		groot->marray->grid.assign(1, std::vector<int64>(fib_n, -1));
		groot->marray->grid[0][0] = rand() % (2 * fib_n) - fib_n + 1;
		for (int ii = 1; ii < fib_n; ii++)
		{
			groot->marray->grid[0][ii] = groot->marray->grid[0][ii - 1] + rand() % fib_n + 1;
		}
		groot->marray->next = true;

		groot->val.l = 0;
		groot->SpawnPosition = SpawnPosition;
		groot->Text->SetText(FText::FromString(c2s(0, fib_n)));
		groot->ct = 2;
		cur = groot;

		qm.reset();
		cur_step = 3;
	}
	else if (cur_step == 1)
	{

		if (!next && !AUTO)
		{
			return;
		}
		next = false;
		if (cur->ct == 2)
		{
			SpawnPosition = cur->SpawnPosition;
			SpawnPosition.Y += DistanceBetweenNodes;
			SpawnPosition.X -= (cur->val.r - cur->val.l + 1) * DistanceBetweenNodes;
			if (cur->ppt < cur->val.l)
			{
				UE_LOG(LogTemp, Error, TEXT("ppt less than l"));
			}
			else if (cur->ppt == cur->val.l)
			{
				cur->ct--;
				return;
			}
			cur->left = GetWorld()->SpawnActor<AGraphNode>(Node, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
			cur->left->val.l = cur->val.l;
			cur->left->val.r = cur->ppt;

			auto sp = SpawnPosition;
			sp.Z -= 400.f;
			sp.Y -= 200.f;
			sp.X -= 200.f * (cur->left->val.r - cur->left->val.l);
			cur->left->marray = GetWorld()->SpawnActor<AGrid>(GBP, sp, FRotator::ZeroRotator, SpawnParamenters);
			cur->left->marray->size_x = cur->left->val.r - cur->left->val.l;
			cur->left->marray->grid3d.assign(1, std::vector<AGrid_Cell*>(cur->left->marray->size_x));
			cur->left->marray->grid.assign(1, std::vector<int64>(cur->left->marray->size_x, -1));
			for (int ii = 0; ii < cur->left->marray->size_x; ii++)
			{
				cur->left->marray->grid[0][ii] = cur->marray->grid[0][ii];
			}
			cur->left->marray->next = true;

			cur->left->SpawnPosition = SpawnPosition;
			cur->left->Text->SetText(FText::FromString(c2s(cur->left->val.l, cur->left->val.r)));
			cur->left->parent = cur;
			if (cur->left->val.l + 2 >= cur->left->val.r)
			{
				cur->left->ct = 0;
			}
			else
				cur->left->ct = 2;

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
			qm.reset();
			
			cur_step = 3;
			return;
		}
		else if (cur->ct == 1)
		{
			if (cur->ppt + 1 == cur->val.r)
			{
				cur->ct--;
				return;
			}
			SpawnPosition = cur->SpawnPosition;
			SpawnPosition.Y += DistanceBetweenNodes;
			SpawnPosition.X += (cur->val.r - cur->val.l) * DistanceBetweenNodes;
			cur->right = GetWorld()->SpawnActor<AGraphNode>(Node, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
			cur->right->val.l = cur->ppt + 1;
			cur->right->val.r = cur->val.r;

			auto sp = SpawnPosition;
			sp.Z -= 400.f;
			sp.Y -= 200.f;
			sp.X -= 200.f * (cur->right->val.r - cur->right->val.l);
			cur->right->marray = GetWorld()->SpawnActor<AGrid>(GBP, sp, FRotator::ZeroRotator, SpawnParamenters);
			cur->right->marray->size_x = cur->right->val.r - cur->right->val.l;
			cur->right->marray->grid3d.assign(1, std::vector<AGrid_Cell*>(cur->right->marray->size_x));
			cur->right->marray->grid.assign(1, std::vector<int64>(cur->right->marray->size_x, -1));
			for (int ii = cur->right->marray->size_x - 1; ii >= 0; ii--)
			{
				cur->right->marray->grid[0][ii] = cur->marray->grid[0][cur->marray->size_x - 1 - (cur->right->marray->size_x - 1 - ii)];
			}
			cur->right->marray->next = true;

			cur->right->SpawnPosition = SpawnPosition;
			cur->right->Text->SetText(FText::FromString(c2s(cur->right->val.l, cur->right->val.r)));
			cur->right->parent = cur;

			if (cur->right->val.l + 2 >= cur->right->val.r)
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
			qm.reset();
			cur_step = 3;
			return;
		}
		else
		{
			if (cur->val.r - cur->val.l == 2)
			{
				if (cur->marray->grid[0][0] > cur->marray->grid[0][1])
				{
					int temp = cur->marray->grid[0][0];
					cur->marray->grid[0][0] = cur->marray->grid[0][1];
					cur->marray->grid[0][1] = temp;
					cur->marray->next = true;
				}
			}
			else if (cur->val.r - cur->val.l > 2)
			{
				cur_step = 2;
			//	cur->marray->text_color(0, 0, 3);
			//	cur->left->marray->text_color(0, 0, 3);
			//	cur->right->marray->text_color(0, 0, 3);
				qc.reset();
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
		if(qc.j && qc.j - 1 != cur->ppt - cur->val.l)
			cur->marray->text_color(0, qc.j - 1, 2);
		if(qc.j < cur->marray->size_x)
			cur->marray->text_color(0, qc.j, 3);
		if (qc.j == cur->ppt - cur->val.l)
		{
			cur->marray->text_color(0, qc.j, 1);
			qc.j++;
		}
		if (cur->left == nullptr || qc.i >= cur->left->marray->size_x)
		{
			qc.l = true;
		}
		if (cur->left != nullptr)
		{	
			if (cur->right == nullptr)
			{
				qc.r = true;
			}
			else if (qc.l && qc.i - cur->left->marray->size_x >= cur->right->marray->size_x)
			{
				qc.r = true;
			}
		}
		else
		{
			if (cur->right == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Why is left and right both empty?"));
			}
			if (qc.i >= cur->right->marray->size_x)
			{
				qc.r = true;
			}
		}
		if (qc.l == false)
		{
			if (qc.i != cur->left->ppt - cur->left->val.l)
				cur->left->marray->text_color(0, qc.i, 3);
			cur->marray->grid[0][qc.j++] = cur->left->marray->grid[0][qc.i++];
			cur->marray->next = true;
		}
		else if (qc.r == false)
		{
			
			if (cur->left != nullptr)
			{
				if(qc.i - cur->left->marray->size_x != cur->right->ppt - cur->right->val.l)
					cur->right->marray->text_color(0, qc.i - cur->left->marray->size_x, 3);
				cur->marray->grid[0][qc.j++] = cur->right->marray->grid[0][(qc.i++) - cur->left->marray->size_x];
			}
			else
			{
				if (qc.i != cur->right->ppt - cur->right->val.l)
					cur->right->marray->text_color(0, qc.i, 3);
				cur->marray->grid[0][qc.j++] = cur->right->marray->grid[0][qc.i++];
			}
			cur->marray->next = true;
		}
		else
		{
			qc.reset();
			if (cur == groot)
			{
				cur_step = -2;
				return;
			}

			cur = cur->parent;
			cur->ct--;
			cur_step = 1;
			return;
		}
	}
	else if (cur_step == 3)
	{
		if (!next && !AUTO)
		{
			return;
		}
		next = false;
		cur->marray->text_color(0, cur->marray->size_x - 1, 3);
		if (qm.i + 1>= 0)
			cur->marray->text_color(0, qm.i + 1, 3);
		if(qm.j && qm.j - 1 != qm.i + 1)
			cur->marray->text_color(0, qm.j - 1, 0);
		if (qm.i + 1 > 0)
			cur->marray->text_color(0, qm.i, 0);
		cur->marray->text_color(0, qm.j, 1);
		cur_step++;
	}
	else if (cur_step == 4)
	{
		if (!next && !AUTO)
		{
			return;
		}
		next = false;

		if (qm.j >= cur->marray->size_x - 1)
		{
			int64 temp = cur->marray->grid[0][qm.i + 1];
			cur->marray->text_color(0, cur->marray->size_x - 1, 0);
			if (qm.j)
				cur->marray->text_color(0, qm.j - 1, 0);
			cur->marray->grid[0][qm.i + 1] = cur->marray->grid[0][cur->marray->size_x - 1];
			cur->marray->grid[0][cur->marray->size_x - 1] = temp;
			cur->marray->next = true;
			cur->ppt = qm.i + 1 + cur->val.l;
			cur->marray->text_color(0, qm.i + 1, 2);
			cur_step = 1;
			return;
		}

		if (cur->marray->grid[0][qm.j] <= cur->marray->grid[0][cur->marray->size_x - 1])
		{
			qm.i++;
			int64 temp = cur->marray->grid[0][qm.i];
			cur->marray->grid[0][qm.i] = cur->marray->grid[0][qm.j];
			cur->marray->grid[0][qm.j] = temp;
			cur->marray->next = true;
		}
		qm.j++;
		cur_step--;
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

void AGraph::node_color(AGraphNode* n, bool green)
{
	UStaticMeshComponent* comp = Cast<UStaticMeshComponent>(n->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	auto mt = n->mt;
	auto dyn = UMaterialInstanceDynamic::Create(mt, NULL);
	if (green)
	{
		dyn->SetScalarParameterValue(TEXT("Opacity"), 0.75f);
		dyn->SetScalarParameterValue(TEXT("col"), 1);
	}
	else dyn->SetScalarParameterValue(TEXT("Opacity"), 0.75f);
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