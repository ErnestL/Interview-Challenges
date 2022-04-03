#include "solution.h"
#include "records.h"
#include <float.h>
#include <math.h>
#include <pthread.h>

static const char *const positions_file = "./positions.dat";
static struct timespec t0, t1;

static size_t *thread_ids      = NULL;
static uint32_t length         = 0;       /* set in main */
static const uint32_t nthreads = THREADS; /* number of threads */
static pthread_t *thread_array = NULL;    /* pointer to future thread array */

/* reference coordinates supplied as per challenge */
static struct vehicle_ref_coords ref_coords[] = {
    {34.544909F, -102.100843F, FLT_MAX, 0}, {32.345544F, -99.123124F, FLT_MAX, 0},
    {33.234235F, -100.214124F, FLT_MAX, 0}, {35.195739F, -95.348899F, FLT_MAX, 0},
    {31.895839F, -97.789573F, FLT_MAX, 0},  {32.895839F, -101.789573F, FLT_MAX, 0},
    {34.115839F, -100.225732F, FLT_MAX, 0}, {32.335839F, -99.992232F, FLT_MAX, 0},
    {33.535339F, -94.792232F, FLT_MAX, 0},  {32.234235F, -100.222222F, FLT_MAX, 0}};

static struct vehicle_ref_coords coords[THREADS][10] = {{{0.0F, 0.0F, FLT_MAX, 0}}};
//  {0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0}},
// {{0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0},
//  {0.0F, 0.0F, FLT_MAX, 0}}};

static void *search(void *id);

int main(void)
{
    /* Allocate space for thread structs and identifiers */
    thread_array = malloc(nthreads * sizeof(pthread_t));
    thread_ids   = malloc(nthreads * sizeof(long));

    /* begin measuring time taken to load and search */
    START_TIMER(t0)
    number_records = get_records(positions_file, &vehicle_records_ptr, sizeof(struct vehicle_records));

    length = number_records;

    /* Assign each thread an ID and create all the threads */
    for (size_t index = 0; index < nthreads; index++)
    {
        thread_ids[index] = index;
        pthread_create(&thread_array[index], NULL, search, &thread_ids[index]);
    }

    /* Join all the threads. Main will pause in this loop until all threads
     * have returned from the thread function. */
    for (size_t index = 0; index < nthreads; index++)
    {
        pthread_join(thread_array[index], NULL);
    }

    STOP_TIMER(t1)

    /* elapsed time in milliseconds */
    printf("delay(milliseconds): %" PRId64 "\n",
           (int64_t)(t1.tv_sec - t0.tv_sec) * 1000 + ((int64_t)(t1.tv_nsec - t0.tv_nsec) / 1000000));

    free(vehicle_records_ptr);
    free(thread_array);
    free(thread_ids);
    return 0;
}

/*
 * Description:
 * Parameters:
 * Returns:
 */
static float gps_distance(float lat1, float lon1, float lat2, float lon2)
{
    float lat = 0.0F, dx = 0.0F, dy = 0.0F;
    lat = (float)((lat1 + lat2) / 2 * 0.01745);
    dx  = (float)(111.3 * cos(lat) * (lon1 - lon2));
    dy  = (float)(111.3 * (lat1 - lat2));

    return (float)sqrt(dx * dx + dy * dy);
}

/*
 * Description:
 * Parameters:
 * Returns:
 */
static void *search(void *id)
{
    float distance = 0.0f;
    size_t *myid   = (size_t *)id;
    size_t index;

    /* assign each thread its own chunk of elements to process */
    uint32_t chunk = length / nthreads;
    uint32_t start = (uint32_t)*myid * chunk;
    uint32_t end   = start + chunk;

    if (*myid == nthreads - 1)
    {
        end = length;
    }

    /* perform search operation */
    for (index = start; index < end; index++)
    {
        if (*myid == 0)
        {
            for (size_t coords_index = 0; coords_index < 10; coords_index++)
            {
                if ((distance =
                         gps_distance(ref_coords[coords_index].latitute_ref, ref_coords[coords_index].longitude_ref,
                                      vehicle_records_ptr[index].latitute, vehicle_records_ptr[index].longitude)) <
                    coords[0][coords_index].distance)
                {
                    coords[0][coords_index].position_id_nearest = vehicle_records_ptr[index].position_id;
                }
            }
        }
        else
        {
            for (size_t coords_index = 0; coords_index < 10; coords_index++)
            {
                if ((distance =
                         gps_distance(ref_coords[coords_index].latitute_ref, ref_coords[coords_index].longitude_ref,
                                      vehicle_records_ptr[index].latitute, vehicle_records_ptr[index].longitude)) <
                    coords[1][coords_index].distance)
                {
                    coords[1][coords_index].position_id_nearest = vehicle_records_ptr[index].position_id;
                }
            }
        }
    }

    return NULL;
}