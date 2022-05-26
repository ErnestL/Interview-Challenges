#include "binary.h"

int binaryFindClosestSol1(const double *coordinates, int rows, int cols, FILE *stream, struct stat *meta)
{
  struct objects *content;
  struct closest *answer;
  double calcDist;
  int i;

  content = NULL;
  answer = NULL;
  calcDist = 0;

  if (coordinates == NULL){
    fprintf(stderr, "null 10 position co-ordinates\n");
    return -1;
  }
  if (stream == NULL){
    fprintf(stderr, "null binary data file stream\n");
    return -1;
  }
  if (meta == NULL){
    fprintf(stderr, "null binary data metadata\n");
    return -1;
  }

  content = calloc(1, sizeof(struct objects));
  if (content == NULL){
    fprintf(stderr, "error allocating memory with malloc\n");
    return -1;
  }
  answer = calloc(1, sizeof(struct closest));
  if (answer == NULL){
    free(content);
    fprintf(stderr, "error allocating memory with malloc\n");
    return -1;
  }
  answer->posID = 0;
  answer->distance = 0;
  answer->lat = 0;
  answer->lon = 0;

  /* loop through each of the 10 co-ordinates */
  for (i = 0; i < cols; i++){
    answer->distance = MAXDISTANCE;
    /* read positions from binary file */
    while ((feof(stream) == 0)){
      fread(&content->posID, sizeof(content->posID), 1, stream);
      fread(&content->vReg, sizeof(content->vReg), 1, stream);
      fread(&content->lat, sizeof(content->lat), 1, stream);
      fread(&content->lon, sizeof(content->lon), 1, stream);
      fread(&content->tStamp, sizeof(content->tStamp), 1, stream);
      //TODO: use feof and ferror to error check while reading

      /* calculate distances and store smallest seen distance */
      calcDist = binaryHarvesine(*(coordinates + i), *(coordinates + cols + i), content->lat, content->lon);
      if (calcDist < answer->distance){
        answer->distance = calcDist;
        answer->posID = content->posID;
        answer->lat = content->lat;
        answer->lon = content->lon;
      }

#ifdef DEBUG
      /* printout binary data object */
      fprintf(stdout, "posID (%d) vReg (%s) lat (%f) lon (%f) tStamp (%ld)\n", content->posID, content->vReg, content->lat, content->lon, content->tStamp);
#endif
      //memset(&content->vReg, 0, sizeof(char)*VREGLEN); /* need this? */
    }

    rewind(stream); /* go to beginning of binary data file */

    /* print solution */
    fprintf(stdout, "# vehicle nearest to co-ordinate %d is at position ID %d (%f km away, lat:%f, lon:%f)\n", i + 1, answer->posID, answer->distance, answer->lat, answer->lon);
  }

  free(content);
  free(answer);

  return 0;
}

int binaryFindClosestSol2(const double *coordinates, int rows, int cols, FILE *stream, struct stat *meta)
{
  struct objects *content;
  struct closest *answer;
  double calcDist;
  int i;


  content = NULL;
  answer = NULL;
  calcDist = 0;

  if (coordinates == NULL){
    fprintf(stderr, "null 10 position co-ordinates\n");
    return -1;
  }
  if (stream == NULL){
    fprintf(stderr, "null binary data file stream\n");
    return -1;
  }
  if (meta == NULL){
    fprintf(stderr, "null binary data metadata\n");
    return -1;
  }

  content = calloc(1, sizeof(struct objects));
  if (content == NULL){
    fprintf(stderr, "error allocating memory with malloc\n");
    return -1;
  }
  answer = calloc(cols, sizeof(struct closest));
  if (answer == NULL){
    free(content);
    fprintf(stderr, "error allocating memory with malloc\n");
    return -1;
  }

  /* initialise array */
  for (i = 0; i < cols; i++){
  answer[i].posID = 0;
  answer[i].distance = MAXDISTANCE;
  answer[i].lat = 0;
  answer[i].lon = 0;
  }

  /* read positions from binary file */
  while ((feof(stream) == 0)){
    fread(&content->posID, sizeof(content->posID), 1, stream);
    fread(&content->vReg, sizeof(content->vReg), 1, stream);
    fread(&content->lat, sizeof(content->lat), 1, stream);
    fread(&content->lon, sizeof(content->lon), 1, stream);
    fread(&content->tStamp, sizeof(content->tStamp), 1, stream);
    //TODO: use feof and ferror to error check while reading

    /* calculate distances and store smallest seen distance for all 10 co-ordinates */
    for (i = 0; i < cols; i++){
    calcDist = binaryHarvesine(*(coordinates + i), *(coordinates + cols + i), content->lat, content->lon);
      if (calcDist < answer[i].distance){
        answer[i].distance = calcDist;
        answer[i].posID = content->posID;
        answer[i].lat = content->lat;
        answer[i].lon = content->lon;
      }
    }

#ifdef DEBUG
    /* printout binary data object */
    fprintf(stdout, "posID (%d) vReg (%s) lat (%f) lon (%f) tStamp (%ld)\n", content->posID, content->vReg, content->lat, content->lon, content->tStamp);
#endif
    //memset(&content->vReg, 0, sizeof(char)*VREGLEN); /* need this? */
  }

  /* loop through each of the 10 co-ordinates */
  for (i = 0; i < cols; i++){
    /* print solution */
    fprintf(stdout, "# vehicle nearest to co-ordinate %d is at position ID %d (%f km away, lat:%f, lon:%f)\n", i + 1, answer[i].posID, answer[i].distance, answer[i].lat, answer[i].lon);
  }

  free(content);
  free(answer);

  return 0;
}

