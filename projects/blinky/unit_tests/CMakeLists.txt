cmake_minimum_required(VERSION 3.16)

project(blinky_unit_tests C)

# REPO_ROOT may be set by parent, but if this 
# is used standalone, it may be missing
if(NOT DEFINED REPO_ROOT)
  get_filename_component(REPO_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../.." ABSOLUTE)
endif()

add_executable(test_blinky
    test_blinky.c
    ${REPO_ROOT}/projects/blinky/app/blinky.c
    ${REPO_ROOT}/common/unit_tests/stubs/gpio_stub.c
)

target_include_directories(test_blinky PRIVATE
    ${REPO_ROOT}/projects/blinky/app
    ${REPO_ROOT}/common/include
    ${CMOCKA_INCLUDE_DIRS}
)

target_link_libraries(test_blinky PRIVATE 
    ${CMOCKA_LIBRARIES}
)

add_test(NAME BlinkyTest COMMAND test_blinky)
set_tests_properties(BlinkyTest PROPERTIES LABELS "blinky")