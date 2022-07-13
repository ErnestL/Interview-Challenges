/*
 * \file binary.h
 *
 * \brief Header for the functions and variables needed to process the binary data file and perfom calculations required to solve the challenge
 *
 * \author Zwivhuya Ramudzuli
 */

#ifndef BINARY_H_
#define BINARY_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

#define BINDATAFILE "positions.dat" /* assumption: binary data file name */
#define VREGLEN 10 /* assumption: maximum vehicle registration characters including Null terminating string */
#define MAXDISTANCE 40075.017 /* assumption: earths circumference from the equator i.e. co-ordinate distance is never larger than half of this */
#define PI 3.14159265358979323846
#define EARTHRADIUS 6371.0 /* assumption: radius of earth in Kilometers */
#define SLOW 1 /* use slower iterative solution i.e. binaryFindClosestSol1 */
#define FAST 2 /* use faster solution i.e. binaryFindClosestSol2 */
#define FASTALT 3 /* use faster solution i.e. binaryFindClosestSol3 */
#define THREAD 4 /* use threads to spread solution computational load */

#define COORDROWS 2 /* assumption: 2 rows, latitude and longitude */
#define COORDCOLS 10 /*assumption: 10 position co-ordinates/location pins given per axis */

/* structure to retrieve binary data file objects */
struct objects{
  int32_t posID;
  char vReg[VREGLEN];
  float lat;
  float lon;
  uint64_t tStamp;
};

struct closest{
  int32_t posID;
  float lat;
  float lon;
  long double distance;
};

struct threadArg{
  pthread_t thread_id;
  FILE *stream;
  int codNum;
  double pin_lat;
  double pin_lon;
};

/* print 10 position coordinates of interest given */
int binaryPrintCoordinates(const double *coordinates, int rows, int cols);
/* calculate Eucladian disctance between two points */
long double binaryHarvesine(double pinLat, double pinLon, float dataLat, float dataLon);
/* convert degree to redians */
double binaryDegToRed(double degree);

/*
 * SOLUTION #1: Harvesine iterative method
 * Iteratively reads the binary data file 10 times each time calculating the closest vehicle to a single co-ordinate using the Harvesine formula
 *
 */
int binaryFindClosestSol1(const double *coordinates, int rows, int cols, FILE *stream, struct stat *meta);

/* SOLUTION 2: Optimsed Harvesine alternative
 * Reads the binary data file 1 time, each time calculating the closest vehicle position for all 10 co-ordinates using the Harvesine formula
 *
 */
int binaryFindClosestSol2(const double *coordinates, int rows, int cols, FILE *stream, struct stat *meta);

/* SOLUTION 3: Euclidian distance based alternative
 * Reads the binary data file 1 time, each time calculating the closest vehicle position for all 10 co-ordinates using Eaclidian distance formula
 *
 */
int binaryFindClosestSol3(const double *coordinates, int rows, int cols, FILE *stream, struct stat *meta);

/* SOLUTION 4: Using threads
 * Reads the binary data file 1 time, spread the load of calculating the closest vehicle position using threads
 */
int binaryFindClosestSol4(const double *coordinates, int rows, int cols);

void *binaryFindClosestSol4Thread(void *args);

long double binaryEuclidian(double pinLat, double pinLon, float dataLat, float dataLon);

long double binaryPythagorean(double pinLat, double pinLon, float dataLat, float dataLon);

/* return difference of two timestamps */
double subtime(clock_t start, clock_t end);

/* return difference of two timestamps */
int subtimeWall(struct timeval *start, struct timeval *end, struct timeval *diff);
#endif /* BINARY_H_ */
