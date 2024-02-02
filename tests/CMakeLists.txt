add_executable(exe_tests main.cpp)

target_link_libraries(exe_tests PRIVATE exe_flags pthread ${GTest_LIBRARIES} CLI Server)
target_include_directories(exe_tests PRIVATE ${GTest_INCLUDE_DIRS} "../src/App-Common/")

enable_testing()
find_program(MEMORYCHECK_COMMAND valgrind)
if(MEMORYCHECK_COMMAND)
    set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes" "--leak-check=full")
    add_test(NAME ValgrindTest COMMAND ${MEMORYCHECK_COMMAND} ${MEMORYCHECK_COMMAND_OPTIONS} $<TARGET_FILE:exe_tests>)
    set_tests_properties(ValgrindTest PROPERTIES PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors from 0 contexts")
endif()
