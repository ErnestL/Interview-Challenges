#include <stdint.h>
#include <sys/types.h>

#ifndef FILE_H
#define FILE_H

struct vehicle_records;
off_t Get_File_Size(const char *const file_name);
uint32_t Get_Records(const char *file_name, struct vehicle_records **vehicle_record_ptr, const uint32_t record_size);

#endif