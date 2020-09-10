cmake_minimum_required(VERSION 3.7.2)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

project(BigInt)
include(CTest)

file(GLOB_RECURSE SOURCES source/*.c source/*.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_executable(BigInt ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
