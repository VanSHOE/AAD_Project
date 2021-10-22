// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include <deque>
#include "EdgeHead.h"
#include "GraphNode.h"
#include "EWeight.h"
#include "Grid.h"
#include "GameFramework/Actor.h"
#include "Graph.generated.h"



UCLASS()
class MYPROJECT_API AGraph : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGraph();
	uint64 min(uint64 a, uint64 b);

	UPROPERTY(EditAnywhere)
		uint64 size_x;
	UPROPERTY(EditAnywhere)
		uint64 size_y;
	UPROPERTY(EditAnywhere)
		uint64 size_z;
	UPROPERTY(EditAnywhere)
		uint64 nodes;

	//UPROPERTY(EditAnywhere)
		uint64 mn = 5;
	uint64 fib_n = 5;

	//UPROPERTY(EditAnywhere)
		uint64 edges;
	UPROPERTY(EditAnywhere)
		int DistanceBetweenNodes = 400;
	UPROPERTY(EditAnywhere)
		int ProbabilityIn1byX = 5;
	UPROPERTY(EditAnywhere)
		int Ep = 5;

	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
		TSubclassOf<AGrid> GBP;
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
		TSubclassOf<AGraphNode> Node;
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
		TSubclassOf<AGraphEdge> Edge;
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
		TSubclassOf<AEdgeHead> Head;
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
		TSubclassOf<AEWeight> WtText;
	uint64 i = 0;
	uint64 j = 0;
	uint64 k = 0;
	std::vector<std::vector<std::vector<AGraphNode*>>> grid3d;
	std::vector<AGraphNode*> Store;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool next = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AUTO = false;
	UPROPERTY(VisibleAnywhere)
		int cur_step = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float headsize = 25.f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	std::deque< AGraphNode*> first;
	std::deque< AGraphNode*> second;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool mem = false;
	std::vector<bool> me;
	virtual void Tick(float DeltaTime) override;
	void Fib_dp(bool& retflag);
	void r_graph(bool& retflag);
	FVector SpawnPosition;
	FActorSpawnParameters SpawnParamenters;
	void node_color(AGraphNode* n, bool green);
	class AGrid* mat;
	UPROPERTY(EditAnywhere)
		double delay = 0.01f;
	FString c2s(int l, int r);
private:
	int c_val = 0;
	int g_index;
	int ox, oy, oz;
	int cnodes = 0;
	double last = 0;
	struct msrtflags
	{
		int i = 0;
		int j = 0;
		int k = 0;
		bool l = true;
		bool r = true;
		void reset()
		{
			i = j = k = 0;
			l = r = true;
		}
	} msi;
	AGraphNode* groot;
	bool skip = false;
	int cur_bfs = 0;
	AGraphNode* cur;
};
