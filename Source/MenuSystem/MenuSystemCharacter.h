// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Interfaces/OnlineSessionInterface.h"

// Header files with '.generated' should be put in the end
#include "MenuSystemCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMenuSystemCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	AMenuSystemCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	// Smart pointer to hold the online session interface
	IOnlineSessionPtr OnlineSessionInterface; // TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> OnlineSessionInterface; // Using a TSharedPtr to class IOnlineSession with ThreadSafe option

private:
	// Delegate to bind callback function of session creation
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	// TSharedPtr for callback function of session search to access
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	// Delegate to bind callback function of session search
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	// Delegate to bind callback function of session joint
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;

protected:
	// Blueprint callable function to create game session
	UFUNCTION(BlueprintCallable)
	void CreateGameSession();

	// Callback function which will be called in response to successfully create a game session
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful); // It's bind to the CreateSessionCompleteDelegate so input&return type have to be correct

	// Blueprint callable function to find game sessions
	UFUNCTION(BlueprintCallable)
	void FindGameSessions();

	// Callback function which will be called in response to successfully find game sessions
	void OnFindSessionsComplete(bool bWasSuccessful); // It's bind to the FindSessionsCompleteDelegate so input&return type have to be correct

	// Blueprint callable function to join game session
	UFUNCTION(BlueprintCallable)
	void JoinGameSession(FOnlineSessionSearchResult Result);

	// Callback function which will be called in response to successfully join a game session
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result); // It's bind to the JoinSessionCompleteDelegate so input&return type have to be correct
};