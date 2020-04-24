// Copyright (c) 2020 Timothy Brackett
// Licensed under the MIT license

/// @file banner.c
/// Simple example of ncpty use that wraps a terminal in header and footer
/// banners.

#include "ncpty.h"

#include <errno.h>
#include <fcntl.h>
#include <panel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vterm.h>


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

    // open a pseudoterminal, get master side file descriptor
    int fd_master = posix_openpt( O_RDWR );
    if ( fd_master < 0 )
    {
        fprintf( stderr, "error: Unable to open pty\n" );
        exit( 2 );
    }

    if ( grantpt( fd_master ) != 0 )
    {
        fprintf( stderr, "error: Unable to grant pty\n" );
        exit( 2 );
    }

    if ( unlockpt( fd_master ) != 0 )
    {
        fprintf( stderr, "error: Unable to unlock pty\n" );
        exit( 2 );
    }

    // open slave side file descriptor of pseudoterminal
    int fd_slave = open( ptsname( fd_master ), O_RDWR );
    if ( fd_slave < 0 )
    {
        fprintf( stderr, "error: Unable to get slave pty\n" );
        exit( 2 );
    }

    if ( fork() )
    {
        // parent-side
        close( fd_slave );

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
        // halfdelay();

        int height = LINES;
        int width  = COLS;

        VTerm* vt = vterm_new( height - 2, width );

        // create a window to be used for pty
        WINDOW* pty_win   = newwin( height - 2, width, 1, 0 );
        PANEL*  pty_panel = new_panel( pty_win );

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

        wprintw( pty_win, ptsname( fd_master ) );

        // loop
        do
        {
            update_panels();
            doupdate();
            int c = getch();
            if ( c == 'q' )
            {
                break;
            }
            vterm_keyboard_unichar( vt, c, VTERM_MOD_NONE );
        } while ( true );

        // graceful shutdown
        close( fd_master );
        vterm_free( vt );
        endwin();
    }
    else
    {
        // child-side
        close( fd_master );

        // redirect i/o to pty
        dup2( fd_slave, 0 ); // use pty as stdin
        dup2( fd_slave, 1 ); // use pty as stdout
        dup2( fd_slave, 2 ); // use pty as stderr

        // Note that the argument list reiterates the file being executed
        // execvp( argv[1], argv + 1 )
    }

    return 0;
}
