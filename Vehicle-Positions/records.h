#include <stdint.h>
#include <sys/types.h>

#ifndef FILE_H
#define FILE_H

struct vehicle_records;
uint32_t get_records(const char *file_name, struct vehicle_records **vehicle_record_ptr, const uint32_t record_size);

#endif