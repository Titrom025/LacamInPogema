cmake_minimum_required(VERSION 3.16)
project(lacam-project CXX)

add_subdirectory(./lacam3)
add_subdirectory(./third_party/argparse)

add_library(lacam SHARED
    main.cpp
)
target_compile_features(lacam PUBLIC cxx_std_17)
target_include_directories(lacam PRIVATE ./lacam3/include ./third_party/argparse/include)
target_link_libraries(lacam lacam3 argparse)

add_executable(main_exec main.cpp)
target_link_libraries(main_exec lacam)