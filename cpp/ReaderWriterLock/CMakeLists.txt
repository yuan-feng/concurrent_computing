cmake_minimum_required (VERSION 3.1)
project (readerWriterLock)

# System Requirement
set (CMAKE_CXX_STANDARD 14)
find_package( Threads REQUIRED)

include_directories( ${PROJECT_SOURCE_DIR} )

# Executable 
set( exe1 readerWriterLock.exe ) 
add_executable(${exe1} readerWriterLock.cpp)
target_link_libraries(${exe1} Threads::Threads)
