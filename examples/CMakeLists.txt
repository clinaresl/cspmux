# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable called "example" that is built from the source file "main.cc".
# The extensions are automatically found.
add_executable (example main.cc)

# Link the executable to the cspmux library. Since the cspmux library has public
# include directories we will use those link directories when building example
target_link_libraries (example LINK_PUBLIC cspmux)
