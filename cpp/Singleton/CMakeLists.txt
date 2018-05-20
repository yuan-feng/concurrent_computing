cmake_minimum_required (VERSION 3.1)
project (Log)

# System Requirement
set (CMAKE_CXX_STANDARD 14)
find_package( Threads REQUIRED)

include_directories( ${PROJECT_SOURCE_DIR} )

# Executable 
set( exe1 Singleton_test.exe ) 
add_executable(${exe1} Singleton_test.cpp)
target_link_libraries(${exe1} Threads::Threads)

