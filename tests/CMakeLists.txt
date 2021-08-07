enable_testing()
find_package(GTest REQUIRED)

add_executable(gtest gtest.cc TSThelpers.cc structs/TSTbmap_t.cc)
target_link_libraries(gtest LINK_PUBLIC cspmux GTest::gtest GTest::gtest_main)

add_test(AllTests gtest)
