cmake_minimum_required(VERSION 3.21)

set(APP_NAME vibe72)
set(APP_VERSION "1.0.0")

project(${APP_NAME})

set(CMAKE_BUILD_TYPE RELEASE)
set(EXECUTABLE_OUTPUT_PATH "bin")

set(
    HEADERS
    src/prgid.hpp
    src/const.hpp
    src/auxf.hpp
    src/conf.hpp
    src/calc.hpp
)

set(
    SOURCES
    src/main.cpp
    src/auxf.cpp
    src/conf.cpp
    src/calc.cpp
)

set(CMAKE_CXX_COMPILER_ARCHITECTURE_ID x64)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
