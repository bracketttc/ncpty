# Copyright (c) 2020 Timothy Brackett
# Licensed under the MIT license

add_test( NAME banner_test
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/banner_test.sh $<TARGET_FILE:banner>
    )

add_executable( ncpty_test ncpty_test.c )
target_include_directories( ncpty_test
    PRIVATE
    ${PROJECT_SOURCE_DIR}/deps/greatest
    ${PROJECT_SOURCE_DIR}/include
    )
target_link_libraries( ncpty_test PRIVATE ncpty )

add_test( NAME ncpty_test
    COMMAND ncpty_test
    )

find_program( GCOV gcov REQUIRED )

add_custom_target( coverage
    COMMAND ${GCOV} -i src/bin/banner.c -o ${CMAKE_BINARY_DIR}/src/bin/CMakeFiles/banner.dir/banner.c.
    COMMAND ${GCOV} -i src/lib/ncpty.c -o ${CMAKE_BINARY_DIR}/src/lib/CMakeFiles/ncpty-object.dir/ncpty.c.
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
