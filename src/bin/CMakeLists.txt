# Copyright (c) 2020 Timothy Brackett
# Licensed under the MIT license

add_executable( banner banner.c )
target_link_libraries( banner
    PRIVATE
    ncpty
    PkgConfig::panel
    PkgConfig::vterm
    )
install( TARGETS banner
    DESTINATION bin
    COMPONENT bin
    )
