# Copyright (c) 2020 Timothy Brackett
# Licensed under the MIT license

set( NCPTY_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${CURSES_INCLUDE_DIRS}
    ${panel_INCLUDE_DIRS}
    ${vterm_INCLUDE_DIRS}
    )

# C library sources
set( NCPTY_SRCS
    ncpty.c
    )

add_library( ncpty-object OBJECT ${NCPTY_SRCS} )
target_include_directories( ncpty-object
    PUBLIC
    ${NCPTY_INCLUDE_DIRS}
    )
target_compile_definitions( ncpty-object PUBLIC _XOPEN_SOURCE=600 _DEFAULT_SOURCE )

# C Shared library
add_library( ncpty-shared SHARED $<TARGET_OBJECTS:ncpty-object> )
target_include_directories( ncpty-shared
    PUBLIC
    ${NCPTY_INCLUDE_DIRS}
    )
target_link_libraries( ncpty-shared
    PUBLIC
    ${CURSES_LIBRARIES}
    ${panel_LINK_LIBRARIES}
    ${vterm_LINK_LIBRARIES}
    )
set_target_properties( ncpty-shared PROPERTIES OUTPUT_NAME "ncpty" )
add_library( ncpty ALIAS ncpty-shared )
install( TARGETS ncpty-shared
    DESTINATION lib
    COMPONENT runtime
    )

# C Static library
add_library( ncpty-static STATIC $<TARGET_OBJECTS:ncpty-object> )
set_target_properties( ncpty-static PROPERTIES OUTPUT_NAME "ncpty" )
install( TARGETS ncpty-static
    DESTINATION lib
    COMPONENT devel
    )
