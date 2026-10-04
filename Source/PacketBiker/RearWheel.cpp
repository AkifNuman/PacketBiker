#include "RearWheel.h"

URearWheel::URearWheel()
{
	WheelRadius = 30.f;
	WheelWidth = 18.f;
	bAffectedBySteering = false;
	bAffectedByEngine = true;
	bAffectedByBrake = true;
	bAffectedByHandbrake = true;
	AxleType = EAxleType::Rear;
	SpringRate = 250.f;
	SuspensionMaxRaise = 8.f;
	SuspensionMaxDrop = 10.f;
}
