// Copyright (c) 2020 Timothy Brackett
// Licensed under the MIT license

/// @file banner.c
/// Simple example of ncpty use that wraps a terminal in header and footer
/// banners.

#include "ncpty.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


void print_usage( const char* name )
{
    printf( "Usage: %s COMMAND [ARGS]\n", name );
}


int main( int argc, char** argv )
{
    if ( argc < 2 )
    {
        print_usage( argv[0] );
        exit( 1 );
    }

    /* ensure banner executable does not have either the set-user-ID bit or the
     * set-group-ID bit set. */
    struct stat statbuf = { 0 };
    if ( 0 != stat( argv[0], &statbuf )
         || ( S_ISUID | S_ISGID ) & statbuf.st_mode )
    {
        fprintf( stderr, "error: this executable cannot be suid or sgid\n" );
        exit( 4 );
    }

    struct ncpty_t* pty = NULL;
    if ( ncpty_execvp( &pty, argv[1], argv + 1 ) )
    {
        fprintf( stderr, "error: Unable to create ncpty\n" );
        exit( 3 );
    }

    // initialize ncurses
    initscr();
    if ( has_colors() )
    {
        start_color();
        if ( can_change_color() )
        {
            // recreate tmux' color table
        }
    }
    raw();
    noecho();
    keypad( stdscr, TRUE );
    halfdelay( 1 );

    int height = LINES;
    int width  = COLS;

    // VTerm* vt = vterm_new( height - 2, width );

    // create a window to be used for pty
    WINDOW* pty_win = newwin( height - 2, width, 1, 0 );
    // PANEL*  pty_panel = new_panel( pty_win );

    // format banner
    const char caption[] = "UNCLASSIFIED";
    const int  len       = strlen( caption );

    init_pair( 1, COLOR_WHITE, COLOR_GREEN );
    attron( COLOR_PAIR( 1 ) );
    wbkgdset( stdscr, COLOR_PAIR( 1 ) );
    move( 0, 0 );
    clrtoeol();
    mvprintw( 0, ( width - len ) / 2, "%s", caption );
    move( height - 1, 0 );
    clrtoeol();
    mvprintw( height - 1, ( width - len ) / 2, "%s", caption );
    attroff( COLOR_PAIR( 1 ) );

    // wprintw( pty_win, ptsname( fd_master ) );

    // loop
    int exit_code = 0;
    do
    {
        // check if child exited
        if ( !ncpty_status( pty, &exit_code ) )
        {
            wprintw( pty_win, "Child process exited!\n" );
            break;
        }

        // process any data from the child

        update_panels();
        doupdate();

        // process any data from user
        int c = getch();
        if ( c == 'q' )
        {
            break;
        }
        // vterm_keyboard_unichar( vt, c, VTERM_MOD_NONE );
    } while ( true );

    ncpty_free( &pty );

    endwin();

    return exit_code;
}
