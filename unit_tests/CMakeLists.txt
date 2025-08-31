cmake_minimum_required(VERSION 3.16)

project(all_unit_tests C)

# Enable CTest
enable_testing()

# Use cmocka for unit testing:
# Native C framework; packaged on Ubuntu; clean usage
find_package(cmocka REQUIRED)

# Resolve repo root
get_filename_component(REPO_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

add_subdirectory(
  "${REPO_ROOT}/projects/blinky/unit_tests"
  "${CMAKE_CURRENT_BINARY_DIR}/blinky")
add_subdirectory(
  "${REPO_ROOT}/projects/uart/unit_tests"
  "${CMAKE_CURRENT_BINARY_DIR}/uart")
#add_subdirectory(${REPO_ROOT}/projects/spi/unit_tests)