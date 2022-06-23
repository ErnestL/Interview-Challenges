#include "dist.h"

Cordinate *getco_ordinates(FILE *fp){

    int read = 0;
    int records = 0;

    static Cordinate co_ordinates[10];
    if (fp == NULL)
    {
        printf("Error opening file.\n");
    }

    do
    {
        read = fscanf(fp,"%d,%f,%f,\n",
                      &co_ordinates[records].Position,
                      &co_ordinates[records].Latitude,
                      &co_ordinates[records].Longitude);
        if (read == 3) records++;

        if(read != 3 && !feof(fp))
        {
            printf("file format incorrect.\n");
        }
        if (ferror(fp))
        {
            printf("Error reading filr.\n");

        }

    }while(!feof(fp));
    
    return co_ordinates;

}