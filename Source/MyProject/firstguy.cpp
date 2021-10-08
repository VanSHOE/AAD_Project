// Fill out your copyright notice in the Description page of Project Settings.


#include "firstguy.h"
#include "Kismet/GameplayStatics.h"
#include "secondclass.h"
// Sets default values
Afirstguy::Afirstguy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
TArray<AActor*> a;
void Afirstguy::BeginPlay()
{
	Super::BeginPlay();
	a.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Asecondclass::StaticClass(), a);

}

// Called every frame
void Afirstguy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (auto x : a)
	{
		auto q = x->GetActorLocation();
		q.Z += 50;
		x->SetActorLocation(q);
	}
}

