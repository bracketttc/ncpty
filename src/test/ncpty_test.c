// Copyright (c) 2020 Timothy Brackett
// Licensed under the MIT license

#include "greatest.h"
#include "ncpty.h"


TEST invalid_status_tests( void )
{
    ASSERT_FALSE( ncpty_status( NULL, NULL ) );

    int code = 0;
    ASSERT_FALSE( ncpty_status( NULL, &code ) );
    ASSERT_EQ( -1, code );

    PASS();
}


TEST invalid_free_test( void )
{
    ncpty_free( NULL );

    struct ncpty_t* pty = NULL;
    ncpty_free( &pty );

    PASS();
}


GREATEST_MAIN_DEFS();

int main( int argc, char** argv )
{
    GREATEST_MAIN_BEGIN();

    RUN_TEST( invalid_status_tests );
    RUN_TEST( invalid_free_test );

    GREATEST_MAIN_END();
}
