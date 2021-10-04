// Fill out your copyright notice in the Description page of Project Settings.


#include "Changeee.h"
#include "Text3DComponent.h"
#include "GauntletTestController.h"
// Sets default values for this component's properties
UChangeee::UChangeee()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UChangeee::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ha"));
	// ...
	const AActor* parent = GetOwner();


}


// Called every frame
void UChangeee::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction); 
	//UE_LOG(LogTemp, Warning, TEXT("ha"));
	auto b = GetOwner();
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message1!"));
	//UE_LOG(LogTemp, Warning, TEXT("ha2"));
	if (b == nullptr) return;
	//UE_LOG(LogTemp, Warning, TEXT("h3"));

	
	auto a = b->GetComponents();
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *a);
	
	for (auto x : a)
	{
		auto name = x->GetFName().ToString();
		if (name != "Text3DComponent") continue;
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *x->GetFName().ToString());
		UText3DComponent* qq = (UText3DComponent*)x;
		auto temp = qq->Text.ToString();
		FText n, p;
		n = p.FromString(temp + "yoo");
		qq->SetText(n);
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *qq->Text.ToString());
	}
	
	// ...
}

