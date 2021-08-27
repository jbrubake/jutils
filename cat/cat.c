/**************************************************************************
 * @file cat.c
 * @author Jeremy Brubaker
 * @brief cat(1)
 *
 * \bug {
 * - Does not conform to POSIX internationalization requirements
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

char ARG_U[] = "-u";
char ARG_STDIN[] = "-";
short FAST_OUTPUT = 0;

const char FOPEN_ERR[] = "No such file or directory";

int
main (int argc, char **argv)
{
    char **f;
    char *progname = *argv; /* save program name */
    argv[0] = ARG_STDIN;    /* set first arg to stdin just in case */

    /* If there are 1+ args, search for "-u" 
     * and detect if we are assuming stdin as an input */
    if (argc > 1)
    {
        /* Start with argv[1] */
        for (f = argv + 1; *f; *f++)
        {
            if (strcmp (*f, ARG_U) == 0)
            {
                /* Unbuffer stdout */
                setvbuf (stdout, NULL, _IONBF, 0);
                FAST_OUTPUT = 1;
            }
            else
            {
                /* At least one arg is *not* "-u" so skip
                 * the "just in case" argv[0] */
                argv++;
            }
        }
    }

    char *line = NULL;
    size_t n   = 0;
    FILE *fp   = NULL;

    /* Loop through argv */
    for (f = argv; *f; *f++)
    {
        /* "-" */
        if (strcmp (*f, ARG_STDIN) == 0)
            fp = stdin;
        /* Ignore "-u" */
        else if (strcmp (*f, ARG_U) == 0)
            continue;
        /* Filename */
        else
           if (!(fp = fopen (*f, "r")))
           {
               fprintf (stderr, "%s: %s: %s\n", progname, *f, FOPEN_ERR);
               continue;
           }

        if (FAST_OUTPUT)
        {
            int c;
            while ((c = fgetc (fp)) != EOF)
                fputc (c, stdout);
            fputc (EOF, stdout); /* Must explicitly print EOF */
        }
        else
        {
            while (getline (&line, &n, fp) != -1)
                printf ("%s", line);
        }

        /* Reset EOF on stdin in case it was requested again */
        if (fp == stdin)
            clearerr (stdin);
    }

    free(line);

    return 0;
}
