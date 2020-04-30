# Copyright (c) 2020 Timothy Brackett
# Licensed under the MIT license

add_test( NAME banner_usage
    COMMAND banner
    )
set_tests_properties( banner_usage
    PROPERTIES WILL_FAIL TRUE
    )

add_test( NAME banner_true
    COMMAND banner true
    )

add_test( NAME banner_false
    COMMAND banner false
    )
set_tests_properties( banner_false
    PROPERTIES WILL_FAIL TRUE
    )

add_test( NAME banner_exec_fail
    COMMAND banner a.out
    )
set_tests_properties( banner_exec_fail
    PROPERTIES WILL_FAIL TRUE
    )

add_test( NAME banner_sleep
    COMMAND banner sleep 1
    )

find_program( GCOV gcov REQUIRED )

add_custom_target( coverage
    COMMAND ${GCOV} -i src/bin/banner.c -o ${CMAKE_BINARY_DIR}/src/bin/CMakeFiles/banner.dir/banner.c.
    COMMAND ${GCOV} -i src/lib/ncpty.c -o ${CMAKE_BINARY_DIR}/src/lib/CMakeFiles/ncpty-object.dir/ncpty.c.
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
