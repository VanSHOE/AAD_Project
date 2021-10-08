// Fill out your copyright notice in the Description page of Project Settings.


#include "secondclass.h"

// Sets default values
Asecondclass::Asecondclass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Asecondclass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Asecondclass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

