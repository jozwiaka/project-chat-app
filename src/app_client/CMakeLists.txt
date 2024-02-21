add_executable(exe_client main.cpp)
target_link_libraries(exe_client PUBLIC
pthread
exe_flags
cli
)

