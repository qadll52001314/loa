# LegacyV3

message(" - - - Blackwing Lair")
file(GLOB_RECURSE sources_BlackwingLair Raid/EasternKingdoms/BlackwingLair/*.cpp Raid/EasternKingdoms/BlackwingLair/*.h)
source_group("Dungeon\\BlackwingLair" FILES ${sources_BlackwingLair})

message(" - - - Karazhan")
file(GLOB_RECURSE sources_Karazhan Raid/EasternKingdoms/Karazhan/*.cpp Raid/EasternKingdoms/Karazhan/*.h)
source_group("Dungeon\\Karazhan" FILES ${sources_Karazhan})

message(" - - - Molten Core")
file(GLOB_RECURSE sources_MoltenCore Raid/EasternKingdoms/MoltenCore/*.cpp Raid/EasternKingdoms/MoltenCore/*.h)
source_group("Dungeon\\MoltenCore" FILES ${sources_MoltenCore})

message(" - - - Sunwell Plateau")
file(GLOB_RECURSE sources_SunwellPlateau Raid/EasternKingdoms/SunwellPlateau/*.cpp Raid/EasternKingdoms/SunwellPlateau/*.h)
source_group("Dungeon\\SunwellPlateau" FILES ${sources_SunwellPlateau})

message(" - - - Zul'Aman")
file(GLOB_RECURSE sources_ZulAman Raid/EasternKingdoms/ZulAman/*.cpp Raid/EasternKingdoms/ZulAman/*.h)
source_group("Dungeon\\ZulAman" FILES ${sources_ZulAman})

message(" - - - Zul'Gurub")
file(GLOB_RECURSE sources_ZulGurub Raid/EasternKingdoms/ZulGurub/*.cpp Raid/EasternKingdoms/ZulGurub/*.h)
source_group("Dungeon\\ZulGurub" FILES ${sources_ZulGurub})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_BlackwingLair}
  ${sources_Karazhan}
  ${sources_MoltenCore}
  ${sources_SunwellPlateau}
  ${sources_ZulAman}
  ${sources_ZulGurub}
)
