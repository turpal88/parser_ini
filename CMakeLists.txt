cmake_minimum_required(VERSION 3.12)
project(simple_parser)
add_executable(simple_parser simple_parser.cpp)

#target_include_directories(simple_parser PUBLIC ${PROJECT_SOURCE_DIR}/include)
add_library(parser_ini INTERFACE ${PROJECT_SOURCE_DIR}/include/parser_ini.h)

target_link_libraries(simple_parser PUBLIC parser_ini)
