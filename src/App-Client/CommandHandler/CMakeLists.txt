add_library(CommandHandler STATIC CommandHandler.cpp)
target_link_libraries(CommandHandler PUBLIC exe_flags Commands)
target_include_directories(CommandHandler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} "../../App-Common")