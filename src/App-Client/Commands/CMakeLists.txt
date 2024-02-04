add_library(Commands STATIC Commands.cpp CommandsLoggedIn.cpp CommandsNotLoggedIn.cpp)
target_link_libraries(Commands PUBLIC exe_flags JsonHandler User)
target_include_directories(Commands PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} "../../App-Common")