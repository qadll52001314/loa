# LegacyV3

message(" - - - BattleForMountHyjal")
file(GLOB_RECURSE sources_BattleForMountHyjal Raid/Kalimdor/BattleForMountHyjal/*.cpp Raid/Kalimdor/BattleForMountHyjal/*.h)
source_group("Dungeon\\BattleForMountHyjal" FILES ${sources_BattleForMountHyjal})

message(" - - - OnyxiasLair")
file(GLOB_RECURSE sources_OnyxiasLair Raid/Kalimdor/OnyxiasLair/*.cpp Raid/Kalimdor/OnyxiasLair/*.h)
source_group("Dungeon\\OnyxiasLair" FILES ${sources_OnyxiasLair})

message(" - - - RuinsOfAhnQiraj")
file(GLOB_RECURSE sources_RuinsOfAhnQiraj Raid/Kalimdor/RuinsOfAhnQiraj/*.cpp Raid/Kalimdor/RuinsOfAhnQiraj/*.h)
source_group("Dungeon\\RuinsOfAhnQiraj" FILES ${sources_RuinsOfAhnQiraj})

message(" - - - TempleOfAhnQiraj")
file(GLOB_RECURSE sources_TempleOfAhnQiraj Raid/Kalimdor/TempleOfAhnQiraj/*.cpp Raid/Kalimdor/TempleOfAhnQiraj/*.h)
source_group("Dungeon\\TempleOfAhnQiraj" FILES ${sources_TempleOfAhnQiraj})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_BattleForMountHyjal}
  ${sources_OnyxiasLair}
  ${sources_RuinsOfAhnQiraj}
  ${sources_TempleOfAhnQiraj}
)
