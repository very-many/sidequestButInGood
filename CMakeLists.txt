cmake_minimum_required(VERSION 3.18)

project(Sidequest CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_BINARY_DIR ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})
set(CMAKE_USE_RELATIVE_PATHS FALSE)

set(SIDEQUEST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/sidequest")
set(SIDEQUEST_SERVER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/server")
set(SIDEQUEST_DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")
include_directories(${SIDEQUEST_INCLUDE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(GetSourceFiles)

set(FILE_TYPES *.cpp *.h )
find_directories(LIBRARY_SOURCE_DIRS "src/sidequest" "${FILE_TYPES}")
generate_source_tree(LIBRARY_SOURCES "${LIBRARY_SOURCE_DIRS}" "${FILE_TYPES}")

find_directories(SERVER_SOURCE_DIRS "src/server" "${FILE_TYPES}")
generate_source_tree(SERVER_SOURCES "${SERVER_SOURCE_DIRS}" "${FILE_TYPES}")

link_directories(${SIDEQUEST_DEPENDENCIES_DIR})
add_library(sidequest_lib STATIC ${LIBRARY_SOURCES} )

add_executable(server ${SERVER_SOURCES} )
add_dependencies(server sidequest_lib)
target_link_libraries(server sidequest_lib)  
target_link_libraries(server sqlite3)  

include_directories(${SIDEQUEST_INCLUDE_DIR})
include_directories(${SIDEQUEST_SERVER_INCLUDE_DIR})
include_directories(${SIDEQUEST_DEPENDENCIES_DIR})

