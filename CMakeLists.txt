cmake_minimum_required(VERSION 2.6)

project(AntennaTest)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMakeModules)

#	-----------------------------------------
#	Antenna
#	-----------------------------------------
add_subdirectory(src/antenna ${PROJECT_BINARY_DIR}/Libraries/antenna)
include_directories(antenna/src)
include_directories(${ANTENNA_INCLUDE_DIRS})
set(LIBS ${LIBS} antenna "${ANTENNA_LIBRARIES}")

set(SOURCES
    ReceiverTest.cpp
    ReceiverTest.h)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES})

add_dependencies(${PROJECT_NAME} antenna)

target_link_libraries(${PROJECT_NAME} ${LIBS})

install (TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin/
		BUNDLE DESTINATION ./ )
