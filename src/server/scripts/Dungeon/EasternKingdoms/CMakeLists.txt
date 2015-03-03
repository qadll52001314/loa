# LegacyV3

message(" - - - Blackrock Depths")
file(GLOB_RECURSE sources_BlackrockDepths Dungeon/EasternKingdoms/BlackrockDepths/*.cpp Dungeon/EasternKingdoms/BlackrockDepths/*.h)
source_group("Dungeon\\BlackrockDepths" FILES ${sources_BlackrockDepths})

message(" - - - Blackrock Spire")
file(GLOB_RECURSE sources_BlackrockSpire Dungeon/EasternKingdoms/BlackrockSpire/*.cpp Dungeon/EasternKingdoms/BlackrockSpire/*.h)
source_group("Dungeon\\BlackrockSpire" FILES ${sources_BlackrockSpire})

message(" - - - Deadmines")
file(GLOB_RECURSE sources_Deadmines Dungeon/EasternKingdoms/Deadmines/*.cpp Dungeon/EasternKingdoms/Deadmines/*.h)
source_group("Dungeon\\Deadmines" FILES ${sources_Deadmines})

message(" - - - Gnomeregan")
file(GLOB_RECURSE sources_Gnomeregan Dungeon/EasternKingdoms/Gnomeregan/*.cpp Dungeon/EasternKingdoms/Gnomeregan/*.h)
source_group("Dungeon\\Gnomeregan" FILES ${sources_Gnomeregan})

message(" - - - Magister's Terrace")
file(GLOB_RECURSE sources_MagistersTerrace Dungeon/EasternKingdoms/MagistersTerrace/*.cpp Dungeon/EasternKingdoms/MagistersTerrace/*.h)
source_group("Dungeon\\MagistersTerrace" FILES ${sources_MagistersTerrace})

message(" - - - Scarlet Monastery")
file(GLOB_RECURSE sources_ScarletMonastery Dungeon/EasternKingdoms/ScarletMonastery/*.cpp Dungeon/EasternKingdoms/ScarletMonastery/*.h)
source_group("Dungeon\\ScarletMonastery" FILES ${sources_ScarletMonastery})

message(" - - - Scholomance")
file(GLOB_RECURSE sources_Scholomance Dungeon/EasternKingdoms/Scholomance/*.cpp Dungeon/EasternKingdoms/Scholomance/*.h)
source_group("Dungeon\\Scholomance" FILES ${sources_Scholomance})

message(" - - - Shadowfang Keep")
file(GLOB_RECURSE sources_ShadowfangKeep Dungeon/EasternKingdoms/ShadowfangKeep/*.cpp Dungeon/EasternKingdoms/ShadowfangKeep/*.h)
source_group("Dungeon\\ShadowfangKeep" FILES ${sources_ShadowfangKeep})

message(" - - - Stratholme")
file(GLOB_RECURSE sources_Stratholme Dungeon/EasternKingdoms/Stratholme/*.cpp Dungeon/EasternKingdoms/Stratholme/*.h)
source_group("Dungeon\\Stratholme" FILES ${sources_Stratholme})

message(" - - - Sunken Temple")
file(GLOB_RECURSE sources_SunkenTemple Dungeon/EasternKingdoms/SunkenTemple/*.cpp Dungeon/EasternKingdoms/SunkenTemple/*.h)
source_group("Dungeon\\SunkenTemple" FILES ${sources_SunkenTemple})

message(" - - - The Stockade")
file(GLOB_RECURSE sources_TheStockade Dungeon/EasternKingdoms/TheStockade/*.cpp Dungeon/EasternKingdoms/TheStockade/*.h)
source_group("Dungeon\\TheStockade" FILES ${sources_TheStockade})

message(" - - - Uldaman")
file(GLOB_RECURSE sources_Uldaman Dungeon/EasternKingdoms/Uldaman/*.cpp Dungeon/EasternKingdoms/Uldaman/*.h)
source_group("Dungeon\\Uldaman" FILES ${sources_Uldaman})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_BlackrockDepths}
  ${sources_BlackrockSpire}
  ${sources_BlackrockDepths}
  ${sources_Deadmines}
  ${sources_Gnomeregan}
  ${sources_MagistersTerrace}
  ${sources_ScarletMonastery}
  ${sources_Scholomance}
  ${sources_ShadowfangKeep}
  ${sources_Stratholme}
  ${sources_SunkenTemple}
  ${sources_TheStockade}
  ${sources_Uldaman}
)
