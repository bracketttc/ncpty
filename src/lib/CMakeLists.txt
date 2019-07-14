# C library sources
set( NCPTY_SRCS
    ncpty.c
    )

add_library( ncpty-object OBJECT ${CURSES_PTY_SRCS} )
target_include_directories( ncpty-object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include )

# C Shared library
add_library( ncpty-shared SHARED $<TARGET_OBJECTS:ncpty-object> )
target_include_directories( ncpty-object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include )
set_target_properties( ncpty-shared PROPERTIES OUTPUT_NAME "ncpty" )
add_library( ncpty ALIAS ncpty-shared )

# C Static library
add_library( ncpty-static STATIC $<TARGET_OBJECTS:ncpty-object> )
set_target_properties( ncpty-static PROPERTIES OUTPUT_NAME "ncpty" )
