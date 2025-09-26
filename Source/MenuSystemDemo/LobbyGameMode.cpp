// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"


void ALobbyGameMode::PostLogin(APlayerController *NewPlayer)
{
    // Call the super version
    Super::PostLogin(NewPlayer);

    /* Get player number */
    if (!GameState)
    {
        return;
    }
    int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
    // Print debug message
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            1,
            60.f,
            FColor::Yellow,
            FString::Printf(TEXT("Players in game: %d"), NumberOfPlayers)
        );
    }
    /* Get player name */
    APlayerState *PlayerState = NewPlayer->GetPlayerState<APlayerState>();
    if (!PlayerState)
    {
        return;
    }
    FString PlayerName = PlayerState->GetPlayerName();
    // Print debug message
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            1,
            60.f,
            FColor::Yellow,
            FString::Printf(TEXT("%s has joined the game"), *PlayerName)
        );
    }
}


void ALobbyGameMode::Logout(AController *Exiting)
{
    // Call the super version
    Super::Logout(Exiting);

    /* Get player number */
    if (!GameState)
    {
        return;
    }
    int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
    // Print debug message
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            1,
            60.f,
            FColor::Yellow,
            FString::Printf(TEXT("Players in game: %d"), NumberOfPlayers - 1)
        );
    }
    /* Get player name */
    APlayerState *PlayerState = Exiting->GetPlayerState<APlayerState>();
    if (!PlayerState)
    {
        return;
    }
    FString PlayerName = PlayerState->GetPlayerName();
    // Print debug message
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            1,
            60.f,
            FColor::Yellow,
            FString::Printf(TEXT("%s has exited the game"), *PlayerName)
        );
    }
}