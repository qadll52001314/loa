# LegacyV3

message(" - - - EyeOfEternity")
file(GLOB_RECURSE sources_EyeOfEternity Raid/Northrend/EyeOfEternity/*.cpp Raid/Northrend/EyeOfEternity/*.h)
source_group("Raid\\EyeOfEternity" FILES ${sources_EyeOfEternity})

message(" - - - IcecrownCitadel")
file(GLOB_RECURSE sources_IcecrownCitadel Raid/Northrend/IcecrownCitadel/*.cpp Raid/Northrend/IcecrownCitadel/*.h)
source_group("Raid\\IcecrownCitadel" FILES ${sources_IcecrownCitadel})

message(" - - - Naxxramas")
file(GLOB_RECURSE sources_Naxxramas Raid/Northrend/Naxxramas/*.cpp Raid/Northrend/Naxxramas/*.h)
source_group("Raid\\Naxxramas" FILES ${sources_Naxxramas})

message(" - - - ObsidianSanctum")
file(GLOB_RECURSE sources_ObsidianSanctum Raid/Northrend/ObsidianSanctum/*.cpp Raid/Northrend/ObsidianSanctum/*.h)
source_group("Raid\\ObsidianSanctum" FILES ${sources_ObsidianSanctum})

message(" - - - TrialOfTheChampion")
file(GLOB_RECURSE sources_TrialOfTheChampion Raid/Northrend/TrialOfTheChampion/*.cpp Raid/Northrend/TrialOfTheChampion/*.h)
source_group("Raid\\TrialOfTheChampion" FILES ${sources_TrialOfTheChampion})

message(" - - - TrialOfTheCrusader")
file(GLOB_RECURSE sources_TrialOfTheCrusader Raid/Northrend/TrialOfTheCrusader/*.cpp Raid/Northrend/TrialOfTheCrusader/*.h)
source_group("Raid\\TrialOfTheCrusader" FILES ${sources_TrialOfTheCrusader})

message(" - - - Ulduar")
file(GLOB_RECURSE sources_Ulduar Raid/Northrend/Ulduar/*.cpp Raid/Northrend/Ulduar/*.h)
source_group("Raid\\Ulduar" FILES ${sources_Ulduar})

message(" - - - VaultOfArchavon")
file(GLOB_RECURSE sources_VaultOfArchavon Raid/Northrend/VaultOfArchavon/*.cpp Raid/Northrend/VaultOfArchavon/*.h)
source_group("Raid\\VaultOfArchavon" FILES ${sources_VaultOfArchavon})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_EyeOfEternity}
  ${sources_IcecrownCitadel}
  ${sources_Naxxramas}
  ${sources_ObsidianSanctum}
  ${sources_TrialOfTheChampion}
  ${sources_TrialOfTheCrusader}
  ${sources_Ulduar}
  ${sources_VaultOfArchavon}
)
