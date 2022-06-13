<<<<<<< HEAD
/*Author: Daniel Lombard */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

float Lat[10] = {34.544909, 32.345544, 33.234235, 35.195739, 31.895839, 32.895839, 34.115839, 32.335839,	33.535339, 32.234235};
float Long[10] = {-102.100843, -99.123124, -100.214124, -95.348899, -97.789573, -101.789573,	-100.225732, -99.992232, -94.792232, -100.222222};

const char* filename = "positions.dat";
struct Truck {
    int Position_ID;
    char Vehicle_reg[10];
    float Latitude;
    float Longitude;
    unsigned long long  UTC;
} Truck;



int main()
{
    struct Truck Current_Truck;
    int Close_ID[10];
    float Close_Lat[10], Close_Long[10], Close_Dist[10];
    float Delta_Lat, Delta_Long, Distance;                        // Current truck data

    /*Start measuring time */
    struct timeval begin,end;
    gettimeofday(&begin,0);


    /*Open binary data file */
    FILE * fpointer = fopen(filename,"rb");

    /*Check for succesfull opening of file */
    if(fpointer == NULL)
    {
        perror("Error opening file"); // print error
        return(1);
    }
    else {
        fseek(fpointer,0,SEEK_SET); //Set cursor to begining of file

        /*Extracts data of first vehicle */
        fread(&Current_Truck.Position_ID,sizeof(Current_Truck.Position_ID),1,fpointer);     //Current vehicle Position ID
        fread(&Current_Truck.Vehicle_reg,sizeof(Current_Truck.Vehicle_reg),1,fpointer);     //Current vehicle Vehicle Registration
        fread(&Current_Truck.Latitude,sizeof(Current_Truck.Latitude),1,fpointer);           //Current vehicle Latitude Coordinate
        fread(&Current_Truck.Longitude,sizeof(Current_Truck.Longitude),1,fpointer);         //Current vehicle Longitude Coordinate
        fread(&Current_Truck.UTC,sizeof(Current_Truck.UTC),1,fpointer);                     //Current vehicle UTC Timestamp


        /*The first vehicle read is closest by default*/
        for (int i=0;i<10;i++){
            Close_ID[i] = Current_Truck.Position_ID;                      // ID of closest truck
            Close_Lat[i] = fabs(Current_Truck.Latitude - Lat[i]);        // Delta Latitude of closest truck
            Close_Long[i] = fabs(Current_Truck.Longitude - Long[i]);     // Delta Longitude of closest truck
            Close_Dist[i] = 0;                                          // 0 distance indicates that it hasn't been calculated yet (no need to do memory intensive multiplication of not neccasary)
        }

        /*Extract vehicle data until the end of file is reached*/
        while ((fgetc(fpointer)!= EOF)){
                fseek(fpointer,-1,SEEK_CUR);

        /* Extract next vehicle data */
        fread(&Current_Truck.Position_ID,sizeof(Current_Truck.Position_ID),1,fpointer);     //Current vehicle Position ID
        fread(&Current_Truck.Vehicle_reg,sizeof(Current_Truck.Vehicle_reg),1,fpointer);     //Current vehicle Vehicle Registration
        fread(&Current_Truck.Latitude,sizeof(Current_Truck.Latitude),1,fpointer);           //Current vehicle Latitude Coordinate
        fread(&Current_Truck.Longitude,sizeof(Current_Truck.Longitude),1,fpointer);         //Current vehicle Longitude Coordinate
        fread(&Current_Truck.UTC,sizeof(Current_Truck.UTC),1,fpointer);                     //Current vehicle UTC Timestamp


        //CHECK WETHER CLOSER FOR ALL 10 POINTS
        for (int i=0;i<10;i++){
            Delta_Lat = fabs(Lat[i] - Current_Truck.Latitude);       //Delta Latitude
            Delta_Long = fabs(Long[i] - Current_Truck.Longitude);    //Delta Longitude


//if both delta lat and long is less than current closer, then there is no need to calculate distance, it must be closer

            if ((Delta_Lat<Close_Lat[i])&&(Delta_Long<Close_Long[i])){
                Close_ID[i] = Current_Truck.Position_ID;
                Close_Lat[i] = Delta_Lat;
                Close_Long[i] = Delta_Long;
                Close_Dist[i] = 0;
            }

            else if (((Delta_Lat<Close_Lat[i])||(Delta_Long<Close_Long[i]))){

                /*Calculate current closest vehicles distance if it hasn't been calculated previously*/
                if (Close_Dist[i] == 0){
                    Close_Dist[i] = (Close_Lat[i])*(Close_Lat[i]) + (Close_Long[i])*(Close_Long[i]);    //Distance^2 of current closest vehicle
                }
                Distance = (Delta_Lat)*(Delta_Lat) + (Delta_Long)*(Delta_Long); //Distance^2 of current vehicle

                /*Determine whether vehicle is closer than the current closest vehicle*/
                if (Distance<Close_Dist[i]){
                    /*Set new closest vehicle*/
                    Close_ID[i] = Current_Truck.Position_ID;
                    Close_Lat[i] = Delta_Lat;
                    Close_Long[i] = Delta_Long;
                    Close_Dist[i] = Distance;
                }
            }
        }
        }

        /*Stop measuring time and calculate the elapsed time */
        gettimeofday(&end,0);
        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double elapsed = seconds + microseconds*1e-6;

    /*Print Results */
    for (int i=0;i<10;i++){
        printf("Truck closest to point %d: %d \n",i+1,Close_ID[i]);
    }
    printf("Wall time measured: %.3f seconds.\n",elapsed);
    }
    /*Close binary data file*/
    fclose(fpointer);
    
    char s[1];
    scanf("%s",s);
    return 0;
}
=======
/*Author: Daniel Lombard */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

