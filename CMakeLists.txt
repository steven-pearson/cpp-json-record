cmake_minimum_required(VERSION 3.0.0...3.5)
project(record VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_ARGS -DJSONCPP_INCLUDE_PREFIX=json)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)

# Code Coverage Configuration
add_library(coverage_config INTERFACE)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# sets GTEST_INCLUDE_DIRS and GTEST_LIBRARIES
find_package( GTest REQUIRED )      

# adds the gtest include directory
include_directories( ${GTEST_INCLUDE_DIRS} )

include(CTest)
enable_testing()
find_package(Jsoncpp REQUIRED)
include_directories(${JSONCPP_INCLUDE_DIR})

option(CODE_COVERAGE "Enable coverage reporting" ON)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(coverage_config INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(coverage_config INTERFACE --coverage)
  else()
    target_link_libraries(coverage_config INTERFACE --coverage)
  endif()
endif(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

include(CodeCoverage)
append_coverage_compiler_flags()

add_subdirectory(src)
add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_test(Tester tests/recordtest)