cmake_minimum_required(VERSION 3.4)

project(covscript-dlib)

if(DEFINED ENV{CS_DEV_PATH})
    include_directories($ENV{CS_DEV_PATH}/include)
    link_directories($ENV{CS_DEV_PATH}/lib)
endif()

# Compiler Options
set(CMAKE_CXX_STANDARD 14)

if (MSVC)
    set(CMAKE_CXX_FLAGS "/O2 /EHsc /utf-8 /w")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
elseif (CMAKE_COMPILER_IS_GNUCXX)
    if (WIN32)
        set(CMAKE_C_FLAGS "--static -fPIC -s -O3")
        set(CMAKE_CXX_FLAGS "--static -fPIC -s -O3")
    else ()
        set(CMAKE_C_FLAGS "-fPIC -s -O3")
        set(CMAKE_CXX_FLAGS "-fPIC -s -O3")
    endif ()
else ()
    set(CMAKE_C_FLAGS "-fPIC -O3")
    set(CMAKE_CXX_FLAGS "-fPIC -O3")
endif ()

find_package(dlib REQUIRED)

add_library(dlib_ext SHARED dlib.cpp)

target_link_libraries(dlib_ext covscript dlib::dlib)

set_target_properties(dlib_ext PROPERTIES OUTPUT_NAME dlib)
set_target_properties(dlib_ext PROPERTIES PREFIX "")
set_target_properties(dlib_ext PROPERTIES SUFFIX ".cse")