cmake_minimum_required(VERSION 3.11)
set (CMAKE_CXX_STANDARD 14)

project(Warzone)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist CACHE PATH ${CMAKE_SOURCE_DIR}/dist FORCE)

if(WIN32)
    set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "Debug" FORCE)
endif()

add_executable(Warzone_CommandProcessing src/CommandProcessingDriver.cpp src/CommandProcessing.cpp src/Cards.cpp src/GameEngine.cpp src/LoggingObserver.cpp src/Map.cpp src/Orders.cpp src/Player.cpp)
add_executable(Warzone_StartupPhase src/StartupPhaseDriver.cpp src/CommandProcessing.cpp src/Cards.cpp src/GameEngine.cpp src/LoggingObserver.cpp src/Map.cpp src/Orders.cpp src/Player.cpp)
add_executable(Warzone_MainLoop src/MainLoopDriver.cpp src/CommandProcessing.cpp src/Cards.cpp src/GameEngine.cpp src/LoggingObserver.cpp src/Map.cpp src/Orders.cpp src/Player.cpp)
add_executable(Warzone_OrderExecution src/OrderExecutionDriver.cpp src/CommandProcessing.cpp src/Cards.cpp src/GameEngine.cpp src/LoggingObserver.cpp src/Map.cpp src/Orders.cpp src/Player.cpp)
add_executable(Warzone_LoggingObserver src/LoggingObserverDriver.cpp src/CommandProcessing.cpp src/Cards.cpp src/GameEngine.cpp src/LoggingObserver.cpp src/Map.cpp src/Orders.cpp src/Player.cpp)
list(APPEND BIN Warzone_CommandProcessing)
list(APPEND BIN Warzone_StartupPhase)
list(APPEND BIN Warzone_MainLoop)
list(APPEND BIN Warzone_OrderExecution)
list(APPEND BIN Warzone_LoggingObserver)

install(TARGETS ${BIN} DESTINATION ${CMAKE_INSTALL_PREFIX})

# Enable all compiler warnings
if(MSVC)
    add_compile_options(PRIVATE /W4)
else()
    add_compile_options(PRIVATE -Wall)
endif()
