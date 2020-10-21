add_subdirectory(test_server)

get_target_property(TEST_SERVER_EXE_NAME system_test_server NAME)

set(SYSTEM_TEST_SERVER_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_SERVER_EXE_NAME}${CMAKE_EXECUTABLE_SUFFIX}")

configure_file(./server_info.py.in ./server_info.py)
