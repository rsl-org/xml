cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 26)

project(rsl_serialize CXX)

add_library(rsl_serialize)
target_include_directories(rsl_serialize PUBLIC include)
add_subdirectory(src)

target_compile_options(rsl_serialize PUBLIC "-freflection-latest")
target_include_directories(rsl_serialize PUBLIC 
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

find_package(rsl-util REQUIRED)
target_link_libraries(rsl_serialize PUBLIC rsl::util)


install(TARGETS rsl_serialize)
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
  add_executable(rsl_serialize_test)
  add_subdirectory(test)

  find_package(GTest REQUIRED)
  target_link_libraries(rsl_serialize_test PRIVATE rsl_serialize)
  target_link_libraries(rsl_serialize_test PRIVATE GTest::gtest)

  include(GoogleTest)
  gtest_discover_tests(rsl_serialize_test)
endif()