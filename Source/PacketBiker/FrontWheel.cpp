#include "FrontWheel.h"

UFrontWheel::UFrontWheel()
{
	WheelRadius = 30.f;
	WheelWidth = 15.f;
	bAffectedBySteering = true;
	bAffectedByEngine = false;
	bAffectedByBrake = true;
	bAffectedByHandbrake = false;
	MaxSteerAngle = 40.f;
	AxleType = EAxleType::Front;
	SpringRate = 250.f;
	SuspensionMaxRaise = 8.f;
	SuspensionMaxDrop = 10.f;
}
