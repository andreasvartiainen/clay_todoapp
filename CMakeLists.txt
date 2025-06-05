cmake_minimum_required(VERSION 3.10)

project(clayProject)

# C++ standard to 23 to support std::print/ln()
set(CMAKE_CXX_STANDARD 23)
# add warnings as errors -Werror not in this because it causes catch2 to fail
add_compile_options(-Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion)

include(FetchContent)

FetchContent_Declare(
	raylib
	GIT_REPOSITORY https://github.com/raysan5/raylib
	GIT_TAG 5.5
	FIND_PACKAGE_ARGS ${RAYLIB_VERSION} EXACT
)

set(BUILD_EXAMPLES OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(raylib)

# globbing the sources in src folder to not require manual adding of .cpp files
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(APPEND SOURCES "ClayMan/clayman.cpp")
# remove main.cpp because that causes problems in the unitTests
list(FILTER SOURCES EXCLUDE REGEX "main.cpp" )

# include dirs for adding them easier to the unitTests
set(INCLUDE_DIRS
	${CMAKE_SOURCE_DIR}/src/include
)

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})
target_link_libraries(${PROJECT_NAME} raylib)

include_directories(
	PRIVATE
		${INCLUDE_DIRS}
)

