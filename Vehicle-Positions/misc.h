#include "solution.h"

#ifndef MISC_H

#define MISC_H

uint32_t get_min(struct vehicle_ref_coords coord[THREADS][SAMPLES], uint32_t index, uint32_t num_threads);
float min(float a, float b, uint32_t *pos, uint32_t i);
float gps_distance(float lat1, float lon1, float lat2, float lon2);

#endif