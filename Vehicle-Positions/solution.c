#include "solution.h"
#include "records.h"
#include <float.h>
#include <math.h>
#include <pthread.h>

static const char *positions_file = "./positions.dat";
struct timespec t0, t1;

long *array;             // allocated in main
long length   = 2000000; // set in main (1 billion)
long nthreads = 2;       // number of threads
long s;                  // scalar

pthread_t *thread_array; // pointer to future thread array
long *thread_ids;

// reference coordinates supplied as per challenge.
static struct vehicle_ref_coords ref_coords[] = {
    {34.544909F, -102.100843F, FLT_MAX, 0}, {32.345544F, -99.123124F, FLT_MAX, 0},
    {33.234235F, -100.214124F, FLT_MAX, 0}, {35.195739F, -95.348899F, FLT_MAX, 0},
    {31.895839F, -97.789573F, FLT_MAX, 0},  {32.895839F, -101.789573F, FLT_MAX, 0},
    {34.115839F, -100.225732F, FLT_MAX, 0}, {32.335839F, -99.992232F, FLT_MAX, 0},
    {33.535339F, -94.792232F, FLT_MAX, 0},  {32.234235F, -100.222222F, FLT_MAX, 0}};

static struct vehicle_ref_coords_test coords[2][10] = {{{34.544909F, -102.100843F, FLT_MAX, 0},
                                                        {32.345544F, -99.123124F, FLT_MAX, 0},
                                                        {33.234235F, -100.214124F, FLT_MAX, 0},
                                                        {35.195739F, -95.348899F, FLT_MAX, 0},
                                                        {31.895839F, -97.789573F, FLT_MAX, 0},
                                                        {32.895839F, -101.789573F, FLT_MAX, 0},
                                                        {34.115839F, -100.225732F, FLT_MAX, 0},
                                                        {32.335839F, -99.992232F, FLT_MAX, 0},
                                                        {33.535339F, -94.792232F, FLT_MAX, 0},
                                                        {32.234235F, -100.222222F, FLT_MAX, 0}},
                                                       {{34.544909F, -102.100843F, FLT_MAX, 0},
                                                        {32.345544F, -99.123124F, FLT_MAX, 0},
                                                        {33.234235F, -100.214124F, FLT_MAX, 0},
                                                        {35.195739F, -95.348899F, FLT_MAX, 0},
                                                        {31.895839F, -97.789573F, FLT_MAX, 0},
                                                        {32.895839F, -101.789573F, FLT_MAX, 0},
                                                        {34.115839F, -100.225732F, FLT_MAX, 0},
                                                        {32.335839F, -99.992232F, FLT_MAX, 0},
                                                        {33.535339F, -94.792232F, FLT_MAX, 0},
                                                        {32.234235F, -100.222222F, FLT_MAX, 0}}};

void *scalar_multiply(void *id);

