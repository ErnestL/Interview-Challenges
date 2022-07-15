/* 
 * Use your preferred naming convention
 * Add comments to help others to understand your code and to document assumptions
 * You are welcome to add additional files if required
 * /
/*
 * Use your preferred naming convention
 * Add comments to help others to understand your code and to document assumptions
 * You are welcome to add additional files if required
 */
   

#include <stdio.h>
#include "dist.h"
#include "getcor.c"
#include <stdlib.h>
#include <time.h>

int main()
{
    clock_t start, end;
    double time_taken;

    start = clock(); /* start measuring the time to execute the main function*/

    int count = 0;
    float dista = 0;

    /* File pointer for the cordinates.txt file*/
    FILE *fp_co;

    /* File pointer for the positions.dat file*/
    FILE *fp; 

    /* Array of Cordinate type structs for the 10 co_ordinates*/
    Cordinate *co_ordinates; 

    Shortest shortest_dist[10];

    Point points; 

    /* declare an array of 2million elements on the heap of Point type structs
    points = (Point*)malloc(2000000*sizeof(Point));*/ 

     /* open co_ordinates.txt file */
    fp_co = fopen("co_ordinates.txt", "r");

    /* read the cordinates from the co_ordinates.txt file into the array of type Cordinates*/
    co_ordinates = getco_ordinates(fp_co);



    /* close co_ordinates.txt file */
    fclose(fp_co);
    
    /* open positions.dat file */
    fp = fopen("positions.dat", "rb");

    /* check if the file opened successfully */
    if(fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    };
    
    /* read data into the points struct and calculate distance until the end of the file*/
    while( fread(&points, sizeof(Point), 1, fp) == 1)
     {
        for( int i =0; i< 10; i++){

            dista = distance(co_ordinates[i].Latitude,co_ordinates[i].Longitude, points.Latitude, points.Longitude);

          if (count == 0){
            shortest_dist[i].Position_ID = points.Position_ID;
            shortest_dist[i].distance = dista;
          }
          if(count !=0 && shortest_dist[i].distance < dista ){

               shortest_dist[i].distance = dista;
               shortest_dist[i].Position_ID = points.Position_ID;
             }

        }
        
        count++;
     }

    /* close positions.dat file */
    fclose(fp);
    printf("Read the file with count of %d\n", count);
    for(int i = 0; i<10; i++){
      printf("the position ID closest to position %d is %d\n", i+1, shortest_dist[i].Position_ID);

    }
     /* stop clock count */
    end = clock();
    time_taken = (double)(end - start) / (double)(CLOCKS_PER_SEC);
    printf("The time taken by the program is %f\n", time_taken);
    return 0;
}
   
