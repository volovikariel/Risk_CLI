cmake_minimum_required(VERSION 3.11)
set (CMAKE_CXX_STANDARD 14)

project(Warzone)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist CACHE PATH ${CMAKE_SOURCE_DIR}/dist FORCE)

if(WIN32)
    set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "Debug" FORCE)
endif()

set(EXEC Warzone)
file(GLOB_RECURSE SRC src/*.cpp)
add_executable(${EXEC} ${SRC})
list(APPEND BIN ${EXEC})

install(TARGETS ${BIN} DESTINATION ${CMAKE_INSTALL_PREFIX})
