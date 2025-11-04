cmake_minimum_required(VERSION 3.14)
project(OS_Tree)

set(CMAKE_CXX_STANDARD 17)

option(DEBUG "Enable debug mode" OFF)
option(BUILD_TESTS "Build tests" ON)

add_executable(tree_app
    src/main.cpp
    src/os_tree.cpp
    src/dothtml.cpp
)

set_target_properties(tree_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/app
)

add_custom_target(run_input
    COMMAND $<TARGET_FILE:tree_app> < ${CMAKE_SOURCE_DIR}/data/input.txt
    DEPENDS tree_app
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_executable(benchmark
    src/benchmark.cpp
    src/os_tree.cpp
    src/dothtml.cpp
)

set_target_properties(benchmark PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/app
)

add_custom_target(run_benchmark
    COMMAND $<TARGET_FILE:benchmark>
    DEPENDS benchmark
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)


if(DEBUG)
    target_compile_definitions(tree_app PRIVATE DEBUG)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
