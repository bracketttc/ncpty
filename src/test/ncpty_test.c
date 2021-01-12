// Copyright (c) 2020 Timothy Brackett
// Licensed under the MIT license

#include "greatest.h"
#include "ncpty.h"
#include <sys/resource.h>
#include <sys/queue.h>
#include <sys/time.h>


TEST invalid_status_tests( void )
{
    ASSERT_FALSE( ncpty_status( NULL, NULL ) );

    int code = 0;
    ASSERT_FALSE( ncpty_status( NULL, &code ) );
    ASSERT_EQ( -1, code );

    PASS();
}


// This test does not report failure, but provides coverage of a path
// that would otherwise result in memory corruption.
TEST invalid_free_test( void )
{
    ncpty_free( NULL );

    PASS();
}


// reduce the limit on the number of concurrent processes for this program, then fork until we hit that limit
TEST fork_fail_test( void )
{
    struct rlimit nproc_limit = { 0 };
    getrlimit( RLIMIT_NPROC, &nproc_limit );

    struct rlimit nproc_new_limit = nproc_limit;
    nproc_new_limit.rlim_cur = 10;
    setrlimit( RLIMIT_NPROC, &nproc_new_limit );

    char* file = "sleep";
    char* arg = "100";
    char* const argv[3] = { file, arg, NULL };

    // initialize ncurses
    initscr();

    LIST_HEAD( listhead, entry ) head = LIST_HEAD_INITIALIZER( head );

    struct entry {
        LIST_ENTRY( entry ) entries;

        WINDOW* win;
        PANEL* panel;
        struct ncpty_t* pty;
    };

    LIST_INIT( &head );

    int count = 0;

    // allocate new pseudoterminals until we run out
    while ( true )
    {
        struct entry* node = (struct entry*)malloc( sizeof(struct entry) );
        if ( !node )
        {
            ASSERT( false );
            break;
        }
        memset( node, 0, sizeof(struct entry) );

        LIST_INSERT_HEAD( &head, node, entries );

        node->win = newwin( 0, 0, 0, 0 );
        if ( !node->win )
        {
            ASSERT( false );
            break;
        }
        node->panel = new_panel( node->win );
        if ( !node->panel )
        {
            ASSERT( false );
            break;
        }
        node->pty = ncpty_new( node->panel );
        if ( !node->pty )
        {
            ASSERT( false );
            // allocation failure
            break;
        }
        if ( 0 != ncpty_execvp( node->pty, file, argv ) )
        {
            ASSERT( true );
            break;
        }
        ++count;
    }

    // clean up
    while ( !LIST_EMPTY( &head ) )
    {
        struct entry* node = LIST_FIRST( &head );

        ncpty_free( node->pty );
        node->pty = NULL;

        del_panel( node->panel );
        node->panel = NULL;

        delwin( node->win );
        node->win = NULL;

        LIST_REMOVE( node, entries );

        free( node );
    }

    endwin();

    // reset process limit
    setrlimit( RLIMIT_NPROC, &nproc_limit );

    fprintf( stderr, "\nCreated %d pseudoterminals\n", count );

    PASS();
}


// spawn pseudoterminals and fork until we run out of available ptys
TEST resource_starvation_test( void )
{
    char* file = "true";
    char* const argv[2] = { file, NULL };

    // initialize ncurses
    initscr();
    if ( has_colors() )
    {
        start_color();
    }
    raw();
    noecho();
    keypad( stdscr, TRUE );
    halfdelay( 1 );

    LIST_HEAD( listhead, entry ) head = LIST_HEAD_INITIALIZER( head );

    struct entry {
        LIST_ENTRY( entry ) entries;

        WINDOW* win;
        PANEL* panel;
        struct ncpty_t* pty;
    };

    LIST_INIT( &head );

    int count = 0;

    // allocate new pseudoterminals until we run out
    while ( true )
    {
        struct entry* node = (struct entry*)malloc( sizeof(struct entry) );
        if ( !node )
        {
            ASSERT( false );
            break;
        }
        memset( node, 0, sizeof(struct entry) );

        LIST_INSERT_HEAD( &head, node, entries );

        node->win = newwin( 0, 0, 0, 0 );
        if ( !node->win )
        {
            ASSERT( false );
            break;
        }
        node->panel = new_panel( node->win );
        if ( !node->panel )
        {
            ASSERT( false );
            break;
        }
        node->pty = ncpty_new( node->panel );
        if ( !node->pty )
        {
            ASSERT( false );
            // allocation failure
            break;
        }
        if ( 0 != ncpty_execvp( node->pty, file, argv ) )
        {
            ASSERT( true );
            break;
        }
        ++count;
    }

    // clean up
    while ( !LIST_EMPTY( &head ) )
    {
        struct entry* node = LIST_FIRST( &head );

        ncpty_free( node->pty );
        node->pty = NULL;

        del_panel( node->panel );
        node->panel = NULL;

        delwin( node->win );
        node->win = NULL;

        LIST_REMOVE( node, entries );

        free( node );
        node = NULL;
    }

    endwin();

    fprintf( stderr, "\nCreated %d pseudoterminals\n", count );

    PASS();
}


GREATEST_MAIN_DEFS();

int main( int argc, char** argv )
{
    GREATEST_MAIN_BEGIN();

    int seed = rand();

    SHUFFLE_TESTS( seed, {
        RUN_TEST( fork_fail_test );
        RUN_TEST( invalid_free_test );
        RUN_TEST( invalid_status_tests );
        RUN_TEST( resource_starvation_test );
        } );

    GREATEST_PRINT_REPORT();
    ncpty_exit( greatest_all_passed() ? EXIT_SUCCESS : EXIT_FAILURE );
}
