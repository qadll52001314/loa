# LegacyV3

file(GLOB_RECURSE sources_ScarletEnclave Zone/EasternKingdoms/ScarletEnclave/*.cpp Zone/EasternKingdoms/ScarletEnclave/*.h)
source_group("Zone\\EasternKingdoms\\ScarletEnclave" FILES ${sources_ScarletEnclave})

file(GLOB sources_EasternKingdoms Zone/EasternKingdoms/*.cpp Zone/EasternKingdoms/*.h)
source_group("Zone\\EasternKingdoms" FILES ${sources_EasternKingdoms})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_EasternKingdoms}
  ${sources_ScarletEnclave}
)
