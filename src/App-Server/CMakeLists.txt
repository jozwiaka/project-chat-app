cmake_minimum_required(VERSION 3.20)
project(projectName LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -DDEBUG")
set(CMAKE_BUILD_TYPE "Release") #for "Debug" there were a problem with linking openssl, because there are no .cmake openssl files in ./build for debug type

add_executable(exe ../App-Common/JsonHandler.cpp main.cpp ChatDatabase.cpp ClientHandler.cpp Server.cpp AuthenticationManager.cpp)

target_include_directories(exe PRIVATE "../App-Common")

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)
    target_include_directories(exe PRIVATE ${OpenSSL_INCLUDE_DIRS}) #It is not needed it I think
    target_link_libraries(exe PRIVATE ${OpenSSL_LIBRARIES}) #flags "-lssl -lcrypto" links to the system openssl

    message(STATUS "OpenSSL Version: ${OpenSSL_VERSION}")
    message(STATUS "${OpenSSL_INCLUDE_DIRS}")
    message(STATUS "${OpenSSL_LIBRARIES}")
else()
    message(FATAL_ERROR "OpenSSL not found!")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQXX REQUIRED libpqxx)
if(LIBPQXX_FOUND)
    target_include_directories(exe PRIVATE ${LIBPQXX_INCLUDE_DIRS})
    target_link_libraries(exe PRIVATE ${LIBPQXX_LIBRARIES})
else()
    message(FATAL_ERROR "libpqxx not found!")
endif()

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
