<div align = "center">

# Unreal Multiplayer MenuSystem

A showcase & tutorial of [Unreal Engine 5 multiplayer menu system plugin](https://github.com/Spr-Aachen/Unreal-MultiplayerMenuSystem-Plugin.git)

![Cover](/docs/media/Cover.png)

</div>


## Guide

### 1. Setup Project

1. Create a C++ project if you don't have any

2. Open the editor, go to `Edit` -> `Plugins`, search for "Online Subsystem Steam", enable the plugin, then close the editor

3. Navigate to the 'Config' folder, open 'DefaultEngine.ini' and paste the following content:
    ```
    [/Script/Engine.GameEngine]
    +NetDriverDefinitions=(DefName="GameNetDriver",DriverClassName="OnlineSubsystemSteam.SteamNetDriver",DriverClassNameFallback="OnlineSubsystemUtils.IpNetDriver")

    [OnlineSubsystem]
    DefaultPlatformService=Steam

    [OnlineSubsystemSteam]
    bEnabled=true
    SteamDevAppId=480 ; This is the dev app ID for Steam's example project called Space War

    ; If using Sessions
    ; bInitServerOnClient=true

    [/Script/OnlineSubsystemSteam.SteamNetDriver]
    NetConnectionClassName="OnlineSubsystemSteam.SteamNetConnection"
    ```

4. Open 'DefaultGame.ini' and paste the following content
    ```
    [/Script/Engine.GameSession]
    MaxPlayers = 100 ; Set to the max number of players for game project we would like to have
    ```

### 2. Migrate Plugin

1. Clone the repository
    ```shell
    git clone https://github.com/Spr-Aachen/Unreal-MultiplayerMenuSystem.git
    ```

2. Navigate to the 'Plugins' folder inside directory, copy the 'MultiplayerSessions' folder

3. Navigate to your target project's directory, paste the 'Plugins' folder

### 3. Build Project

1. Launch the .uproject file, if asking you whether to rebuild missing modules, select "Yes"

- Go to `Tools` -> `Generate Visual Studio Code Project`