#	---------------------------------------------------------------------------------------
# 	2010 Nik Karpinsky
#	LGPL 2010
#
#	Antenna Library for C++
#	---------------------------------------------------------------------------------------

set(NAME antenna)

### ---------------------------------------------------------------------------------------
# Configure Dependencies
### ---------------------------------------------------------------------------------------
find_package(libwebsockets REQUIRED)
include_directories(${LIBWEBSOCKETS_INCLUDE_DIRS})
set(ANTENNA_INCLUDE_DIRS ${ANTENNA_INCLUDE_DIRS} ${LIBWEBSOCKETS_INCLUDE_DIRS})
set(ANTENNA_LIBRARIES ${ANTENNA_LIBRARIES} ${LIBWEBSOCKETS_LIBRARIES})

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})
set(ANTENNA_INCLUDE_DIRS ${ANTENNA_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})
set(ANTENNA_LIBRARIES ${ANTENNA_LIBRARIES} ${ZLIB_LIBRARY})

#	---------------------------------------------------------------------------------------
#	List all of the sources to be compiled
#	---------------------------------------------------------------------------------------

set(SOURCES
	
	Transmitter.cpp
	BaseStation.cpp

	Transmitter.h
	BaseStation.h
)

source_group("antenna" FILES ${SOURCES})

include_directories(..)

#	---------------------------------------------------------------------------------------
#	Build Documentation
#	---------------------------------------------------------------------------------------
option(ANTENNA_GENERATE_DOC "Build Doxygen Documentation" OFF)
if(ANTENNA_GENERATE_DOC)
	add_subdirectory(../../doc ${PROJECT_BINARY_DIR}/doc)
endif(ANTENNA_GENERATE_DOC)

#	---------------------------------------------------------------------------------------
#	Build the library
#	---------------------------------------------------------------------------------------

add_library(${NAME} ${SOURCES})

