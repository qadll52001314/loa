# LegacyV3

message(" - - - Arcatraz")
file(GLOB_RECURSE sources_Arcatraz Dungeon/Outland/Arcatraz/*.cpp Dungeon/Outland/Arcatraz/*.h)
source_group("Dungeon\\Arcatraz" FILES ${sources_Arcatraz})

message(" - - - Auchenai Crypts")
file(GLOB_RECURSE sources_AuchenaiCrypts Dungeon/Outland/AuchenaiCrypts/*.cpp Dungeon/Outland/AuchenaiCrypts/*.h)
source_group("Dungeon\\AuchenaiCrypts" FILES ${sources_AuchenaiCrypts})

message(" - - - Blood Furnace")
file(GLOB_RECURSE sources_BloodFurnace Dungeon/Outland/BloodFurnace/*.cpp Dungeon/Outland/BloodFurnace/*.h)
source_group("Dungeon\\BloodFurnace" FILES ${sources_BloodFurnace})

message(" - - - Botanica")
file(GLOB_RECURSE sources_Botanica Dungeon/Outland/Botanica/*.cpp Dungeon/Outland/Botanica/*.h)
source_group("Dungeon\\Botanica" FILES ${sources_Botanica})

message(" - - - Hellfire Ramparts")
file(GLOB_RECURSE sources_HellfireRamparts Dungeon/Outland/HellfireRamparts/*.cpp Dungeon/Outland/HellfireRamparts/*.h)
source_group("Dungeon\\HellfireRamparts" FILES ${sources_HellfireRamparts})

message(" - - - Mana Tombs")
file(GLOB_RECURSE sources_ManaTombs Dungeon/Outland/ManaTombs/*.cpp Dungeon/Outland/ManaTombs/*.h)
source_group("Dungeon\\ManaTombs" FILES ${sources_ManaTombs})

message(" - - - Mechanar")
file(GLOB_RECURSE sources_Mechanar Dungeon/Outland/Mechanar/*.cpp Dungeon/Outland/Mechanar/*.h)
source_group("Dungeon\\Mechanar" FILES ${sources_Mechanar})

message(" - - - Sethekk Halls")
file(GLOB_RECURSE sources_SethekkHalls Dungeon/Outland/SethekkHalls/*.cpp Dungeon/Outland/SethekkHalls/*.h)
source_group("Dungeon\\SethekkHalls" FILES ${sources_SethekkHalls})

message(" - - - Shadow Labyrinth")
file(GLOB_RECURSE sources_ShadowLabyrinth Dungeon/Outland/ShadowLabyrinth/*.cpp Dungeon/Outland/ShadowLabyrinth/*.h)
source_group("Dungeon\\ShadowLabyrinth" FILES ${sources_ShadowLabyrinth})

message(" - - - Shattered Halls")
file(GLOB_RECURSE sources_ShatteredHalls Dungeon/Outland/ShatteredHalls/*.cpp Dungeon/Outland/ShatteredHalls/*.h)
source_group("Dungeon\\ShatteredHalls" FILES ${sources_ShatteredHalls})

message(" - - - Steam Vault")
file(GLOB_RECURSE sources_SteamVault Dungeon/Outland/SteamVault/*.cpp Dungeon/Outland/SteamVault/*.h)
source_group("Dungeon\\SteamVault" FILES ${sources_SteamVault})

message(" - - - The Slave Pens")
file(GLOB_RECURSE sources_TheSlavePens Dungeon/Outland/TheSlavePens/*.cpp Dungeon/Outland/TheSlavePens/*.h)
source_group("Dungeon\\TheSlavePens" FILES ${sources_TheSlavePens})

message(" - - - The Underbog")
file(GLOB_RECURSE sources_TheUnderbog Dungeon/Outland/TheUnderbog/*.cpp Dungeon/Outland/TheUnderbog/*.h)
source_group("Dungeon\\TheUnderbog" FILES ${sources_TheUnderbog})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_Arcatraz}
  ${sources_AuchenaiCrypts}
  ${sources_BloodFurnace}
  ${sources_Botanica}
  ${sources_HellfireRamparts}
  ${sources_ManaTombs}
  ${sources_Mechanar}
  ${sources_SethekkHalls}
  ${sources_ShadowLabyrinth}
  ${sources_ShatteredHalls}
  ${sources_SteamVault}
  ${sources_TheSlavePens}
  ${sources_TheUnderbog}
)
