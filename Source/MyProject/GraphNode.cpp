// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphNode.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Text3DComponent.h"
// Sets default values
AGraphNode::AGraphNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);

	TextScene = CreateDefaultSubobject<USceneComponent>(TEXT("TextScene"));
	TextScene->SetupAttachment(RootComponent);

	Text = CreateDefaultSubobject<UText3DComponent>(TEXT("Text"));
	Text->SetupAttachment(TextScene);
}

// Called when the game starts or when spawned
void AGraphNode::BeginPlay()
{
	Super::BeginPlay();
	ct = 2;
	TArray<AActor*> a;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), a);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *a[0]->GetName());
	player = Cast<APawn>(a[0]);
}

// Called every frame
void AGraphNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto ptr = player->GetActorLocation() - GetActorLocation();
	TextScene->SetWorldRotation(ptr.Rotation());
}

