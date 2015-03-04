# Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

message(" - Accounts")
file(GLOB_RECURSE sources_Accounts Accounts/*.cpp Accounts/*.h)
source_group("Accounts" FILES ${sources_Accounts})
message(" - Achievements")
file(GLOB_RECURSE sources_Achievements Achievements/*.cpp Achievements/*.h)
source_group("Achievements" FILES ${sources_Achievements})
message(" - Addons")
file(GLOB_RECURSE sources_Addons Addons/*.cpp Addons/*.h)
source_group("Addons" FILES ${sources_Addons})
message(" - AI")
file(GLOB_RECURSE sources_AI AI/*.cpp AI/*.h)
source_group("AI" FILES ${sources_AI})
message(" - Auction House")
file(GLOB_RECURSE sources_AuctionHouse AuctionHouse/*.cpp AuctionHouse/*.h)
source_group("AuctionHouse" FILES ${sources_AuctionHouse})
message(" - Auction House Bot")
file(GLOB_RECURSE sources_AuctionHouseBot AuctionHouseBot/*.cpp AuctionHouseBot/*.h)
source_group("AuctionHouseBot" FILES ${sources_AuctionHouseBot})
message(" - Battlefield")
file(GLOB_RECURSE sources_Battlefield Battlefield/*.cpp Battlefield/*.h)
source_group("Battlefield" FILES ${sources_Battlefield})
message(" - Battlegrounds")
file(GLOB_RECURSE sources_Battlegrounds Battlegrounds/*.cpp Battlegrounds/*.h)
source_group("Battlegrounds" FILES ${sources_Battlegrounds})
message(" - Calendar")
file(GLOB_RECURSE sources_Calendar Calendar/*.cpp Calendar/*.h)
source_group("Calendar" FILES ${sources_Calendar})
message(" - CapitalCity")
file(GLOB_RECURSE sources_CapitalCity CapitalCity/*.cpp CapitalCity/*.h)
source_group("Capital City" FILES ${sources_CapitalCity})
message(" - Chat")
file(GLOB_RECURSE sources_Chat Chat/*.cpp Chat/*.h)
source_group("Chat" FILES ${sources_Chat})
message(" - Combat")
file(GLOB_RECURSE sources_Combat Combat/*.cpp Combat/*.h)
source_group("Combat" FILES ${sources_Combat})
message(" - Conditions")
file(GLOB_RECURSE sources_Conditions Conditions/*.cpp Conditions/*.h)
source_group("Conditions" FILES ${sources_Conditions})
message(" - Data Stores")
file(GLOB_RECURSE sources_DataStores DataStores/*.cpp DataStores/*.h)
source_group("DataStores" FILES ${sources_DataStores})
message(" - Dungeon Finding")
file(GLOB_RECURSE sources_DungeonFinding DungeonFinding/*.cpp DungeonFinding/*.h)
source_group("DungeonFinding" FILES ${sources_DungeonFinding})
message(" - Eluna Methods")
file(GLOB_RECURSE sources_ElunaMethods Eluna/*Methods.h)
source_group("Eluna\\Methods" FILES ${sources_ElunaMethods})
message(" - Eluna Hooks")
file(GLOB_RECURSE sources_ElunaHooks Eluna/*Hooks.cpp)
source_group("Eluna\\Hooks" FILES ${sources_ElunaHooks})
message(" - Eluna Core")
file(GLOB sources_ElunaCore Eluna/*.cpp Eluna/*.h)
source_group("Eluna\\Core" FILES ${sources_ElunaCore})
message(" - Entities")
file(GLOB_RECURSE sources_Entities Entities/*.cpp Entities/*.h)
source_group("Entities" FILES ${sources_Entities})
message(" - Events")
file(GLOB_RECURSE sources_Events Events/*.cpp Events/*.h)
source_group("Events" FILES ${sources_Events})
message(" - Globals")
file(GLOB_RECURSE sources_Globals Globals/*.cpp Globals/*.h)
source_group("Globals" FILES ${sources_Globals})
message(" - Grids")
file(GLOB_RECURSE sources_Grids Grids/*.cpp Grids/*.h)
source_group("Grids" FILES ${sources_Grids})
message(" - Groups")
file(GLOB_RECURSE sources_Groups Groups/*.cpp Groups/*.h)
source_group("Groups" FILES ${sources_Groups})
message(" - Guilds")
file(GLOB_RECURSE sources_Guilds Guilds/*.cpp Guilds/*.h)
source_group("Guilds" FILES ${sources_Guilds})
message(" - Handlers")
file(GLOB_RECURSE sources_Handlers Handlers/*.cpp Handlers/*.h)
source_group("Handlers" FILES ${sources_Handlers})
message(" - Instances")
file(GLOB_RECURSE sources_Instances Instances/*.cpp Instances/*.h)
source_group("Instances" FILES ${sources_Instances})
message(" - Loot")
file(GLOB_RECURSE sources_Loot Loot/*.cpp Loot/*.h)
source_group("Loot" FILES ${sources_Loot})
message(" - Mails")
file(GLOB_RECURSE sources_Mails Mails/*.cpp Mails/*.h)
source_group("Mails" FILES ${sources_Mails})
message(" - Maps")
file(GLOB_RECURSE sources_Maps Maps/*.cpp Maps/*.h)
source_group("Maps" FILES ${sources_Maps})
message(" - Miscellaneous")
file(GLOB_RECURSE sources_Miscellaneous Miscellaneous/*.cpp Miscellaneous/*.h)
source_group("Miscellaneous" FILES ${sources_Miscellaneous})
message(" - Movement")
file(GLOB_RECURSE sources_Movement Movement/*.cpp Movement/*.h)
source_group("Movement" FILES ${sources_Movement})
message(" - Outdoor PvP")
file(GLOB_RECURSE sources_OutdoorPvP OutdoorPvP/*.cpp OutdoorPvP/*.h)
source_group("OutdoorPvP" FILES ${sources_OutdoorPvP})
message(" - Pools")
file(GLOB_RECURSE sources_Pools Pools/*.cpp Pools/*.h)
source_group("Pools" FILES ${sources_Pools})
message(" - Quests")
file(GLOB_RECURSE sources_Quests Quests/*.cpp Quests/*.h)
source_group("Quests" FILES ${sources_Quests})
message(" - Reputation")
file(GLOB_RECURSE sources_Reputation Reputation/*.cpp Reputation/*.h)
source_group("Reputation" FILES ${sources_Reputation})
message(" - Scripting")
file(GLOB_RECURSE sources_Scripting Scripting/*.cpp Scripting/*.h)
source_group("Scripting" FILES ${sources_Scripting})
message(" - Resource Point")
source_group("ResourcePoint" FILES ${sources_ResourcePoint})
file(GLOB_RECURSE sources_ResourcePoint ResourcePoint/*.cpp ResourcePoint/*.h)
message(" - Server")
file(GLOB_RECURSE sources_Server Server/*.cpp Server/*.h)
source_group("Server" FILES ${sources_Server})
message(" - Skills")
file(GLOB_RECURSE sources_Skills Skills/*.cpp Skills/*.h)
source_group("Skills" FILES ${sources_Skills})
message(" - Spells")
file(GLOB_RECURSE sources_Spells Spells/*.cpp Spells/*.h)
source_group("Spells" FILES ${sources_Spells})
message(" - Supremacy")
file(GLOB_RECURSE sources_Supremacy Supremacy/*.cpp Supremacy/*.h)
source_group("Supremacy" FILES ${sources_Supremacy})
message(" - Texts")
file(GLOB_RECURSE sources_Texts Texts/*.cpp Texts/*.h)
source_group("Texts" FILES ${sources_Texts})
message(" - Tools")
file(GLOB_RECURSE sources_Tools Tools/*.cpp Tools/*.h)
source_group("Tools" FILES ${sources_Tools})
message(" - Tickets")
file(GLOB_RECURSE sources_Tickets Tickets/*.cpp Tickets/*.h)
source_group("Tickets" FILES ${sources_Tickets})
message(" - Transmogrification")
file(GLOB_RECURSE sources_Transmogrification Transmogrification/*.cpp Transmogrification/*.h)
source_group("Transmogrification" FILES ${sources_Transmogrification})
message(" - Warden")
file(GLOB_RECURSE sources_Warden Warden/*.cpp Warden/*.h)
source_group("Warden" FILES ${sources_Warden})
message(" - War School")
file(GLOB_RECURSE sources_WarSchool WarSchool/*.cpp WarSchool/*h)
source_group("WarSchool" FILES ${sources_WarSchool})
message(" - Weather")
file(GLOB_RECURSE sources_Weather Weather/*.cpp Weather/*.h)
source_group("Weather" FILES ${sources_Weather})
message(" - World")
file(GLOB_RECURSE sources_World World/*.cpp World/*.h)
source_group("World" FILES ${sources_World})

# Create game-libary

if (USE_COREPCH)
  set(game_STAT_PCH_HDR PrecompiledHeaders/gamePCH.h)
  set(game_STAT_PCH_SRC PrecompiledHeaders/gamePCH.cpp)
endif ()

set(game_STAT_SRCS
  ${game_STAT_SRCS}
  ${sources_Accounts}
  ${sources_Achievements}
  ${sources_Addons}
  ${sources_AI}
  ${sources_AuctionHouse}
  ${sources_AuctionHouseBot}
  ${sources_Battlefield}
  ${sources_Battlegrounds}
  ${sources_Calendar}
  ${sources_CapitalCity}
  ${sources_Chat}
  ${sources_Combat}
  ${sources_Conditions}
  ${sources_DataStores}
  ${sources_DungeonFinding}
  ${sources_ElunaCore}
  ${sources_ElunaHooks}
  ${sources_ElunaMethods}
  ${sources_Entities}
  ${sources_Events}
  ${sources_Globals}
  ${sources_Grids}
  ${sources_Groups}
  ${sources_Guilds}
  ${sources_Handlers}
  ${sources_Instances}
  ${sources_Loot}
  ${sources_Mails}
  ${sources_Maps}
  ${sources_Miscellaneous}
  ${sources_Movement}
  ${sources_OutdoorPvP}
  ${sources_Pools}
  ${sources_Quests}
  ${sources_Reputation}
  ${sources_ResourcePoint}
  ${sources_Scripting}
  ${sources_Server}
  ${sources_Skills}
  ${sources_Spells}
  ${sources_Supremacy}
  ${sources_Texts}
  ${sources_Tools}
  ${sources_Tickets}
  ${sources_Transmogrification}
  ${sources_Warden}
  ${sources_WarSchool}
  ${sources_Weather}
  ${sources_World}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour/Include
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast/Include
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_SOURCE_DIR}/dep/SFMT
  ${CMAKE_SOURCE_DIR}/dep/zlib
  ${CMAKE_SOURCE_DIR}/dep/lua
  ${CMAKE_SOURCE_DIR}/src/server/collision
  ${CMAKE_SOURCE_DIR}/src/server/collision/Management
  ${CMAKE_SOURCE_DIR}/src/server/collision/Models
  ${CMAKE_SOURCE_DIR}/src/server/collision/Maps
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Configuration
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cryptography
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cryptography/Authentication
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database
  ${CMAKE_SOURCE_DIR}/src/server/shared/DataStores
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Dynamic/LinkedReference
  ${CMAKE_SOURCE_DIR}/src/server/shared/Dynamic
  ${CMAKE_SOURCE_DIR}/src/server/shared/Logging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Networking
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Accounts
  ${CMAKE_CURRENT_SOURCE_DIR}/Achievements
  ${CMAKE_CURRENT_SOURCE_DIR}/Addons
  ${CMAKE_CURRENT_SOURCE_DIR}/AI
  ${CMAKE_CURRENT_SOURCE_DIR}/AI/CoreAI
  ${CMAKE_CURRENT_SOURCE_DIR}/AI/ScriptedAI
  ${CMAKE_CURRENT_SOURCE_DIR}/AI/SmartScripts
  ${CMAKE_CURRENT_SOURCE_DIR}/AuctionHouse
  ${CMAKE_CURRENT_SOURCE_DIR}/AuctionHouseBot
  ${CMAKE_CURRENT_SOURCE_DIR}/Battlefield
  ${CMAKE_CURRENT_SOURCE_DIR}/Battlefield/Zones
  ${CMAKE_CURRENT_SOURCE_DIR}/Battlegrounds
  ${CMAKE_CURRENT_SOURCE_DIR}/Battlegrounds/Zones
  ${CMAKE_CURRENT_SOURCE_DIR}/Calendar
  ${CMAKE_CURRENT_SOURCE_DIR}/CapitalCity
  ${CMAKE_CURRENT_SOURCE_DIR}/Chat
  ${CMAKE_CURRENT_SOURCE_DIR}/Chat/Channels
  ${CMAKE_CURRENT_SOURCE_DIR}/Combat
  ${CMAKE_CURRENT_SOURCE_DIR}/Conditions
  ${CMAKE_CURRENT_SOURCE_DIR}/DataStores
  ${CMAKE_CURRENT_SOURCE_DIR}/DungeonFinding
  ${CMAKE_CURRENT_SOURCE_DIR}/Eluna
  ${CMAKE_CURRENT_SOURCE_DIR}/Entities
  ${CMAKE_CURRENT_SOURCE_DIR}/Entities/Creature
  ${CMAKE_CURRENT_SOURCE_DIR}/Entities/Corpse
  ${CMAKE_CURRENT_SOURCE_DIR}/Entities/DynamicObject
  ${CMAKE_CURRENT_SOURCE_DIR}/Entities/GameObject
  ${CMAKE_CURRENT_SOURCE_DIR}/Entities/Item
  ${CMAKE_CURRENT_SOURCE_DIR}/Entities/Item/Container
  ${CMAKE_CURRENT_SOURCE_DIR}/Entities/Object
  ${CMAKE_CURRENT_SOURCE_DIR}/Entities/Object/Updates
  ${CMAKE_CURRENT_SOURCE_DIR}/Entities/Pet
  ${CMAKE_CURRENT_SOURCE_DIR}/Entities/Player
  ${CMAKE_CURRENT_SOURCE_DIR}/Entities/Totem
  ${CMAKE_CURRENT_SOURCE_DIR}/Entities/Unit
  ${CMAKE_CURRENT_SOURCE_DIR}/Entities/Vehicle
  ${CMAKE_CURRENT_SOURCE_DIR}/Entities/Transport
  ${CMAKE_CURRENT_SOURCE_DIR}/Events
  ${CMAKE_CURRENT_SOURCE_DIR}/Globals
  ${CMAKE_CURRENT_SOURCE_DIR}/Grids/Cells
  ${CMAKE_CURRENT_SOURCE_DIR}/Grids/Notifiers
  ${CMAKE_CURRENT_SOURCE_DIR}/Grids
  ${CMAKE_CURRENT_SOURCE_DIR}/Groups
  ${CMAKE_CURRENT_SOURCE_DIR}/Guilds
  ${CMAKE_CURRENT_SOURCE_DIR}/Handlers
  ${CMAKE_CURRENT_SOURCE_DIR}/Instances
  ${CMAKE_CURRENT_SOURCE_DIR}/Loot
  ${CMAKE_CURRENT_SOURCE_DIR}/Mails
  ${CMAKE_CURRENT_SOURCE_DIR}/Maps
  ${CMAKE_CURRENT_SOURCE_DIR}/Miscellaneous
  ${CMAKE_CURRENT_SOURCE_DIR}/Movement
  ${CMAKE_CURRENT_SOURCE_DIR}/Movement/Spline
  ${CMAKE_CURRENT_SOURCE_DIR}/Movement/MovementGenerators
  ${CMAKE_CURRENT_SOURCE_DIR}/Movement/Waypoints
  ${CMAKE_CURRENT_SOURCE_DIR}/OutdoorPvP
  ${CMAKE_CURRENT_SOURCE_DIR}/Pools
  ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders
  ${CMAKE_CURRENT_SOURCE_DIR}/Quests
  ${CMAKE_CURRENT_SOURCE_DIR}/Reputation
  ${CMAKE_CURRENT_SOURCE_DIR}/ResourcePoint
  ${CMAKE_CURRENT_SOURCE_DIR}/Scripting
  ${CMAKE_CURRENT_SOURCE_DIR}/Server/Protocol
  ${CMAKE_CURRENT_SOURCE_DIR}/Server
  ${CMAKE_CURRENT_SOURCE_DIR}/Skills
  ${CMAKE_CURRENT_SOURCE_DIR}/Spells
  ${CMAKE_CURRENT_SOURCE_DIR}/Spells/Auras
  ${CMAKE_CURRENT_SOURCE_DIR}/Supremacy
  ${CMAKE_CURRENT_SOURCE_DIR}/Texts
  ${CMAKE_CURRENT_SOURCE_DIR}/Tools
  ${CMAKE_CURRENT_SOURCE_DIR}/Tickets
  ${CMAKE_CURRENT_SOURCE_DIR}/Transmogrification
  ${CMAKE_CURRENT_SOURCE_DIR}/Warden
  ${CMAKE_CURRENT_SOURCE_DIR}/Warden/Modules
  ${CMAKE_CURRENT_SOURCE_DIR}/WarSchool
  ${CMAKE_CURRENT_SOURCE_DIR}/Weather
  ${CMAKE_CURRENT_SOURCE_DIR}/World
  ${CMAKE_SOURCE_DIR}/src/server/scripts/PrecompiledHeaders
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ${VALGRIND_INCLUDE_DIR}
)

add_library(game STATIC
  ${game_STAT_SRCS}
  ${game_STAT_PCH_SRC}
)

add_dependencies(game revision.h)

# Generate precompiled header
if (USE_COREPCH)
  add_cxx_pch(game ${game_STAT_PCH_HDR} ${game_STAT_PCH_SRC})
endif ()
