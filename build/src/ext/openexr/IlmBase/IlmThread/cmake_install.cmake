# Install script for directory: /Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/ext/openexr/IlmBase/IlmThread

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/build/src/ext/openexr/IlmBase/IlmThread/libIlmThread.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmThread.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmThread.a")
    execute_process(COMMAND "/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmThread.a")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES
    "/Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/ext/openexr/IlmBase/IlmThread/IlmThreadPool.h"
    "/Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/ext/openexr/IlmBase/IlmThread/IlmThread.h"
    "/Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/ext/openexr/IlmBase/IlmThread/IlmThreadSemaphore.h"
    "/Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/ext/openexr/IlmBase/IlmThread/IlmThreadMutex.h"
    "/Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/ext/openexr/IlmBase/IlmThread/IlmThreadNamespace.h"
    "/Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/ext/openexr/IlmBase/IlmThread/IlmThreadExport.h"
    "/Users/alicejung/Projects/ip-cal/code_repositories/bfp-pbrt/pbrt-v3/src/ext/openexr/IlmBase/IlmThread/IlmThreadForward.h"
    )
endif()

