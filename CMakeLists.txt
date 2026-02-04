cmake_minimum_required(VERSION 3.10)
project(NetworkMonitor)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Full static linking
set(CMAKE_EXE_LINKER_FLAGS "-static")

set(SOURCES
    src/main.cpp
    src/network_stats.cpp
    src/tcp_server.cpp
)

add_executable(network_monitor ${SOURCES})
target_link_libraries(network_monitor pthread)
install(TARGETS network_monitor DESTINATION /usr/bin)
