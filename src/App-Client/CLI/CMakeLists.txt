add_library(CLI STATIC CLI.cpp)
target_link_libraries(CLI PUBLIC exe_flags User Client Commands CommandHandler)
target_include_directories(CLI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} "../../App-Common")