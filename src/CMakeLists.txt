# Create a library called cspmux which includes its source files
add_library (cspmux
  structs/MUXbmap_t.cc structs/MUXmultibmap_t.cc
  structs/MUXvalue_t.cc structs/MUXvaltable_t.cc
  structs/MUXvariable_t.cc structs/MUXvartable_t.cc
  solver/MUXmanager.cc)

# Make sure the compiler can find include files for the library when other
# libraries or executables link to it
target_include_directories (cspmux PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
