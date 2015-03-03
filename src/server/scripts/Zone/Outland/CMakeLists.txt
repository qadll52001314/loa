# LegacyV3

file(GLOB sources_Outland Zone/Outland/*.cpp Zone/Outland/*.h)
source_group("Zone\\Outland" FILES ${sources_Outland})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_Outland}
)
