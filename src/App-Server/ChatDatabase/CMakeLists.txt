add_library(ChatDatabase STATIC ChatDatabase.cpp)
target_link_libraries(ChatDatabase PUBLIC exe_flags ${LIBPQXX_LIBRARIES} JsonHandler)
target_include_directories(ChatDatabase PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${LIBPQXX_INCLUDE_DIRS} "../../App-Common")