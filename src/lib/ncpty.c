// Copyright (c) 2020 Timothy Brackett
// Licensed under the MIT license

#include "ncpty.h"

#include <curses.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>


struct ncpty_t* ncpty_new( PANEL* panel )
{
    struct ncpty_t* pty = (struct ncpty_t*)malloc( sizeof( struct ncpty_t ) );
    if ( pty )
    {
        memset( pty, 0, sizeof( struct ncpty_t ) );

        pty->fd    = -1;
        pty->vt    = NULL;
        pty->panel = panel;
    }
    return pty;
}


void ncpty_free( struct ncpty_t* pty )
{
    if ( !pty )
    {
        return;
    }

    if ( pty->fd >= 0 )
    {
        close( pty->fd );
        pty->fd = -1;
    }
    if ( pty->vt )
    {
        vterm_free( pty->vt );
        pty->vt = NULL;
    }
    pty->panel = NULL;

    free( pty );
}


int ncpty_execvp( struct ncpty_t* pty, const char* file, char* const argv[] )
{
    if ( !pty )
    {
        // no pty provided
        return -1;
    }

    if ( !pty->panel || !panel_window( pty->panel ) )
    {
        // pty does not have a PANEL or PANEL does not have an associated WINDOW
        return -1;
    }

    if ( pty->fd >= 0 || pty->vt )
    {
        // pty already in use, or in undefined state
        return -1;
    }

    // TODO use forkpty instead of strict POSIX method below

    // open a pseudoterminal, get master side file descriptor
    pty->fd = posix_openpt( O_RDWR );
    if ( pty->fd < 0 )
    {
        fprintf( stderr, "error: Unable to open pty\n" );
        return -1;
    }

    if ( grantpt( pty->fd ) != 0 )
    {
        // probably unreachable, the error conditions for grantpt appear to be
        // limited to invalid file descriptor arguments
        fprintf( stderr, "error: Unable to grant pty\n" );
        close( pty->fd );
        pty->fd = -1;
        return -1;
    }

    if ( unlockpt( pty->fd ) != 0 )
    {
        // probably unreachable, the error conditions for unlockpt appear to be
        // limited to invalid file descriptor arguments
        fprintf( stderr, "error: Unable to unlock pty\n" );
        close( pty->fd );
        pty->fd = -1;
        return -1;
    }

    // open child side file descriptor of pseudoterminal
    int fd_child = open( ptsname( pty->fd ), O_RDWR );
    if ( fd_child < 0 )
    {
        fprintf( stderr, "error: Unable to open child pty\n" );
        close( pty->fd );
        pty->fd = -1;
        return -1;
    }

    pid_t pid = fork();
    if ( pid == -1 )
    {
        // fork failed
        fprintf( stderr, "error: fork failed\n" );
        close( fd_child );
        close( pty->fd );
        pty->fd = -1;
        return -1;
    }

    if ( pid )
    {
        // parent-side
        close( fd_child );

        pty->pid    = pid;
        WINDOW* win = panel_window( pty->panel );
        int     x   = 0;
        int     y   = 0;
        getmaxyx( win, y, x );
        pty->vt = vterm_new( y, x );
    }
    else
    {
        // child-side
        close( pty->fd );

        struct termios old_term_settings;
        tcgetattr( fd_child, &old_term_settings );

        struct termios new_term_settings = old_term_settings;
        cfmakeraw( &new_term_settings );
        tcsetattr( fd_child, TCSANOW, &new_term_settings );

        // redirect i/o to pty
        dup2( fd_child, 0 ); // use pty as stdin
        dup2( fd_child, 1 ); // use pty as stdout
        dup2( fd_child, 2 ); // use pty as stderr

        // no longer needed
        close( fd_child );

        setsid();

        ioctl( 0, TIOCSCTTY, 1 );

        return execvp( file, argv );
    }
    return 0;
}


bool ncpty_status( struct ncpty_t* pty, int* exit_code )
{
    if ( !pty )
    {
        // no pty provided
        if ( exit_code )
        {
            *exit_code = -1;
        }
        return false;
    }

    siginfo_t siginfo = { 0 };
    if ( 0 == waitid( P_PID, pty->pid, &siginfo, WEXITED | WNOHANG | WNOWAIT )
         && siginfo.si_pid == pty->pid )
    {
        // process exited
        if ( exit_code )
        {
            *exit_code = siginfo.si_status;
        }
        return false;
    }

    return true;
}


void ncpty_exit( int exit_code )
{
#if defined(NCURSES_VERSION_PATCH) && NCURSES_VERSION_PATCH >= 20191214
    exit_curses( exit_code );
#else
    exit( exit_code );
#endif
}
