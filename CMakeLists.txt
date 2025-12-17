cmake_minimum_required(VERSION 3.10)
project(termsaver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find ncursesw via PkgConfig
find_package(PkgConfig REQUIRED)
pkg_check_modules(NCURSESW REQUIRED ncursesw)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/logos
    ${CMAKE_SOURCE_DIR}/src/states
    ${NCURSESW_INCLUDE_DIRS}
)

# Source files
file(GLOB SOURCES 
    "src/core/*.cpp"
    "src/logos/*.cpp"
    "src/states/*.cpp"
    "src/main.cpp"
)

add_executable(termsaver ${SOURCES})

# Link libraries
target_link_libraries(termsaver ${NCURSESW_LIBRARIES})
