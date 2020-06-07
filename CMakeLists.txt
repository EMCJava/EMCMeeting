cmake_minimum_required(VERSION 3.16)
project(EMCMeeting)

set(CMAKE_CXX_STANDARD 14)

# graphic library sfml
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake-module) # Tell CMake where to find the module
find_package(SFML 2 REQUIRED COMPONENTS graphics window system) # Look for SFML
include_directories( PUBLIC ${SFML_INCLUDE_DIRS})

add_executable(EMCMeeting main.cpp)

target_link_libraries(EMCMeeting ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ) # Link SFML and dependencies