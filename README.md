# Problem Solving Approach:
created a txt file to store the 10 given positions and read the file into a an array of structs for each position
created 3 structs as follows:
     - struct to represent each object on the position.dat file
     - struct to represent each object with position ID and co_ordinates for the given data stored in the txt file.
     - struct to store the shorted distance between each given point and the point in the positions.dat file as well as the Position ID of the closest point
created a function to determine the distance between two points in KMs
created a function to read the 10 positions from the text file
Used the fread function in a while loop to get each object from the positions.dat file in binary mode and calculted the distance at each reading iteration
Updated the Shortet distance struct whenever the new calculated distance was less that the one calculted from the previous iteration.

# Assumptions Made
  - Each object in the positions.dat file had PostionID,Latitude,Longitude,Vehicle registratio and UTC
  - The positions.dat file had 2 million objects
  - the latitude and longitude were in degrees
