
/**
* @file example.c
*/
/*
 * Copyright (C) 2004-2005 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */
#include <libstdf.h>

int main(int argc, char *argv[])
{
    stdf_file *f;
    rec_unknown *r;

    if (argc != 2)
        return EXIT_FAILURE;

    f = stdf_open(argv[1]);
    if (!f)
        return EXIT_FAILURE;

    while ((r=stdf_read_record(f)) != NULL) {
        stdf_free_record(r);
    }

    stdf_close(f);

    return EXIT_SUCCESS;
}
