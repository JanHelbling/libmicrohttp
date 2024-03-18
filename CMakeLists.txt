cmake_minimum_required(VERSION 3.27)
project(libmicrohttp C)

set(CMAKE_C_STANDARD 11)

add_library(libmicrohttp src/http_func.c src/parse_url.c)
include_directories(include)
target_sources(libmicrohttp PUBLIC include/libmicrohttp.h)

get_target_property(LIBMICROHTTP_PUBLIC_HEADERS libmicrohttp INTERFACE_SOURCES)
set_target_properties(libmicrohttp PROPERTIES PUBLIC_HEADER "${LIBMICROHTTP_PUBLIC_HEADERS}")
install(TARGETS libmicrohttp PUBLIC_HEADER DESTINATION)