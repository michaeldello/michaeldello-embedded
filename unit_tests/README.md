# Unit Testing

The Unit Test Framework used is CMock for its simplicity and native C implementation.

## Commands

The following commands run from the repository root directory in a running Docker container based on this repository's Dockerfile will build and run all unit tests:

```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DUNIT_TESTS=ON
cmake --build . --parallel
ctest --output-on-failure
```