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
static off_t get_file_size(const char *const file_name)
{
    struct stat sb;

    if (stat(file_name, &sb) != 0)
    {
        fprintf(stderr, "'stat' failed for '%s': %s.\n", file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return sb.st_size;
}

/*
 *  Description: This routine reads the entire file into memory.
 *  Parameters:
 *  Returns:
 */
uint32_t get_records(const char *file_name, struct vehicle_records **vehicle_records_ptr, const uint32_t record_size)
{
    uint32_t number_records = 0;
    off_t file_size         = 0;
    size_t records_read     = 0;
    FILE *file_ptr          = NULL;

    file_size = get_file_size(file_name);

    if ((file_ptr = fopen(file_name, "rb")) == NULL)
    {
        fprintf(stderr, "Could not open '%s': %s.\n", file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* determine number of record entries in file by looking at the last record number entry */
    if ((fseek(file_ptr, ((int32_t)record_size * -1), SEEK_END)) != 0)
    {
        fprintf(stderr, "Error attempting to move to end of file: '%s': %s.\n", file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }

    fread(&number_records, sizeof(int32_t), 1, file_ptr);

    if ((*vehicle_records_ptr = (struct vehicle_records *)malloc(record_size * number_records)) == NULL)
    {
        fprintf(stderr, "Not enough memory.\n");
        exit(EXIT_FAILURE);
    }

    /* populate structures from beginning of file */
    if ((fseek(file_ptr, 0L, SEEK_SET)) != 0)
    {
        fprintf(stderr, "Error attempting to move to start of file: '%s': %s.\n", file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
    records_read = fread(*vehicle_records_ptr, record_size, (size_t)number_records, file_ptr);

    if ((records_read * record_size) != file_size)
    {
        fprintf(stderr,
                "Short read of '%s': expected %d records "
                "but got %zu: %s.\n",
                file_name, number_records, records_read, strerror(errno));

        exit(EXIT_FAILURE);
    }

    if ((fclose(file_ptr)) != 0)
    {
        fprintf(stderr, "Error closing '%s': %s.\n", file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return number_records;
}