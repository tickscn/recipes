set(base_SRCS
    CountDownLatch.cc
    CurrentThread.cc
    LogStream.cc
)
add_library(daina_base ${base_SRCS})
target_link_libraries(daina_base pthread)

install(TARGETS daina_base DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/daina/base)
if(DAINA_BUILD_EXAMPLES)
    add_subdirectory(tests)
endif()
