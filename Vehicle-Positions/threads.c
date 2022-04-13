
#include "misc.h"
#include "solution.h"
#include <math.h>
#include <pthread.h>

extern struct vehicle_ref_coords ref_coords[];
extern struct vehicle_ref_coords coords[THREADS][10];

static const uint32_t nthreads = THREADS; /* number of threads */
static pthread_t *thread_array = NULL;    /* pointer to future thread array */
static size_t *thread_ids      = NULL;

static void *search(void *id);

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

    free(thread_ids);
    free(thread_array);

    return;
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
            if ((distance = find_distance(ref_coords[coords_index].latitute_ref, ref_coords[coords_index].longitude_ref,
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
