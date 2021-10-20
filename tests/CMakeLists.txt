enable_testing()
find_package(GTest REQUIRED)

# add_executable(gtest gtest.cc
#   TSThelpers.cc
#   structs/TSTbmap_t.cc
#   structs/TSTmultibmap_t.cc
#   structs/TSTmultivector_t.cc
#   structs/TSTvaltable_t.cc
#   structs/TSTvariable_t.cc
#   structs/TSTvartable_t.cc
#   solver/TSTaction_t.cc
#   solver/TSTframe_t.cc
#   solver/TSTsstack_t.cc
#   solver/TSTmanager.cc)

add_executable(gtest gtest.cc
  TSThelpers.cc
  structs/TSTbmap_t.cc
  structs/TSTmultibmap_t.cc
  structs/TSTmultivector_t.cc
  structs/TSTvaltable_t.cc)

target_link_libraries(gtest LINK_PUBLIC cspmux GTest::gtest GTest::gtest_main)

add_test(AllTests gtest)
