// Fill out your copyright notice in the Description page of Project Settings.


#include "Word_3D.h"

// Sets default values
AWord_3D::AWord_3D()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	Text = CreateDefaultSubobject<UText3DComponent>(TEXT("Text"));
	Text->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWord_3D::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWord_3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

