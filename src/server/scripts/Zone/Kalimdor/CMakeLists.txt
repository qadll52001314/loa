# LegacyV3

file(GLOB sources_Kalimdor Zone/Kalimdor/*.cpp Zone/Kalimdor/*.h)
source_group("Zone\\Kalimdor" FILES ${sources_Kalimdor})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_Kalimdor}
)
