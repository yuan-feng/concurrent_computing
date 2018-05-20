cmake_minimum_required (VERSION 3.1)
project (Log)

# System Requirement
set (CMAKE_CXX_STANDARD 11)
find_package( Threads REQUIRED)

include_directories( ${PROJECT_SOURCE_DIR} )

# Executable 1
set( exe1 Log_test.exe ) 
add_executable(${exe1} Log_test.cpp)
target_link_libraries(${exe1} Threads::Threads)


# Executable 2
set( exe2 Log_demo.exe ) 
add_executable(${exe2} Log_demo.cpp)
target_link_libraries(${exe2} Threads::Threads)

