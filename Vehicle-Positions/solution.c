#include "solution.h"
#include "records.h"
#include <float.h>
#include <math.h>
#include <pthread.h>

static const char *const positions_file = "./positions.dat"; /* file containing coordinates */
static struct timespec t0, t1;

static size_t *thread_ids      = NULL;
static uint32_t length         = 0;       /* set in main. Used in thread function */
static const uint32_t nthreads = THREADS; /* number of threads */
static pthread_t *thread_array = NULL;    /* pointer to future thread array */

/* reference coordinates supplied as per challenge */
static struct vehicle_ref_coords ref_coords[] = {
    {34.544909F, -102.100843F, FLT_MAX, 0}, {32.345544F, -99.123124F, FLT_MAX, 0},
    {33.234235F, -100.214124F, FLT_MAX, 0}, {35.195739F, -95.348899F, FLT_MAX, 0},
    {31.895839F, -97.789573F, FLT_MAX, 0},  {32.895839F, -101.789573F, FLT_MAX, 0},
    {34.115839F, -100.225732F, FLT_MAX, 0}, {32.335839F, -99.992232F, FLT_MAX, 0},
    {33.535339F, -94.792232F, FLT_MAX, 0},  {32.234235F, -100.222222F, FLT_MAX, 0}};

static struct vehicle_ref_coords coords[THREADS][10] = {{{0.0F, 0.0F, FLT_MAX},
                                                         {0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0}},
                                                        {{0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0},
                                                         {0.0F, 0.0F, FLT_MAX, 0}}};

static void *search(void *id);
static void print_results();
static void create_threads();

int main(void)
{
    START_TIMER(t0) /* begin measuring time taken to load and search */

    /* retrieve all coordinates from file on disk and copy onto heap for processing */
    length = get_records(positions_file, &vehicle_records_ptr, sizeof(struct vehicle_records));

    create_threads();

    STOP_TIMER(t1)

    printf("delay(milliseconds): %" PRId64 "\n\n", /* elapsed time in milliseconds */
           (int64_t)(t1.tv_sec - t0.tv_sec) * 1000 + ((int64_t)(t1.tv_nsec - t0.tv_nsec) / 1000000));

    print_results(); /* print to console nearest position ID's */

    free(vehicle_records_ptr);
    free(thread_ids);
    free(thread_array);

    return 0;
}

/*
 * Description: calculates distance in kilometers given two lat/lon coordinates
 * Parameter:   lat1 -> latitude from first lat/lon coordinate
 *              lon1 -> longitude from first lat/lon coordinate
 *              lat2 -> latitude  from second lat/lon coordinate
 *              lon2 -> longitude from second lat/lon coordinate
 * Returns: float -> distance in kilometers
 */
float gps_distance(float lat1, float lon1, float lat2, float lon2)
{
    float lat = 0.0F, dx = 0.0F, dy = 0.0F;
    lat = (float)((lat1 + lat2) / 2 * 0.01745);
    dx  = (float)(111.3 * cos(lat) * (lon1 - lon2));
    dy  = (float)(111.3 * (lat1 - lat2));

    return (float)sqrt(dx * dx + dy * dy);
}

/*
 * Description: called by two independent threads. Each of these threads get half of the array
 * to search through i.e. first thread searches first half of array while the second thread
 * searches second half of the arrray
 * Parameters: id -> thread ID
 * Returns: void
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
        for (size_t coords_index = 0; coords_index < SAMPLES; coords_index++)
        {
            if ((distance = gps_distance(ref_coords[coords_index].latitute_ref, ref_coords[coords_index].longitude_ref,
                                         vehicle_records_ptr[index].latitute, vehicle_records_ptr[index].longitude)) <
                coords[*myid][coords_index].distance)
            {
                coords[*myid][coords_index].position_id_nearest = vehicle_records_ptr[index].position_id;
                coords[*myid][coords_index].distance            = distance;
            }
        }
    }

    return NULL;
}

/*
 * Description: prints out the supplied reference coordinates along with the calculated position ID's from
 * records in database of nearest points
 * Parameters: none
 * Returns: void
 */
void print_results()
{
    /* determine the shorter distance stored in each of the threads. The shorter distance is what we want */
    for (size_t index = 0; index < SAMPLES; index++)
    {
        ref_coords[index].position_id_nearest = coords[0][index].distance < coords[1][index].distance
                                                    ? coords[0][index].position_id_nearest
                                                    : coords[1][index].position_id_nearest;
    }

    for (size_t index = 0; index < SAMPLES; index++)
    {
        printf("Position: %zu Latitude: %f Longitude: %f Closest Position Id: %d\n", (index + 1),
               ref_coords[index].latitute_ref, ref_coords[index].longitude_ref, ref_coords[index].position_id_nearest);
    }

    return;
}

/*
 * Description: creates space for specified number of threads specified in global space. If threads are created
 * successfuly they will immediately run.
 * Parameters: none
 * Returns: void
 */
void create_threads()
{
    /* Allocate space for thread structs and identifiers */
    if (((thread_array = malloc(nthreads * sizeof(pthread_t))) == NULL) ||
        ((thread_ids = malloc(nthreads * sizeof(long))) == NULL))
    {
        fprintf(stderr, "Not enough memory.\n");
        exit(EXIT_FAILURE);
    }

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

    return;
}