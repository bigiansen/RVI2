enable_testing()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/test_output/lib/shared)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/test_output/lib/static)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/test_output/bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/test_output/lib/shared)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/test_output/lib/static)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/test_output/bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/test_output/lib/shared)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/test_output/lib/static)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/test_output/bin)

add_subdirectory(core)