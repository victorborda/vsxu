cmake_minimum_required(VERSION 2.8)

PROJECT(VSXu)

set(VSXU_VERSION_MAJOR "0")
set(VSXU_VERSION_MINOR "4")
set(VSXU_VERSION_PATCH "0")
set(VSXU_VERSION ${VSXU_VERSION_MAJOR}.${VSXU_VERSION_MINOR}.${VSXU_VERSION_PATCH})

message("---------------------------------------------------------------------")
message("                     Configuring Vovoid VSXu                         ")
message("=====================================================================")
################################################################################
# CMAKE MODULE PATH ############################################################
################################################################################
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)


################################################################################
# GLOBAL INSTALL VARIABLES #####################################################
################################################################################

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(VSXU_INSTALL_INCLUDE_DIR "include")
    set(VSXU_INSTALL_LIB_DIR ".")
    set(VSXU_INSTALL_BIN_DIR ".")
    set(VSXU_INSTALL_SHARE_DIR ".")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(VSXU_INSTALL_INCLUDE_DIR "include/vsxu")
    set(VSXU_INSTALL_LIB_DIR "lib${LIB_SUFFIX}")
    set(VSXU_INSTALL_BIN_DIR "bin")
    set(VSXU_INSTALL_SHARE_DIR "share/vsxu")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(VSXU_INSTALL_INCLUDE_DIR "include")
    set(VSXU_INSTALL_LIB_DIR ".")
    set(VSXU_INSTALL_BIN_DIR ".")
    set(VSXU_INSTALL_SHARE_DIR "share")
endif()
######################################################################################
# Install the Global Headers so as to install VSXu as a library
if (UNIX)
  file(GLOB HEADERS "*.h")
  install (FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/${VSXU_INSTALL_INCLUDE_DIR} COMPONENT Devel)
endif()

################################################################################
# The pkg-config file
################################################################################
IF (UNIX)
  CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/libvsxu.pc.in
                  ${CMAKE_CURRENT_BINARY_DIR}/libvsxu.pc
                  @ONLY)
  INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/libvsxu.pc
           DESTINATION ${VSXU_INSTALL_LIB_DIR}/pkgconfig)
ENDIF (UNIX)


################################################################################
# CPACK OPTIONS ################################################################
################################################################################
SET(CPACK_PACKAGE_NAME "vsxu")
SET(CPACK_DEBIAN_PACKAGE_NAME "vsxu")
SET(CPACK_PACKAGE_VERSION ${VSXU_VERSION})
SET(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/COPYING)

if (UNIX)
  SET(CPACK_PACKAGING_INSTALL_PREFIX "/opt/vovoid")
  SET(CPACK_SET_DESTDIR "on")
  SET(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
endif()

SET(ARCHITECTURE i386)
IF (CMAKE_SIZEOF_VOID_P EQUAL 8)
  SET(ARCHITECTURE amd64)
  set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
ENDIF()

SET(VERSION ${CPACK_DEBIAN_PACKAGE_VERSION})
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_SECTION "graphics")
SET(CPACK_DEBIAN_ARCHITECTURE ${ARCHITECTURE})
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libxrandr2 (>= 2:1.3.0-3), libc6 (>= 2.1.3), libgcc1 (>= 1:4.1.1), libglfw2 (>= 2.6), libpng12-0 (>= 1.2.46-3), libjpeg8 (>= 6b1-1), libglew1.6 (>=1.6.0)")

SET(CPACK_PACKAGE_DESCRIPTION "VSXu Music Visualizer")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "VSXu Music Visualizer, Realtime Graphics Visual Programming Language, ")
SET(CPACK_PACKAGE_CONTACT "Jonatan Wallmander <jonatan@vovoid.com> ")
SET(CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}_${VSXU_VERSION}_${CPACK_DEBIAN_ARCHITECTURE})

SET(CPACK_COMPONENTS_ALL engine artiste player server)
SET(CPACK_COMPONENT_ENGINE_DISPLAY_NAME "VSXu Engine")
SET(CPACK_COMPONENT_ENGINE_REQUIRED 1)
SET(CPACK_COMPONENT_ARTISTE_DISPLAY_NAME "VSXu Artiste")
SET(CPACK_COMPONENT_PLAYER_DISPLAY_NAME "VSXu Player")
SET(CPACK_COMPONENT_PLAYER_REQUIRED 1)
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jonatan 'jaw' Wallmander / Vovoid Media Technologies")

SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "VSXu ${CPACK_PACKAGE_VERSION}")
set(CPACK_PACKAGE_EXECUTABLES "vsxu_player;VSXu Player Window" "vsxu_artiste;VSXu Artiste" "vsxu_demo_player;Demo - Vovoid - Luna - Reactivation")
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
    CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\VSXu Player Fullscreen.lnk\\\" \\\"$INSTDIR\\\\.\\\\vsxu_player.exe \\\" -f
")
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
    Delete \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\VSXu Player Fullscreen.lnk\\\"
")

set(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")
set(CPACK_PACKAGE_NAME VSXu ${CPACK_PACKAGE_VERSION})
set(CPACK_PACKAGE_VENDOR "Vovoid Media Technologies")
set(CPACK_PACKAGE_VERSION_MAJOR ${VSXU_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VSXU_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VSXU_VERSION_PATCH})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Vovoid VSXu ${CPACK_PACKAGE_VERSION}")
set(CPACK_STRIP_FILES 1)

INCLUDE(CPack)


################################################################################
# DEFINES ######################################################################
################################################################################
if(VSXU_DEBUG)
  message("All warnings are enabled")
  add_definitions(-g -Wall -Wtype-limits -fpermissive -Wuninitialized -Wignored-qualifiers -Wempty-body -Wunused-parameter)
else(VSXU_DEBUG)
  if (OPTIMIZATION_FLAGS EQUAL 1)
    add_definitions(-lrt -lm -O1 -march=opteron -fno-thread-jumps -fno-if-conversion2 -fno-delayed-branch -fno-loop-optimize -ftree-dce -ftree-sra -ftree-copyrename -ftree-fre -ftree-ch -fmerge-constants -fcrossjumping -fcse-follow-jumps -fcse-skip-blocks -fexpensive-optimizations -fstrength-reduce -frerun-cse-after-loop -frerun-loop-opt -fforce-addr -fpeephole2 -fschedule-insns2 -fregmove -freorder-blocks -fsched-interblock -funit-at-a-time -falign-functions -falign-jumps -falign-loops -falign-labels -finline-functions -fno-inline -ftracer -fmodulo-sched -fgcse-sm -freschedule-modulo-scheduled-loops -ftree-loop-im -ftree-loop-ivcanon -maccumulate-outgoing-args -mno-align-stringops -D__NO_MATH_INLINES -funsafe-math-optimizations -fno-signaling-nans)
  else (OPTIMIZATION_FLAGS EQUAL 1)
    add_definitions(-Os -c -fmessage-length=0 -fpermissive -finline-functions)
  endif (OPTIMIZATION_FLAGS EQUAL 1)
endif(VSXU_DEBUG)

add_definitions(-DVSXU_INSTALL_LIB_DIR="${VSXU_INSTALL_LIB_DIR}")

#add_definitions(-Os -c -fmessage-length=0 -finline-functions)
#add_definitions(-O3 -c -fmessage-length=0 -finline-functions)

################################################################################
# VSXU LIBRARIES ###############################################################
################################################################################
add_subdirectory(engine)
add_subdirectory(engine_audiovisual)
add_subdirectory(engine_graphics)
add_subdirectory(plugins)

################################################################################
# VSXU IMPLEMENTATIONS/BINARIES ################################################
################################################################################
if (NOT VSXU_OPENGL_ES_2_0 EQUAL 1)
  add_subdirectory(artiste)
  add_subdirectory(player)
  add_subdirectory(player_demo)
endif (NOT VSXU_OPENGL_ES_2_0 EQUAL 1)

if(UNIX)
  add_subdirectory(server)
endif(UNIX)

if (NOT VSXU_ENGINE_STATIC EQUAL 1)
  add_subdirectory(tools/vsxz)
endif (NOT VSXU_ENGINE_STATIC EQUAL 1)




# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

