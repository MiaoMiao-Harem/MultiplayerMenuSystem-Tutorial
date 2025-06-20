// Copyright Epic Games, Inc. All Rights Reserved.

#include "MenuSystemCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMenuSystemCharacter

AMenuSystemCharacter::AMenuSystemCharacter() : // Initialize CreateSessionCompleteDelegate (and bind the callback function to it)
	CreateSessionCompleteDelegate(
		FOnCreateSessionCompleteDelegate::CreateUObject(this, &AMenuSystemCharacter::OnCreateSessionComplete) // Construct a new delegate that took in the address of callback function
	),
	FindSessionsCompleteDelegate(
		FOnFindSessionsCompleteDelegate::CreateUObject(this, &AMenuSystemCharacter::OnFindSessionsComplete) // Construct a new delegate that took in the address of callback function
	)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Get session interface
	IOnlineSubsystem *OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

		// Show debug message
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.6f,
				FColor::Blue,
				FString::Printf(TEXT("Found subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString())
			);
		}
	}
}


void AMenuSystemCharacter::CreateGameSession()
{
	/* Check to see if OnlineSessionInterface is valid */
	if (!OnlineSessionInterface.IsValid()) // The way to check if TSharedPtr is valid is by using the 'IsValid' function
	{
		return;
	}
	/* Check and destroy the existing session */
	// Get the existing session pointer
	auto ExistingSession = OnlineSessionInterface->GetNamedSession(
		NAME_GameSession // If we always use this NAME_GameSession, then we'll always be checking to see if a session with this name exists
	);
	// Check to see if the existing session pointer is not null
	if (ExistingSession != nullptr)
	{
		// Destroy the existing session
		OnlineSessionInterface->DestroySession(
			NAME_GameSession // Same as above
		);
	}
	/* Create a new session */
	// Add CreateSessionCompleteDelegate to OnlineSessionInterface's delegate list
	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate); // Once a session is created, then the fallback function which bound to this delegate will be called
	// Create and initialize a TSharedPtr to class FOnlineSessionSettings
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(
		new FOnlineSessionSettings()
	);
	// Configure session settings
	SessionSettings->bIsLANMatch = false; // Set to false if you want to connect over the internet
	SessionSettings->NumPublicConnections = 4; // Determine how many players can connect to the game
	SessionSettings->bAllowJoinInProgress = true; // Allow players to join when session is running
	SessionSettings->bAllowJoinViaPresence = true; // Allow steam to search for sessions going on players' regions
	SessionSettings->bShouldAdvertise = true; // Allow steam to advertise sessions
	SessionSettings->bUsesPresence = true; // Allow players to find sessions going on their regions
	//SessionSettings->bUseLobbiesIfAvailable = true; // Fix sessions finding issue
	// Get the world's first local player
	const ULocalPlayer *LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	// Create a session
	OnlineSessionInterface->CreateSession(
		*LocalPlayer->GetPreferredUniqueNetId(),
		NAME_GameSession,
		*SessionSettings
	);
}


void AMenuSystemCharacter::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		// Show debug message
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Green,
			FString::Printf(TEXT("Created session: %s"), *SessionName.ToString()));
	}
	else
	{
		// Show debug message
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Failed to create session")));
		}
	}
}


void AMenuSystemCharacter::FindGameSessions()
{
	/* Check to see if OnlineSessionInterface is valid */
	if (!OnlineSessionInterface.IsValid()) // The way to check if TSharedPtr is valid is by using the 'IsValid' function
	{
		return;
	}
	/* Find game sessions */
	// Add FindSessionsCompleteDelegate to OnlineSessionInterface's delegate list
	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate); // Once all sessions are found, then the fallback function which bound to this delegate will be called
	// Initialize a TSharedPtr to class FOnlineSessionSearch
	SessionSearch = MakeShareable( // SessionSearch needs to be a member variable, so declaring local variable (TSharedPtr<FOnlineSessionSearch> SessionSearch) is not allowed
		new FOnlineSessionSearch
	);
	// Configure search settings
	SessionSearch->MaxSearchResults = 10000; // Set to a high number if using public steam server like Spacewar (AppID 480)
	SessionSearch->bIsLanQuery = false; // Set to false if you want to connect over the internet
	SessionSearch->QuerySettings.Set( // Make sure any session we find is using presence
		SEARCH_PRESENCE, // Macro
		true,
		EOnlineComparisonOp::Equals
	);
	// Get the world's first local player
	const ULocalPlayer *LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	// Find sessions
	OnlineSessionInterface->FindSessions(
		*LocalPlayer->GetPreferredUniqueNetId(),
		SessionSearch.ToSharedRef()
	);
}


void AMenuSystemCharacter::OnFindSessionsComplete(bool bWasSuccessful)
{
	/* Check to see if OnlineSessionInterface is valid */
	if (!OnlineSessionInterface.IsValid()) // The way to check if TSharedPtr is valid is by using the 'IsValid' function
	{
		return;
	}
	/* Access search results */
	for (auto Result : SessionSearch->SearchResults) // Traverse the sessions search results
	{
		FString ID = Result.GetSessionIdStr();
		FString User = Result.Session.OwningUserName;
		// Show debug message
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				FString::Printf(TEXT("ID: %s, User: %s"), *ID, *User)
			);
		}
	}	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMenuSystemCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMenuSystemCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMenuSystemCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMenuSystemCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMenuSystemCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMenuSystemCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
