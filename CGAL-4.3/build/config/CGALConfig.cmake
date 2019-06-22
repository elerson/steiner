#
# This files contains definitions needed to use CGAL in a program.
# DO NOT EDIT THIS. The definitons have been generated by CMake at configuration time.
# This file is loaded by cmake via the command "find_package(CGAL)"
#
# This file correspond to a CGAL installation with "make install", thus the actual location
# must be given by the cmake variable or enviroment variable CGAL_DIR. 

set(CGAL_CONFIG_LOADED TRUE)

# CGAL_DIR is the directory where this CGALConfig.cmake is installed
set(CGAL_INSTALL_PREFIX "/usr/local")

set(CGAL_MAJOR_VERSION    "4" )
set(CGAL_MINOR_VERSION    "3" )
set(CGAL_BUILD_VERSION    "1000" )
set(CGAL_SCM_BRANCH_NAME  "")
set(CGAL_GIT_SHA1         "")

set(CGAL_BUILD_SHARED_LIBS        "ON" )
set(CGAL_Boost_USE_STATIC_LIBS    "OFF" )

set(CGAL_CXX_FLAGS_INIT                   " -frounding-math" )
set(CGAL_CXX_FLAGS_RELEASE_INIT           "-O3 -DNDEBUG" )
set(CGAL_CXX_FLAGS_DEBUG_INIT             "-g" )
set(CGAL_MODULE_LINKER_FLAGS_INIT         " " )
set(CGAL_MODULE_LINKER_FLAGS_RELEASE_INIT "" )
set(CGAL_MODULE_LINKER_FLAGS_DEBUG_INIT   "" )
set(CGAL_SHARED_LINKER_FLAGS_INIT         " " )
set(CGAL_SHARED_LINKER_FLAGS_RELEASE_INIT "" )
set(CGAL_SHARED_LINKER_FLAGS_DEBUG_INIT   "" )
set(CGAL_BUILD_TYPE_INIT                  "Release" )

set(CGAL_INCLUDE_DIRS  "/usr/local/include" )
set(CGAL_MODULES_DIR   "/usr/local/lib/CGAL" )
set(CGAL_LIBRARIES_DIR "/usr/local/lib" )

set(WITH_CGAL         "ON" )
set(WITH_CGAL_Core    "ON" )
set(WITH_CGAL_ImageIO "ON" )
set(WITH_CGAL_Qt3     "ON" )
set(WITH_CGAL_Qt4     "ON" )

set(CGAL_LIBRARY         "/usr/local/lib/libCGAL.so")
set(CGAL_Core_LIBRARY    "/usr/local/lib/libCGAL_Core.so")
set(CGAL_ImageIO_LIBRARY "/usr/local/lib/libCGAL_ImageIO.so")
set(CGAL_Qt3_LIBRARY     "")
set(CGAL_Qt4_LIBRARY     "/usr/local/lib/libCGAL_Qt4.so")

set(CGAL_3RD_PARTY_INCLUDE_DIRS   "/usr/include" )
set(CGAL_3RD_PARTY_DEFINITIONS    "" )
set(CGAL_3RD_PARTY_LIBRARIES_DIRS "/usr/lib" )
set(CGAL_3RD_PARTY_LIBRARIES      "/usr/lib/libboost_thread-mt.so;/usr/lib/libboost_system-mt.so" )

set(CGAL_Core_3RD_PARTY_INCLUDE_DIRS   "" )
set(CGAL_Core_3RD_PARTY_DEFINITIONS    "" )
set(CGAL_Core_3RD_PARTY_LIBRARIES_DIRS "" )
set(CGAL_Core_3RD_PARTY_LIBRARIES      "" )

set(CGAL_ImageIO_3RD_PARTY_INCLUDE_DIRS   "/usr/include;/usr/include" )
set(CGAL_ImageIO_3RD_PARTY_DEFINITIONS    "-DCGAL_USE_ZLIB" )
set(CGAL_ImageIO_3RD_PARTY_LIBRARIES_DIRS "" )
set(CGAL_ImageIO_3RD_PARTY_LIBRARIES      "/usr/lib/x86_64-linux-gnu/libGLU.so;/usr/lib/x86_64-linux-gnu/libGL.so;/usr/lib/x86_64-linux-gnu/libSM.so;/usr/lib/x86_64-linux-gnu/libICE.so;/usr/lib/x86_64-linux-gnu/libX11.so;/usr/lib/x86_64-linux-gnu/libXext.so;/usr/lib/x86_64-linux-gnu/libz.so" )
set(CGAL_ImageIO_USE_ZLIB                 "ON" )

set(CGAL_Qt3_3RD_PARTY_INCLUDE_DIRS   "" )
set(CGAL_Qt3_3RD_PARTY_DEFINITIONS    "" )
set(CGAL_Qt3_3RD_PARTY_LIBRARIES_DIRS "" )
set(CGAL_Qt3_3RD_PARTY_LIBRARIES      "" )

set(CGAL_Qt4_3RD_PARTY_INCLUDE_DIRS   "/usr/include/qt4;/usr/include" )
set(CGAL_Qt4_3RD_PARTY_DEFINITIONS    "" )
set(CGAL_Qt4_3RD_PARTY_LIBRARIES_DIRS "" )
set(CGAL_Qt4_3RD_PARTY_LIBRARIES      "/usr/lib/x86_64-linux-gnu/libQtOpenGL.so;/usr/lib/x86_64-linux-gnu/libQtGui.so;/usr/lib/x86_64-linux-gnu/libQtCore.so;/usr/lib/x86_64-linux-gnu/libGLU.so;/usr/lib/x86_64-linux-gnu/libGL.so;/usr/lib/x86_64-linux-gnu/libSM.so;/usr/lib/x86_64-linux-gnu/libICE.so;/usr/lib/x86_64-linux-gnu/libX11.so;/usr/lib/x86_64-linux-gnu/libXext.so" )

