cmake_minimum_required(VERSION 3.23)

set(TARGET_NAME Messenger)


project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 20)

add_executable(Messenger main.cpp)

set(COMMON_FILES
        "Common/Header/CommunicationUnit.h"
        "Common/Header/Core.h"
        "Common/Header/Connection.h"
        "Common/Source/CommunicationUnit.cpp"
        )
add_subdirectory(Server)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

