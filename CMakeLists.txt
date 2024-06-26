cmake_minimum_required(VERSION 3.14)
project(Descision-Helper VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

# Add the main executable
add_executable(Descision-Helper main.cpp Student.cpp CSVManager.cpp DescisionPipeline.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add the test executable
add_executable(test_cases unit_tests.cpp Student.cpp CSVManager.cpp DescisionPipeline.cpp)

target_link_libraries(
  test_cases
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_cases)