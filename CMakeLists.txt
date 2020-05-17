# Copyright (c) 2020 Timothy Brackett
# Licensed under the MIT license

cmake_minimum_required( VERSION 3.1 )

project( ncpty
    VERSION "0.1.0"
    DESCRIPTION "An NCurses pseudoterminal library"
    HOMEPAGE_URL "https://gitlab.com/bracketttc/ncpty"
    LANGUAGES C
    )

set( CMAKE_C_STANDARD 11 )
set( CMAKE_C_STANDARD_REQUIRED ON )
set( CMAKE_C_EXTENSIONS OFF )

option( DOC_ONLY_BUILD "Only build documentation (for readthedocs)" OFF )
if( NOT DOC_ONLY_BUILD )
    # Get dependencies
    set( CURSES_NEED_NCURSES TRUE )
    mark_as_advanced( CURSES_NEED_NCURSES )
    find_package( Curses REQUIRED )

    find_package( PkgConfig REQUIRED )
    pkg_check_modules( panel REQUIRED IMPORTED_TARGET panel )
    pkg_check_modules( vterm REQUIRED IMPORTED_TARGET vterm )
endif()

set( CMAKE_POSITION_INDEPENDENT_CODE ON )

# C flags
string( APPEND CMAKE_C_FLAGS " -ansi -Wall -Wextra -pedantic -Werror=implicit-function-declaration" )

# Security-enhancing flags
string( APPEND CMAKE_C_FLAGS " --stack-protector-strong -D_FORTIFY_SOURCE=2 -Wl,-z,relro -Wl,-z,now" )
string( APPEND CMAKE_EXE_LINKER_FLAGS " -pie" )

# Analysis
find_program( CPPCHECK "cppcheck" )
if( CPPCHECK )
    set( CMAKE_C_CPPCHECK ${CPPCHECK} --std=c11 --enable=warning,performance,portability --inline-suppr )
    mark_as_advanced( CPPCHECK )
else()
    unset( CPPCHECK )
endif()

# options
option( NCPTY_TEST "Build test code" OFF )
option( TEST_COVERAGE "Build with gcov instrumentation to evaluate test coverage" OFF )
if( NCPTY_TEST )
    include( CTest )
    enable_testing()

    if( TEST_COVERAGE )
        set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ftest-coverage -fprofile-arcs" )
    endif()
endif()

install( DIRECTORY include/
    DESTINATION include
    COMPONENT devel
    )

if( NOT DOC_ONLY_BUILD )
    add_subdirectory( src/bin )
    add_subdirectory( src/lib )
    if( NCPTY_TEST )
        add_subdirectory( src/test )
    endif()
endif()

configure_file( ncpty.pc.in
    ${CMAKE_BINARY_DIR}/ncpty.pc
    @ONLY
    )
install( FILES ${CMAKE_BINARY_DIR}/ncpty.pc
    DESTINATION share/pkgconfig
    COMPONENT devel
    )

option( BUILD_DOCUMENTATION "Generate Doxygen documentation" ON )
if( BUILD_DOCUMENTATION)
    find_package( Doxygen
        REQUIRED dot
        )
    set( DOXYGEN_GENERATE_HTML YES )
    set( DOXYGEN_GENERATE_XML YES )
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