float Lat[10] = {34.544909, 32.345544, 33.234235, 35.195739, 31.895839, 32.895839, 34.115839, 32.335839,	33.535339, 32.234235};
float Long[10] = {-102.100843, -99.123124, -100.214124, -95.348899, -97.789573, -101.789573,	-100.225732, -99.992232, -94.792232, -100.222222};

const char* filename = "positions.dat";
struct Truck {
    int Position_ID;
    char Vehicle_reg[10];
    float Latitude;
    float Longitude;
    unsigned long long  UTC;
} Truck;



int main()
{
    struct Truck Current_Truck;
    int Close_ID[10];
    float Close_Lat[10], Close_Long[10], Close_Dist[10];
    float Delta_Lat, Delta_Long, Distance;                        // Current truck data

    /*Start measuring time */
    struct timeval begin,end;
    gettimeofday(&begin,0);


    /*Open binary data file */
    FILE * fpointer = fopen(filename,"rb");

    /*Check for succesfull opening of file */
    if(fpointer == NULL)
    {
        perror("Error opening file"); // print error
        return(1);
    }
    else {
        fseek(fpointer,0,SEEK_SET); //Set cursor to begining of file

        /*Extracts data of first vehicle */
        fread(&Current_Truck.Position_ID,sizeof(Current_Truck.Position_ID),1,fpointer);     //Current vehicle Position ID
        fread(&Current_Truck.Vehicle_reg,sizeof(Current_Truck.Vehicle_reg),1,fpointer);     //Current vehicle Vehicle Registration
        fread(&Current_Truck.Latitude,sizeof(Current_Truck.Latitude),1,fpointer);           //Current vehicle Latitude Coordinate
        fread(&Current_Truck.Longitude,sizeof(Current_Truck.Longitude),1,fpointer);         //Current vehicle Longitude Coordinate
        fread(&Current_Truck.UTC,sizeof(Current_Truck.UTC),1,fpointer);                     //Current vehicle UTC Timestamp


        /*The first vehicle read is closest by default*/
        for (int i=0;i<10;i++){
            Close_ID[i] = Current_Truck.Position_ID;                      // ID of closest truck
            Close_Lat[i] = fabs(Current_Truck.Latitude - Lat[i]);        // Delta Latitude of closest truck
            Close_Long[i] = fabs(Current_Truck.Longitude - Long[i]);     // Delta Longitude of closest truck
            Close_Dist[i] = 0;                                          // 0 distance indicates that it hasn't been calculated yet (no need to do memory intensive multiplication of not neccasary)
        }

        /*Extract vehicle data until the end of file is reached*/
        while ((fgetc(fpointer)!= EOF)){
                fseek(fpointer,-1,SEEK_CUR);

        /* Extract next vehicle data */
        fread(&Current_Truck.Position_ID,sizeof(Current_Truck.Position_ID),1,fpointer);     //Current vehicle Position ID
        fread(&Current_Truck.Vehicle_reg,sizeof(Current_Truck.Vehicle_reg),1,fpointer);     //Current vehicle Vehicle Registration
        fread(&Current_Truck.Latitude,sizeof(Current_Truck.Latitude),1,fpointer);           //Current vehicle Latitude Coordinate
        fread(&Current_Truck.Longitude,sizeof(Current_Truck.Longitude),1,fpointer);         //Current vehicle Longitude Coordinate
        fread(&Current_Truck.UTC,sizeof(Current_Truck.UTC),1,fpointer);                     //Current vehicle UTC Timestamp


        //CHECK WETHER CLOSER FOR ALL 10 POINTS
        for (int i=0;i<10;i++){
            Delta_Lat = fabs(Lat[i] - Current_Truck.Latitude);       //Delta Latitude
            Delta_Long = fabs(Long[i] - Current_Truck.Longitude);    //Delta Longitude


//if both delta lat and long is less than current closer, then there is no need to calculate distance, it must be closer

            if ((Delta_Lat<Close_Lat[i])&&(Delta_Long<Close_Long[i])){
                Close_ID[i] = Current_Truck.Position_ID;
                Close_Lat[i] = Delta_Lat;
                Close_Long[i] = Delta_Long;
                Close_Dist[i] = 0;
            }

            else if (((Delta_Lat<Close_Lat[i])||(Delta_Long<Close_Long[i]))){

                /*Calculate current closest vehicles distance if it hasn't been calculated previously*/
                if (Close_Dist[i] == 0){
                    Close_Dist[i] = (Close_Lat[i])*(Close_Lat[i]) + (Close_Long[i])*(Close_Long[i]);    //Distance^2 of current closest vehicle
                }
                Distance = (Delta_Lat)*(Delta_Lat) + (Delta_Long)*(Delta_Long); //Distance^2 of current vehicle

                /*Determine whether vehicle is closer than the current closest vehicle*/
                if (Distance<Close_Dist[i]){
                    /*Set new closest vehicle*/
                    Close_ID[i] = Current_Truck.Position_ID;
                    Close_Lat[i] = Delta_Lat;
                    Close_Long[i] = Delta_Long;
                    Close_Dist[i] = Distance;
                }
            }
        }
        }

        /*Stop measuring time and calculate the elapsed time */
        gettimeofday(&end,0);
        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double elapsed = seconds + microseconds*1e-6;

    /*Print Results */
    for (int i=0;i<10;i++){
        printf("Truck closest to point %d: %d \n",i+1,Close_ID[i]);
    }
    printf("Wall time measured: %.3f seconds.\n",elapsed);
    }
    /*Close binary data file*/
    fclose(fpointer);
    
    char s[1];
    scanf("%s",s);
    return 0;
}
>>>>>>> afcf5ebd478a09c4cfa77c1ca354f5aeb185c730
