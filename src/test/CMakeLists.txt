# Copyright (c) 2020 Timothy Brackett
# Licensed under the MIT license

# Tests
add_test( NAME banner_suid
    COMMAND banner /bin/bash
    )
set_tests_properties( banner_suid
    PROPERTIES
    TIMEOUT 1
    WILL_FAIL TRUE
    DISABLED TRUE
    )
add_test( NAME banner_sgid
    COMMAND banner /bin/bash
    )
set_tests_properties( banner_sgid
    PROPERTIES
    TIMEOUT 1
    WILL_FAIL TRUE
    DISABLED TRUE
    )

add_test( NAME banner_usage
    COMMAND banner
    )
set_tests_properties( banner_usage
    PROPERTIES
    PASS_REGULAR_EXPRESSION "Usage: .*"
    )

#add_test( NAME banner_interaction
#    )

add_test( NAME banner_return_true
    COMMAND banner true
    )
add_test( NAME banner_return_false
    COMMAND banner false
    )
set_tests_properties( banner_return_false
    PROPERTIES
    WILL_FAIL TRUE
    )

#add_test( NAME banner_exec_fail
#    )

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
mark_as_advanced( GCOV )

add_custom_target( coverage
    COMMAND ${GCOV} -i src/bin/banner.c -o ${CMAKE_BINARY_DIR}/src/bin/CMakeFiles/banner.dir/banner.c.
    COMMAND ${GCOV} -i src/lib/ncpty.c -o ${CMAKE_BINARY_DIR}/src/lib/CMakeFiles/ncpty-object.dir/ncpty.c.
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
