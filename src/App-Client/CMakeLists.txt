cmake_minimum_required(VERSION 3.20)
project(projectName LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -DDEBUG")
set(CMAKE_BUILD_TYPE "Release") #for "Debug" there were a problem with linking openssl, because there are no .cmake openssl files in ./build for debug type

add_executable(exe main.cpp ../App-Common/JsonHandler.cpp CLI.cpp Client.cpp Commands.cpp User.cpp)

target_include_directories(exe PRIVATE "../App-Common")

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

find_package(Boost REQUIRED)
if(Boost_FOUND)
    target_include_directories(exe PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(exe PRIVATE ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "Boost not found!")
endif()

find_package(nlohmann_json REQUIRED)
if(nlohmann_json_FOUND)
    target_include_directories(exe PRIVATE ${nlohmann_json_INCLUDE_DIRS})
    target_link_libraries(exe PRIVATE ${nlohmann_json_LIBRARIES})
else()
    message(FATAL_ERROR "nlohmann_json not found!")
endif()

target_link_libraries(exe PRIVATE pthread)