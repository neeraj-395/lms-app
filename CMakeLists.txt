cmake_minimum_required(VERSION 3.16)

project(lms-app VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "/home/neeraj/Qt/6.7.1/gcc_64/lib/cmake")

add_subdirectory(src)
#add_subdirectory(test)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
