#include "records.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/*
 *  Description: This routine returns the size of the file it is called with.
 *  Parameters:
 *  Returns:
 */
off_t Get_File_Size(const char *const file_name)
{
    struct stat sb;

    if (stat(file_name, &sb) != 0)
    {
        fprintf(stderr, "'stat' failed for '%s': %s.\n", file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return sb.st_size;
}