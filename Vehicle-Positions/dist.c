#include <math.h>
#include "dist.h"
#define pi 3.14159265358979323846

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Function prototypes                                           :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/


float distance(float lat1, float lon1, float lat2, float lon2) {
  float theta, dist;
  if ((lat1 == lat2) && (lon1 == lon2)) {
    return 0;
  }
  else {
    theta = lon1 - lon2;
    dist = sin(lat1 * pi / 180) * sin(lat2 * pi / 180) + cos(lat1 * pi / 180) * cos(lat2 * pi / 180) * cos(theta * pi / 180);
    dist = acos(dist);
    dist = dist * 180 / pi;
    dist = dist * 60 * 1.1515;
    dist = dist * 1.609344;
    
    return (dist);
  }
}