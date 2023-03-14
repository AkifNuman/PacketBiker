// Fill out your copyright notice in the Description page of Project Settings.


#include "RestaurantPoint.h"

// Sets default values
ARestaurantPoint::ARestaurantPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ARestaurantPoint::BeginPlay()
{
	Super::BeginPlay();
	m_restaurantloc = FMath::RandRange(0, 5);
	m_targetloc = FMath::RandRange(0, 10);
	
}

// Called every frame
void ARestaurantPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

