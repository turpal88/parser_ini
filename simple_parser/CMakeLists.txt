﻿

add_executable(simple_parser simple_parser.cpp)
set(CMAKE_CXX_STANDARD 17)
include_directories(${PROJECT_SOURCE_DIR}/parser_ini)
target_link_libraries(simple_parser parser_ini)