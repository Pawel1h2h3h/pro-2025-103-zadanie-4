cmake_minimum_required(VERSION 3.10)
project(MyMap LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.9
)
FetchContent_MakeAvailable(catch2)

# Enforce C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Header-only library target
set(MYMAP_HEADERS
    stdMap.hpp
    stdMap.tpp
    iterator.hpp
    iterator.tpp
)

add_library(MyMap INTERFACE)
target_sources(MyMap INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${MYMAP_HEADERS}
)
target_include_directories(MyMap INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Main executable
add_executable(main main.cpp)
target_link_libraries(main PRIVATE MyMap)

# Enable testing (optional)
include(CTest)
enable_testing()

# Test executable for Catch2 suite
add_executable(test_my_map tests/test.cpp)
target_link_libraries(test_my_map PRIVATE MyMap Catch2::Catch2)
add_test(NAME my_map_tests COMMAND test_my_map)

# Example test executable (if you add tests)
# add_executable(test_my_map tests/test.cpp)
# target_link_libraries(test_my_map PRIVATE MyMap)
# add_test(NAME my_map_tests COMMAND test_my_map)

# Install rules (optional)
install(TARGETS MyMap
    EXPORT MyMapTargets
    FILE_SET HEADERS DESTINATION include
)
install(EXPORT MyMapTargets
    FILE MyMapTargets.cmake
    NAMESPACE MyMap::
    DESTINATION lib/cmake/MyMap
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.tpp"
)
