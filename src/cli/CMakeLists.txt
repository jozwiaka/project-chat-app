file(GLOB SOURCES "./*.cpp")

add_library(cli STATIC ${SOURCES})
target_link_libraries(cli PUBLIC
pthread
exe_flags
common
client
)

target_include_directories(cli PUBLIC
"./"
)
