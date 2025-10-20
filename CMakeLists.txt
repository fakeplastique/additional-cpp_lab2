cmake_minimum_required(VERSION 3.10.0)
project(additional-cpp_lab2)

if (MSVC)
    add_compile_options(/W3)
    add_compile_options(/std:c++23preview)
    add_compile_options(/EHsc)
    add_compile_options(/utf-8)
    add_compile_options(/Zc:tlsGuards)
    add_compile_options(/Zc:threadSafeInit)
else()
    add_compile_options(-Wall)
    add_compile_options(-std=c++23)
    add_compile_options(-fexceptions)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    add_compile_options(-O3)
endif()



set(CMAKE_CXX_STANDARD 23)

find_package(TBB REQUIRED)

add_executable(additional-cpp_lab2
        main.cpp
        timeit
)


target_link_libraries(additional-cpp_lab2 PRIVATE TBB::tbb)
