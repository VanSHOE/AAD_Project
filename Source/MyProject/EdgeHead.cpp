// Fill out your copyright notice in the Description page of Project Settings.


#include "EdgeHead.h"

// Sets default values
AEdgeHead::AEdgeHead()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEdgeHead::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEdgeHead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

