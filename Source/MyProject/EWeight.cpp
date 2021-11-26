// Fill out your copyright notice in the Description page of Project Settings.


#include "EWeight.h"
#include "Text3DComponent.h"
// Sets default values
AEWeight::AEWeight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	TextScene = CreateDefaultSubobject<USceneComponent>(TEXT("TextScene"));
	TextScene->SetupAttachment(RootComponent);

	Text = CreateDefaultSubobject<UText3DComponent>(TEXT("Text"));
	Text->SetupAttachment(TextScene);
}

// Called when the game starts or when spawned
void AEWeight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEWeight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

