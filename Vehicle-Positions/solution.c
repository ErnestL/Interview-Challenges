/* 
 * Use your preferred naming convention
 * Add comments to help others to understand your code and to document assumptions
 * You are welcome to add additional files if required
 * 
 * Zwivhuya Ramudzuli, zwivhuya.ramudzuli@gmail.com
 */

#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include "binary.h"

#define COORDROWS 2 /* assumption: 2 rows, latitude and longitude */
#define COORDCOLS 10 /*assumption: 10 position co-ordinates/location pins given per axis */

const double locPins [COORDROWS][COORDCOLS] = {
  {34.544909, 32.345544, 33.234235, 35.195739, 31.895839, 32.895839, 34.115839, 32.335839, 33.535339, 32.234235}, /* 10 latitude co-ordinates */
  {-102.100843, -99.123124, -100.214124, -95.348899, -97.789573, -101.789573, -100.225732, -99.992232, -94.792232, -100.222222} /* 10 longitude co-ordinates */
};

/* function prototypes */
void usage(char *app);
int subtime(struct timeval *start, struct timeval *end, struct timeval *diff);

int main(int argc, char **argv)
{
  struct timeval startWall, endWall, diffWall;
  FILE *binFile;
  struct stat meta;
  int algo, c, i, j, result;

  startWall.tv_sec = 0;
  startWall.tv_usec = 0;
  endWall.tv_sec = 0;
  endWall.tv_usec = 0;
  diffWall.tv_sec = 0;
  diffWall.tv_usec = 0;
  binFile = NULL;
  algo = FAST;
  result = 0;

  /* process user selection of algorithm to use */
  i = j = 1;
  while (i < argc){
    if (argv[i][0] == '-'){
      c = argv[i][j];
      switch(c){
        case 'h':
          usage(argv[0]);
          return 0;
          break;

        case 'a':
          j++;
          if (argv[i][j] == '\0'){
            j = 0;
            i++;
          }
          if  (i >= argc){
            fprintf(stderr, "argument needs a parameter\n");
            return -1;
          }

          switch (c){
            case 'a':
              algo = atoi(argv[i] + j);
              if ((algo < SLOW) || (algo > FASTALT)){
                fprintf(stderr, "invalid input for argument, see help (-h)\n");
                return -1;
              }
              break;
          }

          i++;
          j = 1;
          break;

        case '-':
          j++;
          break;

        case '\0':
          j = 1;
          i++;
          break;

        default:
          algo = FAST;
          break;
      }
    } else {
      i++;
    }
  }

#ifdef DEBUG
  /* print 10 position coordinates of interest given */
  binaryPrintCoordinates(locPins[0], COORDROWS, COORDCOLS);
#endif

  gettimeofday(&startWall, NULL); /* start wall-timer watch */

  /* load the binary data file for reading */
  binFile = fopen(BINDATAFILE, "rb");
  if (binFile == NULL){
    fprintf(stderr, "failed to open binary data file %s\n", BINDATAFILE);
    return -1;
  }

  /* read file metadata */
  if (stat(BINDATAFILE, &meta) == -1){
    fclose(binFile);
    return -1;
  }

  /* load binary data file and do calculations */
  switch (algo){
    case SLOW :
      result = binaryFindClosestSol1(locPins[0], COORDROWS, COORDCOLS, binFile, &meta);
      break;
    case FAST :
      result = binaryFindClosestSol2(locPins[0], COORDROWS, COORDCOLS, binFile, &meta);
      break;
    case FASTALT :
      result = binaryFindClosestSol3(locPins[0], COORDROWS, COORDCOLS, binFile, &meta);
      break;
  }

  if (result < 0){
    fclose(binFile);
    return -1;
  }

  /* clean up variables */
  fclose(binFile);

  /* calculate and print execution time before exiting */
  gettimeofday(&endWall, NULL);
  subtime(&startWall, &endWall, &diffWall);
  fprintf(stderr, "wall-time: execution took %ld.%ld s\n", diffWall.tv_sec, diffWall.tv_usec);

  return 0;
}

void usage(char *app)
{
  printf("%s: utility to find closest vehicle to given co-ordinates\n", app);
  printf("usage: %s [-a]\n", app);
  printf("-a        algorithm to use [1-slow 2-fast 3-fast-eucladian].\n");
}

/* return difference of two timestamps */
int subtime(struct timeval *start, struct timeval *end, struct timeval *diff)
{
  if ((start == NULL) || (end == NULL) || (diff == NULL)){
    fprintf(stderr, "null argument(s)\n");
    return -1;
  }
  
  /* subtract sec part */
  if (start->tv_sec > end->tv_sec){
    diff->tv_sec = start->tv_sec - end->tv_sec;
  } else {
    diff->tv_sec = end->tv_sec - start->tv_sec;
  }
  /* subtract usec part */
  if (start->tv_usec > end->tv_usec){
    diff->tv_usec = start->tv_usec - end->tv_usec;
  } else {
    diff->tv_usec = end->tv_usec - start->tv_usec;
  }

  return 0;
}
