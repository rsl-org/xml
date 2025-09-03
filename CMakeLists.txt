cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 26)

project(rsl-xml CXX)

add_library(rsl-xml)
target_include_directories(rsl-xml PUBLIC include)
add_subdirectory(src)

target_compile_options(rsl-xml PUBLIC "-freflection-latest")
target_include_directories(rsl-xml PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

find_package(rsl-util REQUIRED)
target_link_libraries(rsl-xml PUBLIC rsl::util)


install(TARGETS rsl-xml)
install(DIRECTORY include/ DESTINATION include)

option(BUILD_EXAMPLES "Enable examples" ON)
if (BUILD_EXAMPLES)
  add_subdirectory(example)
endif()

option(BUILD_TESTING "Enable tests" ON)
option(BUILD_EXAMPLES "Enable examples" ON)

if (BUILD_TESTING)
  message(STATUS "Building unit tests")

  enable_testing()
  add_executable(rsl-xml-test)
  add_subdirectory(test)

  find_package(GTest REQUIRED)
  target_link_libraries(rsl-xml-test PRIVATE rsl-xml)
  target_link_libraries(rsl-xml-test PRIVATE GTest::gtest)

  include(GoogleTest)
  gtest_discover_tests(rsl-xml-test)
endif()
