/* 
 * Use your preferred naming convention
 * Add comments to help others to understand your code and to document assumptions
 * You are welcome to add additional files if required
 * 
 * Zwivhuya Ramudzuli, zwivhuya.ramudzuli@gmail.com
 */

#include <stdio.h>
#include "binary.h"

const double locPins [COORDROWS][COORDCOLS] = {
  {34.544909, 32.345544, 33.234235, 35.195739, 31.895839, 32.895839, 34.115839, 32.335839, 33.535339, 32.234235}, /* 10 latitude co-ordinates */
  {-102.100843, -99.123124, -100.214124, -95.348899, -97.789573, -101.789573, -100.225732, -99.992232, -94.792232, -100.222222} /* 10 longitude co-ordinates */
};

/* function prototypes */
void usage(char *app);

int main(int argc, char **argv)
{
  clock_t startCpu, endCpu;
  double diffCpu;
  FILE *binFile;
  struct stat meta;
  int algo, c, i, j, result;

  diffCpu = 0.0;
  binFile = NULL;
  algo = THREAD;
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
              if ((algo < SLOW) || (algo > THREAD)){
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


  if (algo != THREAD){
    startCpu = clock(); /* start CPU time watch */
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

    /* calculate and print execution time before exiting */
    endCpu = clock();
    diffCpu = subtime(startCpu, endCpu);
    fprintf(stderr, "cpu-time (reading binary file): %6f s\n", diffCpu);
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
    case THREAD :
      startCpu = clock();
      result = binaryFindClosestSol4(locPins[0], COORDROWS, COORDCOLS);
  }

  if (result < 0){
    if (algo != THREAD)
      fclose(binFile);
    return -1;
  }

  /* clean up variables */
  if (algo != THREAD)
    fclose(binFile);

  return 0;
}

void usage(char *app)
{
  printf("%s: utility to find closest vehicle to given co-ordinates\n", app);
  printf("usage: %s [-a]\n", app);
  printf("-a        algorithm to use [1-slow 2-fast 3-fast-eucladian 4-threaded].\n");
}
