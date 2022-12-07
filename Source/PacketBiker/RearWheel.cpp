// Fill out your copyright notice in the Description page of Project Settings.
#include "RearWheel.h"

PRAGMA_ENABLE_DEPRECATION_WARNINGS

URearWheel::URearWheel()
{
	ShapeRadius = 17.5f;
	ShapeWidth = 6.5f;
	bAffectedByHandbrake = true;
	SteerAngle = 0.f;
	Offset = FVector(-34.f, 0.f, -34.f);
}
