# LegacyV3

file(GLOB sources_EasternKingdoms Zone/EasternKingdoms/*.cpp Zone/EasternKingdoms/*.h)
source_group("Zone\\EasternKingdoms" FILES ${sources_EasternKingdoms})

file(GLOB_RECURSE sources_ScarletEnclave Zone/ScarletEnclave/*.cpp Zone/ScarletEnclave/*.h)
source_group("Zone\\EasternKingdoms\\ScarletEnclave" FILES ${sources_ScarletEnclave})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_EasternKingdoms}
  ${sources_ScarletEnclave}
)
