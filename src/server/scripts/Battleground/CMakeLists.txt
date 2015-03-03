# LegacyV3

message(" - Battleground")

message(" - - Alterac Valley")
file(GLOB_RECURSE sources_AlteracValley Battleground/AlteracValley/*.cpp Battleground/AlteracValley/*.h)
source_group("Battleground\\AlteracValley" FILES ${sources_AlteracValley})

message(" - - Isle of Conquest")
file(GLOB_RECURSE sources_IsleOfConquest Battleground/IsleOfConquest/*.cpp Battleground/IsleOfConquest/*.h)
source_group("Battleground\\IsleOfConquest" FILES ${sources_IsleOfConquest})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_AlteracValley}
  ${sources_IsleOfConquest}
)
