/**************************************************************************
 * @file grep.c
 * @author Jeremy Brubaker
 * @brief grep(1)
 *
 * \bug {
 * - Does not implement any command line options
 * - Does not implement POSIX internationalization
 * }
 *
 * This file is part of XXX
 *
 * Copyright (c) 2020, Jeremy Brubaker
 ***************************************************************************
 * XXX is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XXX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XXX.  If not, see <https://www.gnu.org/licenses/>.
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char HELP_MSG[] = "searchterm [file ...]";
const char FOPEN_ERR[] = "No such file or directory";
char ARG_STDIN[] = "-";

int
main (int argc, char **argv)
{
    /* Save argv[0] for later */
    char *progname = argv[0];

    /* Print help message if no search pattern passed */
    if (argc == 1)
    {
        fprintf (stderr, "%s: %s\n", progname, HELP_MSG);
        exit (2);
    }

    char *line = NULL;
    size_t n = 0;
    FILE *fp = NULL;
    char *pattern = argv[1];
    char **f = argv + 2; /* filenames start at argv[2] */

    /* If no filenames passed, insert "-" for stdin at argv[1] */
    if (argc == 2)
    {
        argv[1] = ARG_STDIN;
        f = argv + 1;
    }

    /* Loop through filenames */
    for (; *f; *f++)
    {
        /* "-" */
        if (strcmp (*f, ARG_STDIN) == 0)
            fp = stdin;
        /* filename */
        else if (!(fp = fopen (*f, "r")))
        {
            fprintf (stderr, "%s: %s: %s\n", progname, *f, FOPEN_ERR);
            continue;
        }

        while (getline (&line, &n, fp) != -1)
        {
            /* argc > 3 means multiple filenames were passed
             * so print which file the pattern was found in */
            if (strstr (line, pattern))
            {
                if (argc > 3)
                    printf ("%s:%s", fp == stdin ? "(standard input)" : *f, line);
                else
                    printf ("%s", line);
            }
        }

        /* Reset EOF on stdin in case it is passed multiple times */
        if (fp == stdin)
            clearerr (stdin);
    }

    free (line);

    return 0;
}
