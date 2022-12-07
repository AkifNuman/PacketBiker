// Fill out your copyright notice in the Description page of Project Settings.
#include "FrontWheel.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

UFrontWheel::UFrontWheel()
{
	ShapeRadius = 17.5f;
	ShapeWidth = 10.f;
	bAffectedByHandbrake = false;
	SteerAngle = 35.f;
	Offset = FVector(42.0f, 0.f, -34.f);
}

PRAGMA_ENABLE_DEPRECATION_WARNINGS