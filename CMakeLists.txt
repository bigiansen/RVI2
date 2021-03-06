set(RVI_MIN_CMAKE_VER "3.7.0")
cmake_minimum_required (VERSION ${RVI_MIN_CMAKE_VER})

message("")
message("****************************************")
message("**               ------               **")
message("**             -  RVI2  -             **")
message("**               ------               **")
message("****************************************")
message("")

project(rvi2 LANGUAGES CXX C)

option(RVI2_BUILD_SAMPLES 	"Build sample application(s)" 	ON)
option(RVI2_BUILD_TESTS	 	"Build tests" 					ON)
option(RVI2_BUILD_COZY 		"Build cozy openGL backend" 	ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY 			${PROJECT_BINARY_DIR}/output/lib/shared)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY 			${PROJECT_BINARY_DIR}/output/lib/static)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 			${PROJECT_BINARY_DIR}/output/bin/no-category)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG 	${PROJECT_BINARY_DIR}/output/lib/shared)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG 	${PROJECT_BINARY_DIR}/output/lib/static)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${PROJECT_BINARY_DIR}/output/bin/no-category)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE 	${PROJECT_BINARY_DIR}/output/lib/shared)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE 	${PROJECT_BINARY_DIR}/output/lib/static)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	${PROJECT_BINARY_DIR}/output/bin/no-category)

set_property(GLOBAL PROPERTY RVI2_DATA_DIR ${PROJECT_SOURCE_DIR}/data)
get_property(RVI2_DATA_DIR GLOBAL PROPERTY RVI2_DATA_DIR)

add_subdirectory("third-party")
add_subdirectory("lib")

if(RVI2_BUILD_SAMPLES)
    add_subdirectory("samples")
endif()

if(RVI2_BUILD_TESTS)
    add_subdirectory("test")
endif()