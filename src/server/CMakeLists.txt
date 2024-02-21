find_package(OpenSSL REQUIRED)
find_package(libpqxx REQUIRED)

file(GLOB SOURCES "./*.cpp")

add_library(server ${SOURCES})

target_link_libraries(server PUBLIC
pthread
exe_flags
common
${OpenSSL_LIBRARIES}
${libpqxx_LIBRARIES}
)

target_include_directories(server PUBLIC
"./"
${OpenSSL_INCLUDE_DIRS}
${libpqxx_INCLUDE_DIRS}
)

