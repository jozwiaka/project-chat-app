file(GLOB SOURCES "./*.cpp")

add_library(client STATIC ${SOURCES})
target_link_libraries(client PUBLIC
pthread
exe_flags
common
)

target_include_directories(client PUBLIC
"./"
)
