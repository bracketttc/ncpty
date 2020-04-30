// Copyright (c) 2020 Timothy Brackett
// Licensed under the MIT license

#include "ncpty.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

int ncpty_execvp( struct ncpty_t** pty, const char* file, char* const argv[] )
{
    *pty = (struct ncpty_t*)malloc( sizeof( struct ncpty_t ) );

    // open a pseudoterminal, get master side file descriptor
    ( *pty )->fd = posix_openpt( O_RDWR );
    if ( ( *pty )->fd < 0 )
    {
        fprintf( stderr, "error: Unable to open pty\n" );
        return -1;
    }

    if ( grantpt( ( *pty )->fd ) != 0 )
    {
        fprintf( stderr, "error: Unable to grant pty\n" );
        return -1;
    }

    if ( unlockpt( ( *pty )->fd ) != 0 )
    {
        fprintf( stderr, "error: Unable to unlock pty\n" );
        return -1;
    }

    // open child side file descriptor of pseudoterminal
    int fd_child = open( ptsname( ( *pty )->fd ), O_RDWR );
    if ( fd_child < 0 )
    {
        fprintf( stderr, "error: Unable to open child pty\n" );
        return -1;
    }

    if ( fork() )
    {
        // parent-side
        close( fd_child );
    }
    else
    {
        // child-side
        close( ( *pty )->fd );

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


void ncpty_free( struct ncpty_t** pty )
{
    if ( !pty || !*pty )
    {
        return;
    }

    close( ( *pty )->fd );
    //vterm_free( ( *pty )->vt );

    free( *pty );
    *pty = NULL;
}
