#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* needed for M_PI */
#define __USE_XOPEN

#define THREADS 2

/* number of supplied samples */
#define SAMPLES 10

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

/* structure to store supplied reference coords */
struct vehicle_ref_coords
{
    float latitute_ref;
    float longitude_ref;
    float distance;
    uint32_t position_id_nearest;
};

/* populated with vehicle data from file */
struct vehicle_records
{
    uint32_t position_id;
    char vehicle_registration[10];
    float latitute;
    float longitude;
    uint64_t timestamp;
} __attribute__((packed)) * vehicle_records_ptr;

/* holds total number records in positions.dat */
uint32_t number_records;

#define SOLUTION_H
#endif