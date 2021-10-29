// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphEdge.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Text3DComponent.h"
// Sets default values
AGraphEdge::AGraphEdge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	Cylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder"));
	Cylinder->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AGraphEdge::BeginPlay()
{
	Super::BeginPlay();
	mt = Cylinder->GetMaterial(0);
	g = false;
}

// Called every frame
void AGraphEdge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

