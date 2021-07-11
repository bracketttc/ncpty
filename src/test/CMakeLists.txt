# Copyright (c) 2020 Timothy Brackett
# Licensed under the MIT license

# Tests
add_test( NAME banner_usage
    COMMAND banner
    )
set_tests_properties( banner_usage
    PROPERTIES
    PASS_REGULAR_EXPRESSION "Usage: .*"
    LABELS memcheck
    )

add_test( NAME banner_interaction
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/banner_interaction_test.sh $<TARGET_FILE:banner>
    )

add_test( NAME banner_forced_curses
    COMMAND banner -f sleep 1
    )
set_tests_properties( banner_forced_curses
    PROPERTIES
    LABELS memcheck
    )

add_test( NAME banner_return_true
    COMMAND banner true
    )
set_tests_properties( banner_return_true
    PROPERTIES
    LABELS memcheck
    )
add_test( NAME banner_return_false
    COMMAND banner false
    )
set_tests_properties( banner_return_false
    PROPERTIES
    WILL_FAIL TRUE
    LABELS memcheck
    )

add_test( NAME banner_exec_fail
    COMMAND banner 0
    )
set_tests_properties( banner_exec_fail
    PROPERTIES
    WILL_FAIL TRUE
    LABELS memcheck
    )

if( BUILD_TESTING )
    add_executable( ncpty_test ncpty_test.c )
    target_include_directories( ncpty_test
        PRIVATE
        ${PROJECT_SOURCE_DIR}/deps/greatest
        ${PROJECT_SOURCE_DIR}/include
        )
    target_link_libraries( ncpty_test PRIVATE ncpty )
endif()

add_test( NAME ncpty_test
    COMMAND ncpty_test
    )
set_tests_properties( ncpty_test
    PROPERTIES
    LABELS memcheck
    )

find_program( GCOV gcov REQUIRED )
mark_as_advanced( GCOV )

add_custom_target( coverage
    COMMAND ${GCOV} -i src/bin/banner.c -o ${CMAKE_BINARY_DIR}/src/bin/CMakeFiles/banner.dir/banner.c.
    COMMAND ${GCOV} -i src/lib/ncpty.c -o ${CMAKE_BINARY_DIR}/src/lib/CMakeFiles/ncpty-object.dir/ncpty.c.
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