set(CGAL_VERSION "${CGAL_MAJOR_VERSION}.${CGAL_MINOR_VERSION}.${CGAL_BUILD_VERSION}")

set(CGAL_USE_FILE "${CGAL_MODULES_DIR}/UseCGAL.cmake" )

set(CGAL_ALLOW_ALL_PRECONFIGURED_LIBS_COMPONENT "OFF")

if ( CGAL_FIND_REQUIRED )
  set( CHECK_CGAL_COMPONENT_MSG_ON_ERROR TRUE        )
  set( CHECK_CGAL_COMPONENT_ERROR_TYPE   FATAL_ERROR )
  set( CHECK_CGAL_COMPONENT_ERROR_TITLE  "ERROR:"    )
else()
  if ( NOT CGAL_FIND_QUIETLY )
    set( CHECK_CGAL_COMPONENT_MSG_ON_ERROR TRUE      )
    set( CHECK_CGAL_COMPONENT_ERROR_TYPE   STATUS    )
    set( CHECK_CGAL_COMPONENT_ERROR_TITLE "NOTICE:" )
  else()  
    set( CHECK_CGAL_COMPONENT_MSG_ON_ERROR FALSE )
  endif()
endif()

set(CGAL_CONFIGURED_LIBRARIES "CGAL_Core;CGAL_ImageIO;CGAL_Qt3;CGAL_Qt4")

macro(check_cgal_component COMPONENT)

  set( CGAL_LIB ${COMPONENT} )
  #message("LIB: ${CGAL_LIB}")

  if ( "${CGAL_LIB}" STREQUAL "CGAL" )
    set( CGAL_FOUND TRUE )
    set( CHECK_CGAL_ERROR_TAIL "" )
  else() 
    if ( WITH_${CGAL_LIB} )
      set( ${CGAL_LIB}_FOUND TRUE )
    else()
      set( ${CGAL_LIB}_FOUND FALSE )
    endif()
    set( CHECK_${CGAL_LIB}_ERROR_TAIL " Please configure CGAL using WITH_${CGAL_LIB}=ON." )
  endif()  

  if ( NOT ${CGAL_LIB}_FOUND AND CHECK_CGAL_COMPONENT_MSG_ON_ERROR )
    message( ${CHECK_CGAL_COMPONENT_ERROR_TYPE} "${CHECK_CGAL_COMPONENT_ERROR_TITLE} The ${CGAL_LIB} library was not configured.${CHECK_${CGAL_LIB}_ERROR_TAIL}" )
  endif()
  
endmacro()

check_cgal_component("CGAL")

foreach( CGAL_COMPONENT ${CGAL_FIND_COMPONENTS} )
  list (FIND CGAL_CONFIGURED_LIBRARIES "CGAL_${CGAL_COMPONENT}" POSITION)
  if ("${POSITION}" STRGREATER "-1") # means: CGAL_COMPONENT is contained in list
    check_cgal_component("CGAL_${CGAL_COMPONENT}")
# TODO EBEB do something for supporting lib in check_component?
  endif()
endforeach()

# Starting with cmake 2.6.3, CGAL_FIND_COMPONENTS is cleared out when find_package returns.
# But we need it within UseCGAL.cmake, so we save it aside into another variable
set( CGAL_REQUESTED_COMPONENTS ${CGAL_FIND_COMPONENTS} )

# for preconfigured libs
set(CGAL_ENABLE_PRECONFIG "ON")
set(CGAL_SUPPORTING_3RD_PARTY_LIBRARIES "GMP;GMPXX;MPFR;ZLIB;OpenGL;LEDA;MPFI;RS;RS3;OpenNL;TAUCS;Eigen3;BLAS;LAPACK;QGLViewer;ESBTL;Coin3D;NTL;IPE")
set(CGAL_ESSENTIAL_3RD_PARTY_LIBRARIES "GMP;MPFR;GMP;MPFR")

set(CGAL_EXT_LIB_Qt4_PREFIX "QT")
set(CGAL_EXT_LIB_Eigen3_PREFIX "EIGEN3")
set(CGAL_EXT_LIB_QGLViewer_PREFIX "QGLVIEWER")
set(CGAL_EXT_LIB_Coin3D_PREFIX "COIN3D")
if (NOT CGAL_IGNORE_PRECONFIGURED_GMP)
  set( GMP_FOUND           "TRUE")
  set( GMP_USE_FILE        "" )
  set( GMP_INCLUDE_DIR     "/usr/include" )
  set( GMP_LIBRARIES       "/usr/lib/x86_64-linux-gnu/libgmp.so" )
  set( GMP_DEFINITIONS     "" )
endif()

if (NOT CGAL_IGNORE_PRECONFIGURED_MPFR)
  set( MPFR_FOUND           "TRUE")
  set( MPFR_USE_FILE        "" )
  set( MPFR_INCLUDE_DIR     "/usr/local/include" )
  set( MPFR_LIBRARIES       "/usr/local/lib/libmpfr.so" )
  set( MPFR_DEFINITIONS     "" )
endif()

