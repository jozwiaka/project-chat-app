add_library(ChatDatabase STATIC ChatDatabase.cpp)
target_link_libraries(ChatDatabase PUBLIC exe_flags ${libpqxx_LIBRARIES} JsonHandler)
target_include_directories(ChatDatabase PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${libpqxx_INCLUDE_DIRS} "../../App-Common")