add_library(Client STATIC Client.cpp)
target_link_libraries(Client PUBLIC exe_flags)
target_include_directories(Client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} "../../App-Common")