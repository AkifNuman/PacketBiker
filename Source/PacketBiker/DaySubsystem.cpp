#include "DaySubsystem.h"
#include "PBSaveGame.h"
#include "MissionSubsystem.h"

void UDaySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Collection.InitializeDependency<UMissionSubsystem>();
	Save = UPBSaveGame::LoadOrCreate();
	if (UMissionSubsystem* M = GetWorld()->GetSubsystem<UMissionSubsystem>())
	{
		M->OnMissionFinished.AddDynamic(this, &UDaySubsystem::HandleMissionFinished);
	}
}

void UDaySubsystem::StartDay()
{
	Today = FDaySummary();
	Today.Day = Save->Day;
	Elapsed = 0.f;
	OfferTimer = 5.f; // ilk sipariş 5 sn sonra
	bRunning = true;
}

void UDaySubsystem::EndDay()
{
	if (!bRunning) return;
	bRunning = false;
	Save->Day++;
	Save->BestDayEarnings = FMath::Max(Save->BestDayEarnings, Today.Earnings);
	Save->Save();
	OnDayEnded.Broadcast(Today);
}

float UDaySubsystem::NextOfferInterval() const
{
	// GDD: ilk 2 dk 40 sn, orta 4 dk 25 sn, son 2 dk 20 sn
	const float P = GetDayProgress();
	if (P < 0.25f) return 40.f;
	if (P < 0.75f) return 25.f;
	return 20.f;
}

void UDaySubsystem::Tick(float DeltaTime)
{
	if (!bRunning) return;
	Elapsed += DeltaTime;
	if (Elapsed >= DayLengthSeconds) { EndDay(); return; }

	UMissionSubsystem* M = GetWorld()->GetSubsystem<UMissionSubsystem>();
	if (!M) return;
	if (M->HasActiveMission() || M->GetMission().State == EMissionState::Offered) { OfferTimer = NextOfferInterval(); return; }
	OfferTimer -= DeltaTime;
	if (OfferTimer <= 0.f)
	{
		M->OfferMission();
		OfferTimer = NextOfferInterval();
	}
}

void UDaySubsystem::HandleMissionFinished(const FMission& Mission, float Pay)
{
	if (Mission.State != EMissionState::Completed) return;
	const int32 P = FMath::RoundToInt(Pay);
	Today.Deliveries++;
	Today.Earnings += P;
	if (Mission.TimeRemaining >= 0.f) Today.OnTime++;
	Today.Crashes += Mission.Crashes;
	if (Mission.Crashes == 0) Today.BestTip = FMath::Max(Today.BestTip, FMath::RoundToInt(Mission.BasePay * 0.2f));
	Save->TotalDeliveries++;
	Save->TotalCrashes += Mission.Crashes;
	AddMoney(P);
}

void UDaySubsystem::AddMoney(int32 Amount)
{
	Save->Money = FMath::Max(0, Save->Money + Amount);
	OnMoneyChanged.Broadcast(Save->Money);
}

bool UDaySubsystem::SpendMoney(int32 Amount)
{
	if (Amount > Save->Money) return false;
	AddMoney(-Amount);
	Save->Save();
	return true;
}

int32 UDaySubsystem::GetMoney() const { return Save ? Save->Money : 0; }
int32 UDaySubsystem::GetDay() const   { return Save ? Save->Day : 1; }

FText UDaySubsystem::GetClockText() const
{
	const float Hours = 8.f + GetDayProgress() * 16.f; // 08:00 → 24:00
	const int32 H = FMath::Clamp(FMath::FloorToInt(Hours), 8, 24);
	const int32 Min = FMath::FloorToInt(FMath::Frac(Hours) * 60.f);
	return FText::FromString(FString::Printf(TEXT("%02d:%02d"), H, Min));
}
