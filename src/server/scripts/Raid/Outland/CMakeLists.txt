# LegacyV3

message(" - - - Black Temple")
file(GLOB_RECURSE sources_BlackTemple Raid/Outland/BlackTemple/*.cpp Raid/Outland/BlackTemple/*.h)
source_group("Raid\\BlackTemple" FILES ${sources_BlackTemple})

message(" - - - Gruul's Lair")
file(GLOB_RECURSE sources_GruulsLair Raid/Outland/GruulsLair/*.cpp Raid/Outland/GruulsLair/*.h)
source_group("Raid\\GruulsLair" FILES ${sources_GruulsLair})

message(" - - - Magtheridon's Lair")
file(GLOB_RECURSE sources_MagtheridonsLair Raid/Outland/MagtheridonsLair/*.cpp Raid/Outland/MagtheridonsLair/*.h)
source_group("Raid\\MagtheridonsLair" FILES ${sources_MagtheridonsLair})

message(" - - - Serpent Shrine")
file(GLOB_RECURSE sources_SerpentShrine Raid/Outland/SerpentShrine/*.cpp Raid/Outland/SerpentShrine/*.h)
source_group("Raid\\SerpentShrine" FILES ${sources_SerpentShrine})

message(" - - - The Eye")
file(GLOB_RECURSE sources_TheEye Raid/Outland/TheEye/*.cpp Raid/Outland/TheEye/*.h)
source_group("Raid\\TheEye" FILES ${sources_TheEye})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_BlackTemple}
  ${sources_GruulsLair}
  ${sources_MagtheridonsLair}
  ${sources_SerpentShrine}
  ${sources_TheEye}
)
