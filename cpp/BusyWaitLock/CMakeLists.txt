cmake_minimum_required (VERSION 3.1)
project (MPMCQueue)

# System Requirement
set (CMAKE_CXX_STANDARD 11)
find_package( Threads REQUIRED)

include_directories( ${PROJECT_SOURCE_DIR} )

# Executable
set( exe1 example.exe ) 
add_executable(${exe1} example.cpp)
target_link_libraries(${exe1} Threads::Threads)

