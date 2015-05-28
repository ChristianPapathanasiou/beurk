/*
 * BEURK is an userland rootkit for GNU/Linux, focused around stealth.
 * Copyright (C) 2015  unix-thrust
 *
 * This file is part of BEURK.
 *
 * BEURK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BEURK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BEURK.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifndef _DEBUG_H_
# define _DEBUG_H_

# include <stdlib.h>
# include <unistd.h> /* write() */
# include <stdio.h> /* FILE, perror(), fclose(), fprintf() */
# include <stdarg.h> /* va_list, va_start(), va_end() */
# include "config.h"


# if DEBUG_LEVEL > 0
typedef enum    dbg_type {
    D_INFO,
    D_ERROR,
}               e_dbg_type;

static FILE     *get_debug_file(void) {
    const char  *debug_file_name;
    FILE        *debug_file;

    if ((debug_file_name = getenv("BEURK_DEBUG_FILE")) != NULL)
        debug_file = REAL_FOPEN(debug_file_name, "a");
    else
        debug_file = REAL_FOPEN(DEBUG_FILE, "a");

    if (debug_file == NULL) {
        write(STDERR_FILENO, "\n[BEURK]: FATAL ERROR: ", 23);
        perror("cannot open DEBUG_FILE");
    }

    return (debug_file);
}
# else /* debug disabled */
#  define DEBUG(...) ((void)0)
# endif

# if DEBUG_LEVEL == 1 /* debug basic */
static void debug_basic(e_dbg_type flag, const char *fmt, ...) {
    FILE    *debug_file;
    va_list ap;

    if (!(debug_file = get_debug_file()))
        return ;

    if (flag == D_INFO)
        fprintf(debug_file, "[BEURK_INFO]: ");
    else if (flag == D_ERROR)
        fprintf(debug_file, "[BEURK_ERROR]: ");
    else
        fprintf(debug_file, "[BEURK_UNKNOWN]: ");

    va_start(ap, fmt);
    vfprintf(debug_file, fmt, ap);
    va_end(ap);
    fprintf(debug_file, "\n");
    fflush(debug_file);
    fclose(debug_file);
}
#  define DEBUG(...) (debug_basic(__VA_ARGS__))

# elif DEBUG_LEVEL == 2 /* debug high (with file and line numbers) */
static void debug_high(const char *f, int l, e_dbg_type flag, char *fmt, ...) {
    FILE    *debug_file;
    va_list ap;

    if (!(debug_file = get_debug_file()))
        return ;

    if (flag == D_INFO)
        fprintf(debug_file, "[BEURK_INFO<%s:%d>]: ", f, l);
    else if (flag == D_ERROR)
        fprintf(debug_file, "[BEURK_ERROR<%s:%d>]: ", f, l);
    else
        fprintf(debug_file, "[BEURK_UNKNOWN<%s:%d>]: ", f, l);

    va_start(ap, fmt);
    vfprintf(debug_file, fmt, ap);
    va_end(ap);
    fprintf(debug_file, "\n");
    fflush(debug_file);
    fclose(debug_file);
}
#  define DEBUG(...) (debug_high(__FILE__, __LINE__, __VA_ARGS__))
# endif /* DEBUG_LEVEL */

#endif /* _DEBUG_H_ */
