find_package(GTest REQUIRED)

add_executable(test_os_tree
    test_os_tree.cpp
    ../src/os_tree.cpp
    ../src/dothtml.cpp
)

target_link_libraries(test_os_tree GTest::gtest_main)

add_test(NAME test_os_tree COMMAND test_os_tree)
