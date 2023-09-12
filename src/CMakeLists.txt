add_library(record STATIC record.cpp Variant.cpp record.h)
target_link_libraries(record ${JSONCPP_LIBRARY})

