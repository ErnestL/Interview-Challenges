#include "misc.h"
#include <math.h>

/*
 * Description: returns minimum float value in an array
 * Parameters: array -> unordered float array, n -> number of items in float array
 * Returns: minimum value in float array
 */
uint32_t get_min(struct vehicle_ref_coords coord[THREADS][SAMPLES], uint32_t index, uint32_t num_threads)
{
    uint32_t small_index_pos = 0;

    float res = coord[0][index].distance;

    for (size_t loop = 1; loop < num_threads; loop++)
        res = min(res, coord[loop][index].distance, &small_index_pos, (uint32_t)loop);

    return coord[small_index_pos][index].position_id_nearest;
}

/*
 * Description: compares two floating point numbers and returns the snaller of the two
 * Parameters: a -> first value to be compared, b -> second value to be compared
 * Returns: minimum value of two floats
 */
float min(float a, float b, uint32_t *pos, uint32_t i)
{
    return (a > b) ? *pos = i, b : a;
}

/*
 * Description: calculates distance in kilometers given two lat/lon coordinates
 * Parameter:   lat1 -> latitude from first lat/lon coordinate
 *              lon1 -> longitude from first lat/lon coordinate
 *              lat2 -> latitude  from second lat/lon coordinate
 *              lon2 -> longitude from second lat/lon coordinate
 * Returns: float -> distance in kilometers
 */
inline float gps_distance(float lat1, float lon1, float lat2, float lon2)
{
    float lat = 0.0F, dx = 0.0F, dy = 0.0F;
    lat = (float)((lat1 + lat2) / 2 * 0.01745);
    dx  = (float)(111.3 * cos(lat) * (lon1 - lon2));
    dy  = (float)(111.3 * (lat1 - lat2));

    return (float)sqrt(dx * dx + dy * dy);
}