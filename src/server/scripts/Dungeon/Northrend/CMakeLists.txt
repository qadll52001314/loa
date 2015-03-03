# LegacyV3

message(" - - - Ahnkahet")
file(GLOB_RECURSE sources_Ahnkahet Dungeon/Northrend/Ahnkahet/*.cpp Dungeon/Northrend/Ahnkahet/*.h)
source_group("Dungeon\\Ahnkahet" FILES ${sources_Ahnkahet})

message(" - - - AzjolNerub")
file(GLOB_RECURSE sources_AzjolNerub Dungeon/Northrend/AzjolNerub/*.cpp Dungeon/Northrend/AzjolNerub/*.h)
source_group("Dungeon\\AzjolNerub" FILES ${sources_AzjolNerub})

message(" - - - DraktharonKeep")
file(GLOB_RECURSE sources_DraktharonKeep Dungeon/Northrend/DraktharonKeep/*.cpp Dungeon/Northrend/DraktharonKeep/*.h)
source_group("Dungeon\\DraktharonKeep" FILES ${sources_DraktharonKeep})

message(" - - - ForgeOfSouls")
file(GLOB_RECURSE sources_ForgeOfSouls Dungeon/Northrend/ForgeOfSouls/*.cpp Dungeon/Northrend/ForgeOfSouls/*.h)
source_group("Dungeon\\ForgeOfSouls" FILES ${sources_ForgeOfSouls})

message(" - - - Gundrak")
file(GLOB_RECURSE sources_Gundrak Dungeon/Northrend/Gundrak/*.cpp Dungeon/Northrend/Gundrak/*.h)
source_group("Dungeon\\Gundrak" FILES ${sources_Gundrak})

message(" - - - HallsOfLightning")
file(GLOB_RECURSE sources_HallsOfLightning Dungeon/Northrend/HallsOfLightning/*.cpp Dungeon/Northrend/HallsOfLightning/*.h)
source_group("Dungeon\\HallsOfLightning" FILES ${sources_HallsOfLightning})

message(" - - - HallsOfReflection")
file(GLOB_RECURSE sources_HallsOfReflection Dungeon/Northrend/HallsOfReflection/*.cpp Dungeon/Northrend/HallsOfReflection/*.h)
source_group("Dungeon\\HallsOfReflection" FILES ${sources_HallsOfReflection})

message(" - - - HallsOfStone")
file(GLOB_RECURSE sources_HallsOfStone Dungeon/Northrend/HallsOfStone/*.cpp Dungeon/Northrend/HallsOfStone/*.h)
source_group("Dungeon\\HallsOfStone" FILES ${sources_HallsOfStone})

message(" - - - Nexus")
file(GLOB_RECURSE sources_Nexus Dungeon/Northrend/Nexus/*.cpp Dungeon/Northrend/Nexus/*.h)
source_group("Dungeon\\Nexus" FILES ${sources_Nexus})

message(" - - - Oculus")
file(GLOB_RECURSE sources_Oculus Dungeon/Northrend/Oculus/*.cpp Dungeon/Northrend/Oculus/*.h)
source_group("Dungeon\\Oculus" FILES ${sources_Oculus})

message(" - - - PitOfSaron")
file(GLOB_RECURSE sources_PitOfSaron Dungeon/Northrend/PitOfSaron/*.cpp Dungeon/Northrend/PitOfSaron/*.h)
source_group("Dungeon\\PitOfSaron" FILES ${sources_PitOfSaron})

message(" - - - RubySanctum")
file(GLOB_RECURSE sources_RubySanctum Dungeon/Northrend/RubySanctum/*.cpp Dungeon/Northrend/RubySanctum/*.h)
source_group("Dungeon\\RubySanctum" FILES ${sources_RubySanctum})

message(" - - - UtgardeKeep")
file(GLOB_RECURSE sources_UtgardeKeep Dungeon/Northrend/UtgardeKeep/*.cpp Dungeon/Northrend/UtgardeKeep/*.h)
source_group("Dungeon\\UtgardeKeep" FILES ${sources_UtgardeKeep})

message(" - - - UtgardePinnacle")
file(GLOB_RECURSE sources_UtgardePinnacle Dungeon/Northrend/UtgardePinnacle/*.cpp Dungeon/Northrend/UtgardePinnacle/*.h)
source_group("Dungeon\\UtgardePinnacle" FILES ${sources_UtgardePinnacle})

message(" - - - VioletHold")
file(GLOB_RECURSE sources_VioletHold Dungeon/Northrend/VioletHold/*.cpp Dungeon/Northrend/VioletHold/*.h)
source_group("Dungeon\\VioletHold" FILES ${sources_VioletHold})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_Ahnkahet}
  ${sources_AzjolNerub}
  ${sources_DraktharonKeep}
  ${sources_ForgeOfSouls}
  ${sources_Gundrak}
  ${sources_HallsOfLightning}
  ${sources_HallsOfReflection}
  ${sources_HallsOfStone}
  ${sources_Nexus}
  ${sources_Oculus}
  ${sources_PitOfSaron}
  ${sources_RubySanctum}
  ${sources_UtgardeKeep}
  ${sources_UtgardePinnacle}
  ${sources_VioletHold}
)
