
file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(recordtest ${sources})
#add_executable(recordtest Runner.cpp RecordTests.cpp VariantTests.cpp)

# links gtest
target_link_libraries( recordtest ${GTEST_LIBRARIES} record )


