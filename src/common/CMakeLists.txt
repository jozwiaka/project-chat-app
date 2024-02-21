find_package(nlohmann_json REQUIRED)

file(GLOB SOURCES "./*.cpp")

add_library(common STATIC ${SOURCES})

target_link_libraries(common PUBLIC exe_flags ${nlohmann_json_LIBRARIES})

target_include_directories(
    common PUBLIC
    ${nlohmann_json_INCLUDE_DIRS}
    "./"
)
