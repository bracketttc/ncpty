# Copyright (c) 2020 Timothy Brackett
# Licensed under the MIT license

cmake_minimum_required( VERSION 3.1 )

project( ncpty
    VERSION "0.1.0"
    DESCRIPTION "An NCurses pseudoterminal library"
    HOMEPAGE_URL "https://gitlab.com/bracketttc/ncpty"
    LANGUAGES C
    )

# Get dependencies
set( CURSES_NEED_NCURSES TRUE )
mark_as_advanced( CURSES_NEED_NCURSES )
find_package( Curses REQUIRED )

find_package( PkgConfig REQUIRED )
pkg_check_modules( panel REQUIRED IMPORTED_TARGET panel )
pkg_check_modules( vterm REQUIRED IMPORTED_TARGET vterm )

set( CMAKE_POSITION_INDEPENDENT_CODE ON )

# Insert stuff here
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Werror=implicit-function-declaration" )

# Analysis
find_program( CPPCHECK "cppcheck" )
if( CPPCHECK )
    set( CMAKE_C_CPPCHECK ${CPPCHECK} )
else()
    unset( CPPCHECK )
endif()

install( DIRECTORY include/
    DESTINATION include
    COMPONENT devel
    )

add_subdirectory( src/bin )
add_subdirectory( src/lib )

configure_file( ncpty.pc.in
    ${CMAKE_BINARY_DIR}/ncpty.pc
    @ONLY
    )
install( FILES ${CMAKE_BINARY_DIR}/ncpty.pc
    DESTINATION share/pkgconfig
    COMPONENT devel
    )

option( BUILD_DOCUMENTATION "" ON )
if( BUILD_DOCUMENTATION)
    find_package( Doxygen
        REQUIRED dot
        )
    set( DOXYGEN_GENERATE_HTML YES )
    set( DOXYGEN_GENERATE_MAN YES )
    mark_as_advanced( DOXYGEN_GENERATE_MAN )
    doxygen_add_docs( doc
        include
        src
        )
endif()

set( CPACK_GENERATOR "TGZ" )
set( CPACK_SOURCE_GENERATOR "TGZ" )
set( CPACK_SOURCE_IGNORE_FILES .git/;build/ )
include( CPack )
