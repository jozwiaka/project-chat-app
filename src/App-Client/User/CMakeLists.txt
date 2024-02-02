add_library(User STATIC User.cpp)
target_link_libraries(User PUBLIC exe_flags)
target_include_directories(User PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} "../../App-Common")