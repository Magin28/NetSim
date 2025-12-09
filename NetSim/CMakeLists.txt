cmake_minimum_required(VERSION 3.24)
project(NetSim)

set(CMAKE_CXX_STANDARD 20)

# ------------------------------------------------------------------------------
# Pobranie GoogleTest
# ------------------------------------------------------------------------------
include(FetchContent)
enable_testing()

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ------------------------------------------------------------------------------
# Testowa binarka = Twój main + kod projektu + tests/tests.cpp
# ------------------------------------------------------------------------------
add_executable(netsim_tests
        main.cpp
        tests/tests.cpp
        src/package.cpp
        src/storage_types.cpp
)

target_include_directories(netsim_tests PUBLIC include)
target_link_libraries(netsim_tests PRIVATE gtest netsim_tests)

include(GoogleTest)
gtest_discover_tests(netsim_tests)

