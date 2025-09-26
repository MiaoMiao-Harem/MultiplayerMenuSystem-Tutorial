// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "LobbyGameMode.generated.h"


UCLASS()
class MENUSYSTEMDEMO_API ALobbyGameMode : public AGameModeBase{
	GENERATED_BODY()

public:
	/* Override original functions */
	virtual void PostLogin(APlayerController *NewPlayer) override;
	virtual void Logout(AController *Exiting) override;
};
