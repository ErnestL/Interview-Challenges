#include "solution.h"
#include "records.h"
#include <float.h>
#include <math.h>

static const char *positions_file = "./positions.dat";
struct timespec t0, t1;

// reference coordinates supplied as per challenge.
static struct vehicle_ref_coords ref_coords[] = {
    {34.544909F, -102.100843F, FLT_MAX, 0}, {32.345544F, -99.123124F, FLT_MAX, 0},
    {33.234235F, -100.214124F, FLT_MAX, 0}, {35.195739F, -95.348899F, FLT_MAX, 0},
    {31.895839F, -97.789573F, FLT_MAX, 0},  {32.895839F, -101.789573F, FLT_MAX, 0},
    {34.115839F, -100.225732F, FLT_MAX, 0}, {32.335839F, -99.992232F, FLT_MAX, 0},
    {33.535339F, -94.792232F, FLT_MAX, 0},  {32.234235F, -100.222222F, FLT_MAX, 0}};

int main(void)
{
    // begin measuring time taken to load and search
    START_TIMER(t0)
    number_records = Get_Records(positions_file, &vehicle_records_ptr, sizeof(struct vehicle_records));
    STOP_TIMER(t1)

    // elapsed time in milliseconds.
    printf("delay(milliseconds): %" PRId64 "\n",
           (int64_t)(t1.tv_sec - t0.tv_sec) * 1000 + ((int64_t)(t1.tv_nsec - t0.tv_nsec) / 1000000));
}
