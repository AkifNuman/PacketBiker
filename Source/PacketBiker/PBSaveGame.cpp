#include "PBSaveGame.h"
#include "Kismet/GameplayStatics.h"

const FString UPBSaveGame::SlotName = TEXT("PacketBiker");

UPBSaveGame* UPBSaveGame::LoadOrCreate()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		if (UPBSaveGame* S = Cast<UPBSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0))) return S;
	}
	return Cast<UPBSaveGame>(UGameplayStatics::CreateSaveGameObject(UPBSaveGame::StaticClass()));
}

bool UPBSaveGame::Save()
{
	return UGameplayStatics::SaveGameToSlot(this, SlotName, 0);
}
