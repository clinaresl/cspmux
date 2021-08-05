cmake_minimum_required (VERSION 3.21.1)
project (cspmux VERSION 1.0)

# Recurse into the "Hello" and "Demo" subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
add_subdirectory (src)
add_subdirectory (examples)