int binaryFindClosestSol3(const double *coordinates, int rows, int cols, FILE *stream, struct stat *meta)
{
  struct objects *content;
  struct closest *answer;
  double calcDist;
  int i;


  content = NULL;
  answer = NULL;
  calcDist = 0;

  if (coordinates == NULL){
    fprintf(stderr, "null 10 position co-ordinates\n");
    return -1;
  }
  if (stream == NULL){
    fprintf(stderr, "null binary data file stream\n");
    return -1;
  }
  if (meta == NULL){
    fprintf(stderr, "null binary data metadata\n");
    return -1;
  }

  content = calloc(1, sizeof(struct objects));
  if (content == NULL){
    fprintf(stderr, "error allocating memory with malloc\n");
    return -1;
  }
  answer = calloc(cols, sizeof(struct closest));
  if (answer == NULL){
    free(content);
    fprintf(stderr, "error allocating memory with malloc\n");
    return -1;
  }

  /* initialise array */
  for (i = 0; i < cols; i++){
  answer[i].posID = 0;
  answer[i].distance = MAXDISTANCE;
  answer[i].lat = 0;
  answer[i].lon = 0;
  }

  /* read positions from binary file */
  while ((feof(stream) == 0)){
    fread(&content->posID, sizeof(content->posID), 1, stream);
    fread(&content->vReg, sizeof(content->vReg), 1, stream);
    fread(&content->lat, sizeof(content->lat), 1, stream);
    fread(&content->lon, sizeof(content->lon), 1, stream);
    fread(&content->tStamp, sizeof(content->tStamp), 1, stream);
    //TODO: use feof and ferror to error check while reading

    /* calculate distances and store smallest seen distance for all 10 co-ordinates */
    for (i = 0; i < cols; i++){
    calcDist = binaryEuclidian(*(coordinates + i), *(coordinates + cols + i), content->lat, content->lon);
      if (calcDist < answer[i].distance){
        answer[i].distance = calcDist;
        answer[i].posID = content->posID;
        answer[i].lat = content->lat;
        answer[i].lon = content->lon;
      }
    }

#ifdef DEBUG
    /* printout binary data object */
    fprintf(stdout, "posID (%d) vReg (%s) lat (%f) lon (%f) tStamp (%ld)\n", content->posID, content->vReg, content->lat, content->lon, content->tStamp);
#endif
    //memset(&content->vReg, 0, sizeof(char)*VREGLEN); /* need this? */
  }

  /* loop through each of the 10 co-ordinates */
  for (i = 0; i < cols; i++){
    /* print solution */
    fprintf(stdout, "# vehicle nearest to co-ordinate %d is at position ID %d (%f km away, lat:%f, lon:%f)\n", i + 1, answer[i].posID, answer[i].distance, answer[i].lat, answer[i].lon);
  }

  free(content);
  free(answer);

  return 0;
}

int binaryPrintCoordinates(const double *coordinates, int rows, int cols)
{
  int i, j;

  if (coordinates == NULL){
    fprintf(stderr, "null 10 position co-ordinates\n");
    return -1;
  }

  for (i = 0; i < rows; i++){
    fprintf(stdout, ">|");
    for (j = 0; j < cols; j++){
      fprintf(stdout, "%f|", *(coordinates + i*cols + j));
    }
    fprintf(stdout, "\n");
  }

  return 0;
}

long double binaryHarvesine(double pinLat, double pinLon, float dataLat, float dataLon)
{
  double pinLatRad, pinLonRad, dataLatRad, dataLonRad;
  double diffLat, diffLon;
  long double ans;

  /* convert latitude and longitude co-ordinates to radians */
  pinLatRad = binaryDegToRed(pinLat);
  pinLonRad = binaryDegToRed(pinLon);
  dataLatRad = binaryDegToRed(dataLat);
  dataLonRad = binaryDegToRed(dataLon);
  diffLat = dataLatRad - pinLatRad;
  diffLon = dataLonRad - pinLonRad;

  /* Euclidian distance */
  ans = pow(sin(diffLat/2), 2) + cos(pinLatRad) * cos(dataLatRad) * pow(sin(diffLon/2), 2);
  ans = 2 * asin(sqrt(ans));

  /* return distance in kilometers */
  return ans * EARTHRADIUS;
}

long double binaryEuclidian(double pinLat, double pinLon, float dataLat, float dataLon)
{
  double pinLatRad, pinLonRad, dataLatRad, dataLonRad;
  double x1, y1, x2, y2;
  long double ans;

  /* convert latitude and longitude co-ordinates to radians */
  pinLatRad = binaryDegToRed(pinLat);
  pinLonRad = binaryDegToRed(pinLon);
  dataLatRad = binaryDegToRed(dataLat);
  dataLonRad = binaryDegToRed(dataLon);

  x1 = EARTHRADIUS * cos(pinLatRad) * cos(pinLonRad);
  y1 = EARTHRADIUS * cos(pinLatRad) * sin(pinLonRad);
  x2 = EARTHRADIUS * cos(dataLatRad) * cos(dataLonRad);
  y2 = EARTHRADIUS * cos(dataLatRad) * sin(dataLonRad);

  /* return distance in kilometers */
  return  ans = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double binaryDegToRed(double degree)
{
  return (PI * degree) / 180.0;
}
