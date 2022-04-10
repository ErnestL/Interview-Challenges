#include "solution.h"
#include "misc.h"
#include "records.h"
#include <float.h>

static const char *const positions_file = "./positions.dat"; /* file containing coordinates */

/* reference coordinates supplied as per challenge */
struct vehicle_ref_coords ref_coords[] = {
    {34.544909F, -102.100843F, FLT_MAX, 0}, {32.345544F, -99.123124F, FLT_MAX, 0},
    {33.234235F, -100.214124F, FLT_MAX, 0}, {35.195739F, -95.348899F, FLT_MAX, 0},
    {31.895839F, -97.789573F, FLT_MAX, 0},  {32.895839F, -101.789573F, FLT_MAX, 0},
    {34.115839F, -100.225732F, FLT_MAX, 0}, {32.335839F, -99.992232F, FLT_MAX, 0},
    {33.535339F, -94.792232F, FLT_MAX, 0},  {32.234235F, -100.222222F, FLT_MAX, 0}};

struct vehicle_ref_coords coords[THREADS][10];

static void print_results();
extern void create_threads();

int main(void)
{
    START_TIMER(t0) /* begin measuring time taken to load and search */

    initArray(coords);

    /* retrieve all coordinates from file on disk and copy onto heap for processing */
    length = get_records(positions_file, &vehicle_records_ptr, sizeof(struct vehicle_records));

    create_threads();

    STOP_TIMER(t1)

    printf("delay(milliseconds): %" PRId64 "\n\n", /* elapsed time in milliseconds */
           (int64_t)(t1.tv_sec - t0.tv_sec) * 1000 + ((int64_t)(t1.tv_nsec - t0.tv_nsec) / 1000000));

    print_results(); /* print to console nearest position ID's */

    free(vehicle_records_ptr);

    return 0;
}

/*
 * Description: prints out the supplied reference coordinates along with the calculated position ID's from
 * records in database of nearest points
 * Parameters: none
 * Returns: void
 */
static void print_results()
{
    for (size_t index = 0; index < SAMPLES; index++)
    {
        ref_coords[index].position_id_nearest = get_min(coords, (uint32_t)index, THREADS);
    }

    for (size_t index = 0; index < SAMPLES; index++)
    {
        printf("Position: %zu Latitude: %f Longitude: %f Closest Position Id: %d\n", (index + 1),
               ref_coords[index].latitute_ref, ref_coords[index].longitude_ref, ref_coords[index].position_id_nearest);
    }

    return;
}
