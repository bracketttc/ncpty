# Copyright (c) 2020 Timothy Brackett
# Licensed under the MIT license

add_test( NAME banner_usage
    COMMAND banner
    )
set_tests_properties( banner_usage
    PROPERTIES WILL_FAIL TRUE
    )

