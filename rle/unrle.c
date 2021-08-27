/**************************************************************************
 * @file unrle.c
 * @author Jeremy Brubaker
 * @brief Implement basic Run-Length un-encoding
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

int
main (int argc, char **argv)
{
    /* Save argv[0] for later */
    char *progname = argv[0];

    FILE *fp = NULL;

    if (argc != 2)
    {
        fprintf (stderr, "%s: %s\n", progname, HELP_MSG);
        exit (1);
    }
    else
    {
        if (!(fp = fopen (argv[1], "r")))
        {
            fprintf (stderr, "%s: %s: %s\n", progname, argv[1], FOPEN_ERR);
            exit (2);
        }
    }

    char b = 0;
    uint32_t n = 1;
    /* Get the count */
    while (fread (&n, sizeof n, 1, fp))
    {
        /* Get the character */
        fread (&b, sizeof b, 1, fp);
        /* Print b n times */
        for (int i = 0; i < n; i++)
            fwrite (&b, sizeof b, 1, stdout);
    }

    return 0;
}
