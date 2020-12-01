project(nesemu)
cmake_minimum_required(VERSION 3.14)
set (CMAKE_CXX_STANDARD 20)

include_directories("${PROJECT_SOURCE_DIR}/include")

set(SOURCE 
    src/main.cpp
    src/bus.cpp
    src/cpu6502.cpp
)

add_executable(NesEmu ${SOURCE}) 
set_target_properties(NesEmu PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)