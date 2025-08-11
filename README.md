<div align = "center">

# Unreal Multiplayer MenuSystem

A multiplayer plugin with menu system for Unreal Engine 5

![Title](/docs/media/Title.png)

</div>


## Guide

### 1. Setup Project

- Create a C++ project if you don't have any

- Open the editor, `Edit` -> `Plugins`, type "Online Subsystem Steam" in the search bar, enable the Online Subsystem Steam plugin, then close the editor

- Enter the 'Config' folder, open 'DefaultEngine.ini' and paste the following content
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

- open 'DefaultGame.ini' and paste the following content
    ```
    [/Script/Engine.GameSession]
    MaxPlayers = 100 ; Set to the max number of players for game project we would like to have
    ```

### 2. Migrate Plugin

- Clone Repository
    ```shell
    git clone https://github.com/Spr-Aachen/Unreal-MultiplayerMenuSystem.git
    ```

- Enter the directory, copy the 'Plugins' folder

- Nevigate to the your target project directory, paste the 'Plugins' folder

### 3. Build Project

- Launch .uproject file, if asking you whether to rebuild missing modules then click "Yes"

- `Tools` -> `Generate Visual Studio Code Project`