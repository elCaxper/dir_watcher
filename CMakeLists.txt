cmake_minimum_required(VERSION 3.8)
project(filesystem_watcher_lib CXX)
include(GNUInstallDirs)
#set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "--std=c++17 -fpermissive -Wall -Wextra")

add_library(watcher SHARED
        lib/watcher.cpp
        lib/include/watcher.h
        )

set_target_properties(watcher PROPERTIES VERSION 1.0.0)
set_target_properties(watcher PROPERTIES PUBLIC_HEADER lib/include/watcher.h)
target_include_directories(watcher PRIVATE lib/include)
install(TARGETS watcher
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set_target_properties(watcher PROPERTIES
        VERSION 1.0.0
        SOVERSION 1
        PUBLIC_HEADER lib/include/watcher.h)


set(SOURCE_FILES main.cpp)
set_source_files_properties(${SOURCE_FILES} PROPERTIES LANGUAGE CXX)
add_executable(filesystem_watcher_lib ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries (filesystem_watcher_lib LINK_PUBLIC watcher stdc++fs)
target_include_directories(filesystem_watcher_lib PRIVATE lib/include)
