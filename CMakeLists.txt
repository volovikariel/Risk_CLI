cmake_minimum_required(VERSION 3.11)
set (CMAKE_CXX_STANDARD 14)

project(Warzone)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist CACHE PATH ${CMAKE_SOURCE_DIR}/dist FORCE)

if(WIN32)
    set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "Debug" FORCE)
endif()

add_executable(Warzone_Map src/Map.cpp src/MapDriver.cpp)
add_executable(Warzone_Player src/Player.cpp src/PlayerDriver.cpp src/Map.cpp src/Orders.cpp src/Cards.cpp)
add_executable(Warzone_Orders src/Orders.cpp src/OrdersDriver.cpp src/Map.cpp src/Player.cpp src/Cards.cpp)
add_executable(Warzone_Cards src/Cards.cpp src/CardsDriver.cpp src/Map.cpp src/Player.cpp src/Orders.cpp)
add_executable(Warzone_Engine src/GameEngine.cpp src/GameEngineDriver.cpp src/Map.cpp src/Player.cpp src/Orders.cpp src/Cards.cpp)
list(APPEND BIN Warzone_Map)
list(APPEND BIN Warzone_Player)
list(APPEND BIN Warzone_Orders)
list(APPEND BIN Warzone_Cards)
list(APPEND BIN Warzone_Engine)

install(TARGETS ${BIN} DESTINATION ${CMAKE_INSTALL_PREFIX})

# Enable all compiler warnings
if(MSVC)
    add_compile_options(PRIVATE /W4)
else()
    add_compile_options(PRIVATE -Wall)
endif()
