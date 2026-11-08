cmake_minimum_required(VERSION 3.21)
project(Lab7_Variant6 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB LIB_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_library(ODELib STATIC ${LIB_SOURCES})
target_include_directories(ODELib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(ODEDemo demo/main.cpp)
target_link_libraries(ODEDemo PRIVATE ODELib)

include(CTest)
find_package(GTest CONFIG QUIET)

if(GTest_FOUND)
    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "tests/*.cpp")

    add_executable(ODETests ${TEST_SOURCES})
    target_link_libraries(ODETests PRIVATE
        ODELib
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(ODETests)
else()
    message(WARNING
        "Google Test not found. Tests are disabled."
    )
endif()