int main(void)
{
    // Allocate space for thread structs and identifiers.
    thread_array = malloc(nthreads * sizeof(pthread_t));
    thread_ids   = malloc(nthreads * sizeof(long));

    // begin measuring time taken to load and search
    START_TIMER(t0)
    number_records = Get_Records(positions_file, &vehicle_records_ptr, sizeof(struct vehicle_records));

    // Find_Nearest(ref_coords, sizeof(ref_coords) / sizeof(struct vehicle_ref_coords), vehicle_records_ptr,
    //             number_records);

    // Assign each thread an ID and create all the threads.
    for (int i = 0; i < nthreads; i++)
    {
        thread_ids[i] = i;
        pthread_create(&thread_array[i], NULL, scalar_multiply, &thread_ids[i]);
    }

    /* Join all the threads. Main will pause in this loop until all threads
     * have returned from the thread function. */
    for (int i = 0; i < nthreads; i++)
    {
        pthread_join(thread_array[i], NULL);
    }

    STOP_TIMER(t1)

    // elapsed time in milliseconds.
    printf("delay(milliseconds): %" PRId64 "\n",
           (int64_t)(t1.tv_sec - t0.tv_sec) * 1000 + ((int64_t)(t1.tv_nsec - t0.tv_nsec) / 1000000));

    // printf("id nearest:%d dist:%f pos_id:%lu\n", coords[0].position_id_nearest, coords[0].distance,
    //        coords[0].position_id_nearest);
    // printf("id nearest:%d dist:%f pos_id:%lu\n", coords[1].position_id_nearest, coords[1].distance,
    //        coords[1].position_id_nearest);
    // for (int i = 0; i < 100; i++)
    // {
    //     printf("lat:%f lon:%f position_id:%d\n", vehicle_records_ptr[i].latitute, vehicle_records_ptr[i].longitude,
    //            vehicle_records_ptr[i].position_id);
    // }
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
static float GPS_Distance(float lat1, float lon1, float lat2, float lon2)
{
    float lat = 0.0F, dx = 0.0F, dy = 0.0F;
    lat = (float)((lat1 + lat2) / 2 * 0.01745);
    dx  = (float)(111.3 * cos(lat) * (lon1 - lon2));
    dy  = (float)(111.3 * (lat1 - lat2));

    return (float)sqrt(dx * dx + dy * dy);
}

/*
 *  Description:
 *  Parameters:
 *  Returns:
 */
void Find_Nearest(struct vehicle_ref_coords ref_coords[], uint32_t coords_count,
                  struct vehicle_records vehicle_records[], uint32_t records_count)
{
    float distance = 0.0f;

    for (size_t record_index = 0; record_index < records_count; record_index++)
    {
        for (size_t coords_index = 0; coords_index < coords_count; coords_index++)
        {
            if ((distance = GPS_Distance(ref_coords[coords_index].latitute_ref, ref_coords[coords_index].longitude_ref,
                                         vehicle_records[record_index].latitute,
                                         vehicle_records[record_index].longitude)) < ref_coords[coords_index].distance)
            {

                ref_coords[coords_index].position_id_nearest = vehicle_records[record_index].position_id;
                ref_coords[coords_index].distance            = distance;
            }
        }
    }

    printf("id nearest:%d dist:%f\n", ref_coords[0].position_id_nearest, ref_coords[0].distance);
    // printf("id nearest:%d dist:%f\n", ref_coords[1].position_id_nearest, ref_coords[1].distance);
    // printf("id nearest:%d dist:%f\n", ref_coords[2].position_id_nearest, ref_coords[2].distance);
    // printf("id nearest:%d dist:%f\n", ref_coords[3].position_id_nearest, ref_coords[3].distance);
    // printf("id nearest:%d dist:%f\n", ref_coords[4].position_id_nearest, ref_coords[4].distance);
    // printf("id nearest:%d dist:%f\n", ref_coords[5].position_id_nearest, ref_coords[5].distance);
    // printf("id nearest:%d dist:%f\n", ref_coords[6].position_id_nearest, ref_coords[6].distance);
    // printf("id nearest:%d dist:%f\n", ref_coords[7].position_id_nearest, ref_coords[7].distance);
    // printf("id nearest:%d dist:%f\n", ref_coords[8].position_id_nearest, ref_coords[8].distance);
    // printf("id nearest:%d dist:%f\n", ref_coords[9].position_id_nearest, ref_coords[9].distance);

    // printf("lat:%f lon:%f position_id:%d\n", vehicle_records[1017651].latitute, vehicle_records[1017651].longitude,
    //        vehicle_records[1017651].position_id);
}

void *scalar_multiply(void *id)
{
    float distance = 0.0f;
    long *myid     = (long *)id;
    long i;

    // assign each thread its own chunk of elements to process
    long chunk = length / nthreads;
    long start = *myid * chunk;
    long end   = start + chunk;
    if (*myid == nthreads - 1)
    {
        end = length;
    }

    // perform scalar multiplication on assigned chunk
    for (i = start; i < end; i++)
    {
        // for (size_t record_index = start; record_index < end; record_index++)
        //{
        // for (size_t coords_index = 0; coords_index < coords_count; coords_index++)
        //{
        if (*myid == 0)
        {
            for (size_t coords_index = 0; coords_index < 10; coords_index++)
            {
                if ((distance = GPS_Distance(ref_coords[coords_index].latitute_ref,
                                             ref_coords[coords_index].longitude_ref, vehicle_records_ptr[i].latitute,
                                             vehicle_records_ptr[i].longitude)) < coords[0][coords_index].distance)
                {
                    coords[0][coords_index].distance            = distance;
                    coords[0][coords_index].latitute_ref        = vehicle_records_ptr[i].latitute;
                    coords[0][coords_index].longitude_ref       = vehicle_records_ptr[i].longitude;
                    coords[0][coords_index].position_id_nearest = vehicle_records_ptr[i].position_id;
                }
            }
        }

        else
        {
            for (size_t coords_index = 0; coords_index < 10; coords_index++)
            {
                if ((distance = GPS_Distance(ref_coords[coords_index].latitute_ref,
                                             ref_coords[coords_index].longitude_ref, vehicle_records_ptr[i].latitute,
                                             vehicle_records_ptr[i].longitude)) < coords[1][coords_index].distance)
                {
                    coords[1][coords_index].distance            = distance;
                    coords[1][coords_index].latitute_ref        = vehicle_records_ptr[i].latitute;
                    coords[1][coords_index].longitude_ref       = vehicle_records_ptr[i].longitude;
                    coords[1][coords_index].position_id_nearest = vehicle_records_ptr[i].position_id;
                }
            }
        }
        // ref_coords[coords_index].position_id_nearest = vehicle_records[record_index].position_id;
        // ref_coords[coords_index].distance            = distance;
    }
    //  }
    //}

    return NULL;
}