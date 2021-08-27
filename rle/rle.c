/**************************************************************************
 * @file rle.c
 * @author Jeremy Brubaker
 * @brief Implement basic Run-Length encoding
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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char HELP_MSG[] = "[file]";
const char FOPEN_ERR[] = "No such file or directory";
char ARG_STDIN[] = "-";

int
main (int argc, char **argv)
{
    /* Save argv[0] for later */
    char *progname = argv[0];

    FILE *fp = NULL;

    if (argc == 1)
    {
        fp = stdin;
    }
    else if (argc == 2)
    {
        if (!(fp = fopen (argv[1], "r")))
        {
            fprintf (stderr, "%s: %s: %s\n", progname, argv[1], FOPEN_ERR);
            exit (2);
        }
    }
    else
    {
        fprintf (stderr, "%s: %s\n", progname, HELP_MSG);
        exit (1);
    }

    char b = 0;
    char prev = 0;
    uint32_t n = 1; /* start with 1 instance */

    /* Get the first character */
    fread (&prev, sizeof prev, 1, fp);
    /* Get the next character */
    while (fread (&b, sizeof b, 1, fp))
    {
        /* If current character equals the
         * last one, just increment the counter */
        if (b == prev)
        {
            n++;
        }
        /* If current character is different,
         * write the count and the previous character */
        else
        {
            fwrite (&n, sizeof n, 1, stdout);
            fwrite (&prev, sizeof prev, 1, stdout);
            n = 1;
            prev = b;
        }
    }
    /* Write the last character because
     * the loop exits without doing it */
    fwrite (&n, sizeof n, 1, stdout);
    fwrite (&prev, sizeof prev, 1, stdout);

    return 0;
}
