#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

/** Yürüyen oyuncu/yaya tabanı. */
UCLASS()
class PACKETBIKER_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	/** Bir motorun etkileşim kutusunun içinde mi (binmeye hazır). */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Vehicle")
	bool InBox = false;
};
