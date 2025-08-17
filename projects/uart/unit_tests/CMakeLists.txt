cmake_minimum_required(VERSION 3.16)

project(uart_echo_unit_tests C)

# REPO_ROOT may be set by parent, but if this 
# is used standalone, it may be missing
if(NOT DEFINED REPO_ROOT)
  get_filename_component(REPO_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../.." ABSOLUTE)
endif()

# Ring Buffer Tests
add_executable(test_ringbuf
    ${REPO_ROOT}/projects/uart/unit_tests/test_ringbuf.c
    ${REPO_ROOT}/common/drivers/uart/ringbuf.c
)
target_include_directories(test_ringbuf PRIVATE
    ${REPO_ROOT}/common/drivers/uart
)
target_include_directories(test_ringbuf PRIVATE
    ${CMOCKA_INCLUDE_DIRS}
)
add_test(NAME UartRingBufTest COMMAND test_ringbuf)
set_tests_properties(UartRingBufTest PROPERTIES LABELS "uart_echo")

# UART Core Tests
add_executable(test_uart_core
    ${REPO_ROOT}/projects/uart/unit_tests/test_uart_core.c
    ${REPO_ROOT}/common/drivers/uart/uart_core.c
    ${REPO_ROOT}/common/drivers/uart/ringbuf.c
    ${REPO_ROOT}/common/unit_tests/stubs/uart_hw_stub.c
)
target_include_directories(test_uart_core PRIVATE
    ${REPO_ROOT}/common/include
    ${REPO_ROOT}/common/drivers/uart
    ${REPO_ROOT}/common/unit_tests/stubs
)
target_include_directories(test_uart_core PRIVATE
    ${CMOCKA_INCLUDE_DIRS}
)
add_test(NAME UartCoreTest COMMAND test_uart_core)
set_tests_properties(UartCoreTest PROPERTIES LABELS "uart_echo")
