# Copyright (c) 2020 Timothy Brackett
# Licensed under the MIT license

cmake_minimum_required( VERSION 3.1 )

project( ncpty
    VERSION "0.1.0"
    DESCRIPTION "An NCurses pseudoterminal library"
    HOMEPAGE_URL "https://gitlab.com/bracketttc/ncpty"
    )

find_package( Bison REQUIRED )
find_package( Curses REQUIRED )

# Insert stuff here

add_subdirectory( src/bin )
add_subdirectory( src/lib )

option( BUILD_DOCUMENTATION "" ON )
if( BUILD_DOCUMENTATION)
    find_package( Doxygen
        REQUIRED dot
        )
    doxygen_add_docs( docs
        include
        src
        )
endif()
