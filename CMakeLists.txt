cmake_minimum_required(VERSION 3.22)
project(project-chat-app LANGUAGES CXX)
set(CMAKE_BUILD_TYPE "Release") #for "Debug" there were a problem with linking openssl, because there are no .cmake openssl files in ./build for debug type

add_library(exe_flags INTERFACE)
target_compile_features(exe_flags INTERFACE cxx_std_23)
target_compile_options(exe_flags INTERFACE
  "$<$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:$<BUILD_INTERFACE:-W3>>"
)

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

add_subdirectory(src)
add_subdirectory(tests)