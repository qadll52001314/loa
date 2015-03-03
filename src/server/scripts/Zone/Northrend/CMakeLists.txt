# LegacyV3

file(GLOB sources_Northrend Zone/Northrend/*.cpp Zone/Northrend/*.h)
source_group("Zone\\Northrend" FILES ${sources_Northrend})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_Northrend}
)
