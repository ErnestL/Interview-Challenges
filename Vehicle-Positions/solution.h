#include <errno.h>
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

#define SOLUTION_H
#endif