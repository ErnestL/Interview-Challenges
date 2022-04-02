#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef SOLUTION_H

#define START_TIMER(t0)                                                                                                \
    if (timespec_get(&t0, TIME_UTC) != TIME_UTC)                                                                       \
    {                                                                                                                  \
        exit(EXIT_FAILURE);                                                                                            \
    }

#define STOP_TIMER(t1)                                                                                                 \
    if (timespec_get(&t1, TIME_UTC) != TIME_UTC)                                                                       \
    {                                                                                                                  \
        exit(EXIT_FAILURE);                                                                                            \
    }

// structure to store reference coords
struct vehicle_ref_coords
{
    const float latitute_ref;
    const float longitude_ref;
    float distance;
    int32_t position_id_nearest;
};

// structure to store vehicle stats
struct vehicle_records
{
    int32_t position_id;
    char vehicle_registration[10];
    float latitute;
    float longitude;
    int64_t timestamp;
} __attribute__((packed)) * vehicle_records_ptr;

// holds total number records in positions.dat
uint32_t number_records = 0;

#define SOLUTION_H
#endif