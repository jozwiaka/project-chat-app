add_library(JsonHandler STATIC JsonHandler.cpp)
target_link_libraries(JsonHandler PUBLIC exe_flags ${nlohmann_json_LIBRARIES})
target_include_directories(JsonHandler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${nlohmann_json_INCLUDE_DIRS} "../")