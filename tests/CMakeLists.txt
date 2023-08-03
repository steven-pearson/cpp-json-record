
add_executable(recordtest recordtests.cpp)

# links gtest
target_link_libraries( recordtest ${GTEST_LIBRARIES} record )


