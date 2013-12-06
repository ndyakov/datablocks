cmake_minimum_required(VERSION 2.8)

project(datablocks CXX)

set(EXECUTABLE "datablocks")
#set(MAIN_LIB "datablocks")

set(SRC_DIR src)
#set(TESTS_DIR tests)
set(LIB_DIR lib)

set(SRC_DIR_FULL ${PROJECT_SOURCE_DIR}/${SRC_DIR})
#set(TESTS_DIR_FULL ${PROJECT_SOURCE_DIR}/${TESTS_DIR})
set(LIB_DIR_FULL ${PROJECT_SOURCE_DIR}/${LIB_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${LIB_DIR_FULL})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(${SRC_DIR})

find_package(CxxTest)

#if (CXXTEST_FOUND)
#    enable_testing()
#    add_subdirectory(${TESTS_DIR})
#endif()

