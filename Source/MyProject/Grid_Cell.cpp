// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid_Cell.h"
#include "Components/StaticMeshComponent.h"
#include "Text3DComponent.h"
// Sets default values
AGrid_Cell::AGrid_Cell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Floor->SetupAttachment(RootComponent);

	Text = CreateDefaultSubobject<UText3DComponent>(TEXT("Text"));
	Text->SetupAttachment(RootComponent);

	Wall_0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall_0"));
	Wall_0->SetupAttachment(RootComponent);

	Wall_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall_1"));
	Wall_1->SetupAttachment(RootComponent);

	Wall_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall_2"));
	Wall_2->SetupAttachment(RootComponent);

	Wall_3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall_3"));
	Wall_3->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGrid_Cell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrid_Cell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FText n = n.FromString("sup" + Text->Text.ToString());
	//Text->SetText(n);
}

void AGrid_Cell::stext(FString s)
{

}