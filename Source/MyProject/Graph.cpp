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
	/*
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
					FVector scale = { 1,1,dir.Size() - (100 + headsize)};
					q.edge->SetActorScale3D(scale);
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
	*/

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
		auto sp = SpawnPosition;
		sp.Z -= 400.f;
		sp.Y -= 200.f;
		sp.X -= 200.f * fib_n;
		groot->marray = GetWorld()->SpawnActor<AGrid>(GBP, sp, FRotator::ZeroRotator, SpawnParamenters);
		groot->marray->size_x = groot->val.r = fib_n;

		groot->marray->grid3d.assign(1, std::vector<AGrid_Cell*>(fib_n));
		groot->marray->grid.assign(1, std::vector<int>(fib_n, -1));
		for (int ii = 0; ii < fib_n; ii++)
		{
			groot->marray->grid[0][ii] = rand() % (2 * fib_n) - fib_n + 1;
		}
		groot->marray->next = true;

		groot->val.l = 0;
		groot->SpawnPosition = SpawnPosition;
		groot->Text->SetText(FText::FromString(c2s(0, fib_n)));
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
			SpawnPosition.Y += DistanceBetweenNodes;
			SpawnPosition.X -= (cur->val.r - cur->val.l + 1) * DistanceBetweenNodes;

			cur->left = GetWorld()->SpawnActor<AGraphNode>(Node, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
			cur->left->val.l = cur->val.l;
			cur->left->val.r = (cur->val.l + cur->val.r) / 2;

			auto sp = SpawnPosition;
			sp.Z -= 400.f;
			sp.Y -= 200.f;
			sp.X -= 200.f * (cur->left->val.r - cur->left->val.l);
			cur->left->marray = GetWorld()->SpawnActor<AGrid>(GBP, sp, FRotator::ZeroRotator, SpawnParamenters);
			cur->left->marray->size_x = cur->left->val.r - cur->left->val.l;
			cur->left->marray->grid3d.assign(1, std::vector<AGrid_Cell*>(cur->left->marray->size_x));
			cur->left->marray->grid.assign(1, std::vector<int>(cur->left->marray->size_x, -1));
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
			//cur_step++;
			return;
		}
		else if (cur->ct == 1)
		{
			SpawnPosition = cur->SpawnPosition;
			SpawnPosition.Y += DistanceBetweenNodes;
			SpawnPosition.X += (cur->val.r - cur->val.l) * DistanceBetweenNodes;
			cur->right = GetWorld()->SpawnActor<AGraphNode>(Node, SpawnPosition, FRotator::ZeroRotator, SpawnParamenters);
			cur->right->val.l = (cur->val.l + cur->val.r) / 2;
			cur->right->val.r = cur->val.r;

			auto sp = SpawnPosition;
			sp.Z -= 400.f;
			sp.Y -= 200.f;
			sp.X -= 200.f * (cur->right->val.r - cur->right->val.l);
			cur->right->marray = GetWorld()->SpawnActor<AGrid>(GBP, sp, FRotator::ZeroRotator, SpawnParamenters);
			cur->right->marray->size_x = cur->right->val.r - cur->right->val.l;
			cur->right->marray->grid3d.assign(1, std::vector<AGrid_Cell*>(cur->right->marray->size_x));
			cur->right->marray->grid.assign(1, std::vector<int>(cur->right->marray->size_x, -1));
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

			//cur_step++;
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
				cur->marray->text_color(0, 0, 3);
				cur->left->marray->text_color(0, 0, 3);
				cur->right->marray->text_color(0, 0, 3);
				msi.reset();
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
		if (msi.i >= cur->left->marray->size_x)
		{
			msi.l = false;
		}
		if (msi.j >= cur->right->marray->size_x)
		{
			msi.r = false;
		}
		if (msi.l && msi.r)
		{
			if (cur->left->marray->grid[0][msi.i] < cur->right->marray->grid[0][msi.j])
			{
				cur->marray->text_color(0, msi.k, 2);
				cur->left->marray->text_color(0, msi.i, 2);

				cur->marray->grid[0][msi.k++] = cur->left->marray->grid[0][msi.i++];

				if(msi.k < cur->marray->size_x)
					cur->marray->text_color(0, msi.k, 3);
				if(msi.i < cur->left->marray->size_x)
					cur->left->marray->text_color(0, msi.i, 3);
			}
			else
			{
				cur->marray->text_color(0, msi.k, 2);
				cur->right->marray->text_color(0, msi.j, 2);

				cur->marray->grid[0][msi.k++] = cur->right->marray->grid[0][msi.j++];

				if (msi.k < cur->marray->size_x)
					cur->marray->text_color(0, msi.k, 3);
				if (msi.j < cur->right->marray->size_x)
					cur->right->marray->text_color(0, msi.j, 3);
			}
			cur->marray->next = true;
		}
		else if (msi.l)
		{
			cur->marray->text_color(0, msi.k, 2);
			cur->left->marray->text_color(0, msi.i, 2);

			cur->marray->grid[0][msi.k++] = cur->left->marray->grid[0][msi.i++];

			if (msi.k < cur->marray->size_x)
				cur->marray->text_color(0, msi.k, 3);
			if (msi.i < cur->left->marray->size_x)
				cur->left->marray->text_color(0, msi.i, 3);

			cur->marray->next = true;
		}
		else if (msi.r)
		{
			cur->marray->text_color(0, msi.k, 2);
			cur->right->marray->text_color(0, msi.j, 2);

			cur->marray->grid[0][msi.k++] = cur->right->marray->grid[0][msi.j++];

			if (msi.k < cur->marray->size_x)
				cur->marray->text_color(0, msi.k, 3);
			if (msi.j < cur->right->marray->size_x)
				cur->right->marray->text_color(0, msi.j, 3);
			cur->marray->next = true;
		}
		else
		{
			msi.reset();
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