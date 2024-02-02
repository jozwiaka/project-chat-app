
add_executable(exe_server main.cpp)
target_link_libraries(exe_server PRIVATE pthread exe_flags)
