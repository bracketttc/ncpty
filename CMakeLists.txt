cmake_minimum_required( VERSION 3.1 )

project( banner )

find_package( Bison REQUIRED )
find_package( Curses REQUIRED )

# Insert stuff here

add_subdirectory( src/bin )
add_subdirectory( src/lib )
