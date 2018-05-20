cmake_minimum_required (VERSION 3.1)
project (readerWriterLock)

# System Requirement
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
find_package( Threads REQUIRED)

# Executable 
set( exe1 readerWriterLock.exe ) 
add_executable(${exe1} readerWriterLock.cpp)
target_link_libraries(${exe1} Threads::Threads)

enable_testing()
add_test(exe1 ${exe1})
