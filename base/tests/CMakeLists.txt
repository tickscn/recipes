add_executable(blockingqueue_test BlockingQueue_test.cc)
target_link_libraries(blockingqueue_test daina_base)

add_executable(logstream_test LogStream_test.cc)
target_link_libraries(logstream_test daina_base gtest gtest_main)
