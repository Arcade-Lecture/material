# this will make sure that one is using the clang compiler
set(CMAKE_CXX_COMPILER  "/usr/bin/clang++")
set(CMAKE_C_COMPILER "/usr/bin/clang")

cmake_minimum_required(VERSION 3.5)

project(03___Row_Stores)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -std=c11 -Wall -Werror")

include(
        "${CMAKE_SOURCE_DIR}/cmake/FindAPR.cmake"
)

include_directories(
        "/usr/local/include/"
        "include"
        "libs/gecko-commons/include"
        ${APR_INCLUDE_DIR}
        ${OPENSSL_INCLUDE_DIR}
)

file(GLOB_RECURSE COMMONS_SOURCE_FILES "libs/gecko-commons/src/*.c")
file(GLOB_RECURSE COMMONS_HEADER_FILES "libs/gecko-commons/src/*.h")

set(SOURCE_FILES
        include/gs_attr.h
        include/gs_tuplet_field.h
        include/gs_frag.h
        include/gs_schema.h
        include/gs_tuplet.h
        include/gs_frag_printer.h
        include/frag_printers/gs_console_printer.h
        include/gs_unsafe.h
        include/gs_field_type.h
        include/gs_pred.h
        include/frags/gs_frag_host_vm.h

        src/gs_attr.c
        src/frags/gs_frag_host_vm.c
        src/gs_tuplet_field.c
        src/gs_frag.c
        src/gs_schema.c
        src/gs_tuplet.c
        src/gs_frag_printer.c
        src/frag_printers/gs_console_printer.c
        src/gs_unsafe.c
        src/gs_field_type.c
        src/gs_pred.c
        help/exercice_helper.h help/exercice_helper.c tasks/reference/reference_implementation.h tasks/task_1/task_1.h tasks/task_2/task_2.h tasks/reference/reference_scan_impl.h
        tasks/task_1/task_1_scan_impl.h
        tasks/task_1_solution/task_1_scan_impl.h
        tasks/task_1_solution/task_1.h)

add_executable(
        exercice
        main.c
        ${SOURCE_FILES}
        ${COMMONS_HEADER_FILES}
        ${COMMONS_SOURCE_FILES}
)

target_link_libraries(
        exercice
        ${APR_LIBRARIES}
        ${APRUTIL_LIBRARIES}
        pthread
)
