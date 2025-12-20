
cmake_minimum_required(VERSION 3.10)

project(NetSim VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(NETSIM_SOURCES
        src/package.cpp
        src/storage_types.cpp
        src/helpers.cpp
        include/helpers.hxx
        src/nodes.cpp
        include/nodes.hxx
)

add_library(NetSim STATIC ${NETSIM_SOURCES} include/package.hxx include/storage_types.hxx include/types.hxx)

target_include_directories(NetSim
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --------------------------------------------------
## 🚀 Tworzenie Aplikacji/Test Runnera (NetSim_App)
# --------------------------------------------------

# Pliki źródłowe aplikacji: main.cpp (Test Runner) ORAZ tests.cpp (Testy)
set(TEST_RUNNER_SOURCES
        main.cpp
        tests/tests.cpp
)

# Target NetSim_App teraz kompiluje oba pliki
add_executable(NetSim_App ${TEST_RUNNER_SOURCES})

# --------------------------------------------------
## 🧪 Testy (Konfiguracja GoogleTest)
# --------------------------------------------------

# Ścieżka do GoogleTest (jeden katalog wstecz)
set(GOOGLETEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../googletest-1.17.0)
add_subdirectory(${GOOGLETEST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/googletest)

# Sprawdzenie, czy GoogleTest został włączony
if(TARGET gtest_main)

    # Łączenie pliku wykonywalnego z biblioteką NetSim i GoogleTest
    target_link_libraries(NetSim_App
            PRIVATE
            NetSim
            gtest_main
    )

    # Włączanie testów dla CMake (dla targetu NetSim_App)
    include(CTest)
    include(GoogleTest)

    # Używamy NetSim_App jako test runnera
    gtest_discover_tests(NetSim_App)

endif()