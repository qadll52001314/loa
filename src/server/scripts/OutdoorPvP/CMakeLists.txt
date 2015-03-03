# Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

message(" - Outdoor PVP")
file(GLOB_RECURSE scripts_OutdoorPVP OutdoorPVP/*.cpp OutdoorPVP/*.h)
source_group("OutdoorPVP" FILES ${scripts_OutdoorPVP})

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${scripts_OutdoorPVP}
)
