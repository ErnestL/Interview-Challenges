#ifndef _DIST_
#define _DIST_

/* Co_ordinates struct*/
 typedef struct cordinate  {
     
    int Position;
    float Latitude;
    float Longitude;
} Cordinate;

/* struct to represent each record on the position.dat file*/
typedef struct point {
     int Position_ID;
     char Vehicle_Reg[100];
     float Latitude;
     float Longitude;
     unsigned long long UTC; 
 }Point;

/* struct to represent the Position_ID with the shortest distance to one of the 10 given points*/
  typedef struct shortest {
     
    int Position_ID;
    float distance;
} Shortest;

/* function to calculate the distance between two co_ordinates*/
float distance(float lat1, float lon1, float lat2, float lon2);



#endif