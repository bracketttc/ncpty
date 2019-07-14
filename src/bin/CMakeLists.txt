add_executable( banner banner.c )
target_link_libraries( banner
    PRIVATE
    ncpty
    )
