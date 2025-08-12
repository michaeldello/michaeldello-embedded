cmake_minimum_required(VERSION 3.16)

project(all_unit_tests C)

enable_testing()

# Use cmocka for unit testing:
# Native C framework; packaged on Ubuntu; clean usage
find_package(cmocka REQUIRED)

# Resolve repo root
get_filename_component(REPO_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
# message(STATUS "Unit test aggregator: REPO_ROOT=${REPO_ROOT}")

add_subdirectory(
  "${REPO_ROOT}/projects/blinky/unit_tests"
  "${CMAKE_CURRENT_BINARY_DIR}/blinky")
#add_subdirectory(${REPO_ROOT}/projects/uart/unit_tests)
#add_subdirectory(${REPO_ROOT}/projects/spi/unit_tests)