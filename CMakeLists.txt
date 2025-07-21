cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 26)

project(rsl_xml CXX)

add_library(rsl_xml)
target_include_directories(rsl_xml PUBLIC include)
add_subdirectory(src)

target_compile_options(rsl_xml PUBLIC "-freflection-latest")
target_include_directories(rsl_xml PUBLIC 
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

find_package(rsl-util REQUIRED)
target_link_libraries(rsl_xml PUBLIC rsl::util)


install(TARGETS rsl_xml)
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
  add_executable(rsl_xml_test)
  add_subdirectory(test)

  find_package(GTest REQUIRED)
  target_link_libraries(rsl_xml_test PRIVATE rsl_xml)
  target_link_libraries(rsl_xml_test PRIVATE GTest::gtest)

  include(GoogleTest)
  gtest_discover_tests(rsl_xml_test)
endif()