// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include <set>
#include <deque>
#include <unordered_set>
#include "EdgeHead.h"
#include "GraphNode.h"
#include <map>
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
	int64 min(int64 a, int64 b);

	UPROPERTY(EditAnywhere)
		int64 size_x;
	UPROPERTY(EditAnywhere)
		int64 size_y;
	UPROPERTY(EditAnywhere)
		int64 size_z;
	UPROPERTY(EditAnywhere)
		int64 nodes;
	UPROPERTY(EditAnywhere)
		bool optimized = false;
	//UPROPERTY(EditAnywhere)
		int64 mn = 5;
	int64 fib_n = 5;

	//UPROPERTY(VisibleAnywhere)
		uint64 edges = 0;
	UPROPERTY(EditAnywhere)
		int DistanceBetweenNodes = 400;
	UPROPERTY(EditAnywhere)
		int ProbabilityIn1byX = 5;
	UPROPERTY(EditAnywhere)
		int Ep = 5;
	UPROPERTY(EditAnywhere)
		int MaxWT = 5;
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
	int64 i = 0;
	int64 j = 0;
	int64 k = 0;
	std::vector<std::vector<std::vector<AGraphNode*>>> grid3d;
	std::vector<AGraphNode*> Store;
	std::vector<AGraphNode*> r_store;
	struct EdgeStorage
	{
		AGraphEdge* edge = nullptr;
		AGraphNode* from = nullptr;
		AGraphNode* to = nullptr;
		EdgeStorage()
		{
			edge = nullptr;
			from = to = nullptr;
		}
	};
	std::vector<EdgeStorage> Edge_Store;
	std::map<std::pair<AGraphNode*, AGraphNode*>, AGraphEdge*> adjm;

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
	std::deque< AGraphNode*> fStack;
	std::deque< AGraphNode*> second;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool mem = false;
	std::vector<bool> me;
	virtual void Tick(float DeltaTime) override;
	void Fib_dp(bool& retflag);
	void r_graph(bool& retflag);
	FVector SpawnPosition;
	FActorSpawnParameters SpawnParameters;
	void node_color(AGraphNode* n, bool green, float opacity = 0.75f);
	void edge_color(AGraphEdge* n, bool green = true, bool shine = true);
	AGrid* mat;
	AGrid* b_mat;
	UPROPERTY(EditAnywhere)
		double delay = 0.01f;
	FString c2s(int l, int r);
	void setpq();

private:
	int d_idx = 0;
	bool secondDFS = false;
	int setCounter = 0;
	int c_val = 0;
	int g_index;
	int ox, oy, oz;
	int cnodes = 0;
	double last = 0;
	//int ppt;
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

	struct SCCit
	{
		FString ans = "";
		int i = 0;
		int j = 0;
		int k = 0;
		bool add_it = true;
		void reset()
		{
			add_it = true;
			ans = "";
			i = 0;
			j = 0;
			k = 0;
		}
	} itsc;
	AGraphNode* bucket_min();
	std::vector<std::deque<AGraphNode*>> buckets;
	std::vector<AGrid*> bucket3d;
	AGraphNode* groot;
	bool skip = false;
	int cur_bfs = 0;
	void berase(AGraphNode* to_delete, int bucket_index);
	AGraphNode* cur;
	struct bford
	{
		int i = 0;
		int j = 1;
		void reset()
		{
			i = 0;
			j = 0;
		}
	} bfc;
	struct qsort_m
	{
		int i = 0;
		int j = 0;
		bool l = false;
		bool r = false;
		void reset()
		{
			i = 0;
			j = 0;
			l = false;
			r = false;
		}
	} qc;
	struct qsort_p
	{
		int i = -1;
		int j = 0;
		void reset()
		{
			i = -1;
			j = 0;
		}
	} qm;
	//UPROPERTY(VisibleAnywhere)
		//bool NegativeCycle = false;
	//UPROPERTY(VisibleAnywhere)
		//int MaxIt = 0;
	std::vector<std::vector<AGraphNode*>> SCCs;
	//std::set <std::pair<int64, AGraphNode*>> pq; 
	std::unordered_set<AGraphEdge*> inPath;
	std::vector<int64> bfo;
	std::vector<int64> bfknown;
	std::vector<EdgeStorage> prev;
	AGraphNode* curnode = nullptr;
	int DCcounter = 0;

	void print_buckets();
	void update_buckets(int index);

};
