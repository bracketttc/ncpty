// Copyright (c) 2020 Timothy Brackett
// Licensed under the MIT license

#ifndef NCPTY_H_INCLUDED
#define NCPTY_H_INCLUDED

/// @file ncpty.h
/// Public header for ncpty - the curses pseudoterminal.

#include <ncurses.h>
#include <panel.h>
#include <sys/types.h>
#include <vterm.h>

// Need to define a clear and concise API for dealing with creating a pty and
// attaching it to an ncurses window

typedef struct ncpty_t
{
    int     fd;     ///< File descriptor for master-side of pseudoterminal
    pid_t   pid;    ///< Process id
    VTerm*  vt;     ///< Virtual terminal pointer
    PANEL*  panel;  ///< ncurses panel
    WINDOW* window; ///< ncurses window
} ncpty;


/// @brief Create a pseudoterminal, fork, exec and attach a child process to
/// the child-side.
///
/// @param[out] pty ncpty object
/// device
/// @param[in]  file filename of file to executed (will search on $PATH)
/// @param[in]  argv array of null-terminated string arguments, terminated by a
/// null pointer
/// @returns 0 for success, -1 for execvp error. errno is set to indicate the
/// specific error encountered.
int ncpty_execvp( struct ncpty_t** pty, const char* file, char* const argv[] );

/// @brief Check if child process controlled by ncpty object has exited.
///
/// @param[in] pty ncpty object
/// @param[out] exit_code
/// @returns @c true if process is still running, @c false otherwise
bool ncpty_status( struct ncpty_t* pty, int* exit_code );

/// @brief Deallocate an ncpty object.
///
/// @param[in] pty ncpty object to free
void ncpty_free( struct ncpty_t** pty );

#endif // NCPTY_H_INCLUDED
