add_library(ClientHandler STATIC ClientHandler.cpp)
target_link_libraries(ClientHandler PUBLIC exe_flags ${nlohmann_json_LIBRARIES} JsonHandler ChatDatabase AuthenticationManager)
target_include_directories(ClientHandler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${nlohmann_json_INCLUDE_DIRS} "../../App-Common")