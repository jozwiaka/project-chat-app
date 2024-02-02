add_library(Server STATIC Server.cpp)
target_link_libraries(Server PUBLIC pthread exe_flags ClientHandler)
target_include_directories(Server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} "../../App-Common")