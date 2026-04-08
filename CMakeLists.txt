cmake_minimum_required(VERSION 3.10)
project(Letters C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(lttrs
    src/main.c
    src/utils.c
    src/file_manager.c
    src/path_list.c
    src/file_buffer.c
    src/text_editor.c
)

target_link_libraries(lttrs ${CURSES_LIBRARIES})

install(TARGETS lttrs DESTINATION bin)
