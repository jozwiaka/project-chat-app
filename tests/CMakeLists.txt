add_executable(exe_tests main.cpp)

target_include_directories(exe PRIVATE "../src/App-Common/")
target_include_directories(exe PRIVATE "../src/App-Server/")
target_include_directories(exe PRIVATE "../src/App-Client/")

target_link_libraries(exe PRIVATE pthread)

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

find_package(GTest REQUIRED)
if(GTest_FOUND)
    message(STATUS "GTest found!")
    target_include_directories(exe PRIVATE ${GTest_INCLUDE_DIRS})
    target_link_libraries(exe PRIVATE ${GTest_LIBRARIES})
else()
    message(FATAL_ERROR "GTest not found!")
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

enable_testing()

find_program(MEMORYCHECK_COMMAND valgrind)

if(MEMORYCHECK_COMMAND)
    set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes" "--leak-check=full")
    add_test(NAME ValgrindTest COMMAND ${MEMORYCHECK_COMMAND} ${MEMORYCHECK_COMMAND_OPTIONS} $<TARGET_FILE:exe>)
    set_tests_properties(ValgrindTest PROPERTIES PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors from 0 contexts")
endif()
