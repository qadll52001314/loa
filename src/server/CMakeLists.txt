# Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# Enforce compileparameters for corebuilds under GCC
# This to stop a few silly crashes that could have been avoided IF people
# weren't doing some -O3 psychooptimizations etc.

if(CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)
  add_definitions(-fno-delete-null-pointer-checks)
endif()

if( SERVERS )
  set(sources_windows_Debugging
    ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging/WheatyExceptionReport.cpp
    ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging/WheatyExceptionReport.h
  )
  message(STATUS "SHARED")
  add_subdirectory(shared)
  message(STATUS "GAME")
  add_subdirectory(game)
  message(STATUS "COLLISION")
  add_subdirectory(collision)
  message(STATUS "AUTHSERVER")
  add_subdirectory(authserver)
  message(STATUS "SCRIPTS")
  add_subdirectory(scripts)
  message(STATUS "WORLDSERVER")
  add_subdirectory(worldserver)
else()
  if( TOOLS )
  message(STATUS "COLLISION")
    add_subdirectory(collision)
	message(STATUS "SHARED")
    add_subdirectory(shared)
  endif()
endif()
