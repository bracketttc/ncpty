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

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

option( DOC_ONLY_BUILD "Only build documentation (for readthedocs)" OFF )
if( NOT DOC_ONLY_BUILD )
    # Get dependencies
    set( CURSES_NEED_NCURSES TRUE )
    mark_as_advanced( CURSES_NEED_NCURSES )
    find_package( Curses REQUIRED )

    find_package( PkgConfig REQUIRED )
    pkg_check_modules( panel REQUIRED IMPORTED_TARGET panel )
    pkg_check_modules( vterm REQUIRED IMPORTED_TARGET vterm )
    mark_as_advanced( pkgcfg_lib_panel_panel pkgcfg_lib_vterm_vterm )
endif()

set( CMAKE_POSITION_INDEPENDENT_CODE ON )

option( USE_INTERNAL_NCURSES_BUILD "" OFF )
if( USE_INTERNAL_NCURSES_BUILD )
    include( ExternalProject )
    ExternalProject_Add( ncurses
        URL      https://ftp.gnu.org/pub/gnu/ncurses/ncurses-6.2.tar.gz
        URL_HASH SHA256=30306e0c76e0f9f1f0de987cf1c82a5c21e1ce6568b9227f7da5b71cbea86c9d
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/deps/ncurses
        INSTALL_DIR ${CMAKE_BINARY_DIR}/ncurses
        CONFIGURE_COMMAND ${CMAKE_SOURCE_DIR}/deps/ncurses/configure --prefix=<INSTALL_DIR> --disable-leaks --with-shared --disable-pc-files --disable-rpath --enable-echo --disable-stripping --enable-const --without-ada --without-tests --without-progs --enable-symlinks --disable-termcap --with-default-terminfo-dir=/etc/terminfo --with-terminfo-dirs=/etc/terminfo:/lib/terminfo:/usr/share/terminfo --with-ticlib=tic --with-termlib-tinfo --with-versioned-syms --disable-wattr-macros --disable-relink --enable-overwrite --with-xterm-kbs=del
        )
    set( CURSES_CURSES_LIBRARY ${CMAKE_BINARY_DIR}/ncurses/lib/libcurses.so CACHE PATH "" FORCE )
    set( CURSES_FORM_LIBRARY ${CMAKE_BINARY_DIR}/ncurses/lib/libform.so CACHE PATH "" FORCE )
    set( CURSES_INCLUDE_PATH ${CMAKE_BINARY_DIR}/ncurses/include CACHE PATH "" FORCE )
    set( CURSES_NCURSES_LIBRARY ${CMAKE_BINARY_DIR}/ncurses/lib/libncurses.so CACHE PATH "" FORCE )
    set( pkgcfg_lib_panel_panel ${CMAKE_BINARY_DIR}/ncurses/lib/libpanel.so CACHE PATH "" FORCE )
endif()
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
option( TEST_COVERAGE "Build with gcov instrumentation to evaluate test coverage" OFF )
include( CTest )
enable_testing()

if( TEST_COVERAGE )
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ftest-coverage -fprofile-arcs" )
endif()

install( DIRECTORY include/
    DESTINATION include
    COMPONENT devel
    )

if( NOT DOC_ONLY_BUILD )
    add_subdirectory( src/bin )
    add_subdirectory( src/lib )
    add_subdirectory( src/test )
endif()

configure_file( ncpty.pc.in
    ${CMAKE_BINARY_DIR}/ncpty.pc
    @ONLY
    )
install( FILES ${CMAKE_BINARY_DIR}/ncpty.pc
    DESTINATION share/pkgconfig
    COMPONENT devel
    )

include( CMakePackageConfigHelpers )
configure_package_config_file( ${CMAKE_CURRENT_SOURCE_DIR}/ncptyConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/ncptyConfig.cmake"
    INSTALL_DESTINATION lib/cmake/ncpty
    )
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ncptyConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
    )
install( FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ncptyConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ncptyConfigVersion.cmake
    DESTINATION lib/cmake/ncpty
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
