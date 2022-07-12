# Vehicle Position Challenge
:nerd_face: Author: Zwivhuya Ramudzuli

# Problem Solving Approach

Given that the vehicle position challenge requires the computation of geographical distances on Earth, mathematical formula that model the geographic characteristics of the Earth are required. For the purposes of this challenge, two very popular and frequently used formula in the development of Geographic Information Systems (GIS) were considered; the `Harvesine Formula` and the `Euclidian Formula`.

### Harvesine shortest distance formula
The Harvesine shortest distance formula calculates the shortest distance between two points on a sphere using their latitudes and longitudes measured along the surface. The formula to calculate is derived as follows:
```
a = sin²(ΔlatDifference/2) + cos(lat1).cos(lt2).sin²(ΔlonDifference/2)
c = 2.atan2(√a, √(1−a))
d = R.c 
```
Where,
* ΔlatDifference = lat1 – lat2 (denotes difference of latitude)
* ΔlonDifference = lon1 – lon2 (denotes difference of longitude)
* R denotes the radius of earth
* d denotes the distance computed between two points in kilometers

The formula consists of a number of mathematical computations spanning trigonometric identities, squaring and other general mathematical operators when compared to the Eucladian formula, however it is better models the Earth geographic characteristics and consequently produce more accurate results. This formula can be considered computationally expensive however this will be quantitatively evaluated from the benchmark results.

### Eucladian shortest distance formula
The Eucladian shortest distance formula calculates the shortest straight line distance between two points by transforming the points to the Cartesian plane (i.e. x, y and z). To achieve this, the formula disregards the elevation and the Cartesian plane does not model the spherical characteristics of the Earth. The formula to calculate is derived as follows:
```
⃗r=⎛x,y,z⎞=⎛Rcosθcosϕ, Rcosθsinϕ, Rsinθ⎞
d(r1,r2)=√(x2−x1)2+(y2−y1)2+(z2−z1)2.
```
Where,
* ϕ denotes, the latitude
* λ denotes and the longitude
* R denotes, the Earth's radius
* r denotes the cartesian coordinates
* d denotes the distance Euclidean distance between two points in kilometers

Consequently the formula looses some accuracy however is usable for this challenge where speed is the priority. This formula computationally inexpensive when compared to the Harvesine since it has fewer terms to evaluate in computing the resultant Euclidean distance.

### Pythegorus Theorem distance formula
The Pythegorus theorem based distance formula is similar to the Eucladian shortest distance formula however the coordinates do not under go any tranformation which fruther reduces the computaiton complexity. The formula to calculate is derived as follows:
```
d=√(x2−x1)2+(y2−y1)2+(z2−z1)2.
```
Where,
* x1,x2 denotes, the latitude coordinates
* y1,y2 denotes and the longitude coordinates
* d denotes the distance Euclidean distance between two points in kilometers

# Method
The 10 reference co-ordinates are stored in a 2D array, which is used an an argument to the shortest distance calculation functions. The binary stream based I/O functions found in the standard C library `fread()` was used to read data from the binary file. Other alternatives such as `read()` and `mmap()` could have been used however sources read suggest that they do not offer notable efficiency towards faster execution times.

### Data structures
The following `struct objects` data structure was used to extract the individual data values from the records in the binary data file. A pointer to an `objects` structure is passed into the `fread()` method do retreate and isolate the various fields for use in the shortest distance calculations.
```C
/* structure to retrieve binary data file objects */
struct objects{
  int32_t posID;
  char vReg[VREGLEN];
  float lat;
  float lon;
  uint64_t tStamp;
};
```
The following `struct closest` data structure was used to store the shortest individual distance for each of the 10 reference positions. An array of `closest`stucts was subsequently used to store the solutions consisting of 10 position ID's, and their respective latitude and longitude co-ordinates, and shortest distance.
```C
struct closest{
  int32_t posID;
  float lat;
  float lon;
  double distance;
};
```

### Harvesine formula based algorithm (slowest)
Iteratively reads the binary data file 10 times each time calculating the closest vehicle to a single co-ordinate using the Harvesine distance formula. This algorithm rewinds to the beginning of the binary data file for every reference position closest distance calculation using the standard C library method `rewind(stream)`. The wall time increase with an increase in reference position for which the distances are calculated for. Consequently, the binary data file is read `n` times where `n` is equal to the number of reference position of interest, where `n = 10` for this challenge.
```
long double binaryHarvesine(double pinLat, double pinLon, float dataLat, float dataLon);
int binaryFindClosestSol1(const double *coordinates, int rows, int cols, FILE *stream, struct stat *meta);
```

### Optimised Harvesine formula based algorithm (fast)
Reads the binary data file 1 time, each time calculating the closest vehicle position for all 10 co-ordinates using the Harvesine distance formula. This approach sequentially reads the entire file however for each data record read, the shortest distances are calculated for all 10 reference position before reading the next. With each `fread()` call, the shortest distance is stored if it is mathematically smaller than the last calculated distance. This guarantees that the shortest distances are considered after all binary data records are read.
```
int binaryFindClosestSol2(const double *coordinates, int rows, int cols, FILE *stream, struct stat *meta);
```

### Eucladian formula based algorithm (equally fast)
Reads the binary data file 1 time, each time calculating the closest vehicle position for all 10 co-ordinates using Eaclidian distance formula. The algorithm is similar to the one above with the exception of the formula used.
```
int binaryFindClosestSol3(const double *coordinates, int rows, int cols, FILE *stream, struct stat *meta);
long double binaryEuclidian(double pinLat, double pinLon, float dataLat, float dataLon);
```
# Assumptions
The following assumptions were made

### General
* No coding standard was suggested for the challenge as such the code was written to be neat and readable
* A 64 bit Linux machine running Ubuntu 20.04.4 LTS was used
* The binary data file is considered to be complete with no missing or illegal data objects, as such the file is read without checking for `fread()` errors  that would arise
* The end of file test criteria (`feof(stream)`) is the only method used to determine when to stop reading the file

### Bench-marking
* Given that the application performs an I/O operation of reading a binary data file, measuring wall-time is the appropriate measure for execution time.
* The `gettimeofday()` method provided in the standard system time library was used to measure the applications wall time. 
* It is important to note this method is affected by discontinuous jumps in the system time

### Constants and units
* The results are presented in kilometers (km)
* `BINDATAFILE` "positions.dat" // assumption: binary data file name
* `VREGLEN 10` // assumption: maximum vehicle registration characters including Null terminating string
* `MAXDISTANCE` 40075.017 // assumption: earths circumference from the equator i.e. co-ordinate distance is never larger than half of this
* `PI` 3.14159265358979323846
* `EARTHRADIUS` 6371.0 // assumption: radius of earth in Kilometers
* 10 position co-ordinates/location pins were given

### Compiler
* Compiler standard version C17 (__STDC_VERSION__ 201710L) used
* GCC compiler version (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0

### Makefile
* Use GCC compiler toolchain
* GNU Make utility version 4.2.1
* Includes the standard C math library (-lm)
* Completely disable most optimisations
* Produces no debug information
* Turned on most, but not all compiler warnings
* Turned on warn for implicit conversions that may alter a value
* Rule to only recompile code when any source code changed
* Rule for cleaning

# Compilation

The application can be compiled using the `make` command as follows:
```
$make
gcc -O0 -g0 -Wall -Wconversion   -c -o solution.o solution.c
gcc -O0 -g0 -Wall -Wconversion   -c -o binary.o binary.c
gcc -O0 -g0 -Wall -Wconversion -o solution solution.o binary.o -lm
...
```

# Usage
The application was written to allow the user to control parameters within the application through the use of arguments in-order to easily evaluate the performance of the alternatives considered in the approach.

The help message to the application can be invoked by using the argument `-h` as follows
```
$ ./solution -h
./solution: utility to find closest vehicle to given co-ordinates
usage: ./solution [-a]
-a        algorithm to use [1-slow 2-fast 3-fast-eucladian].
```
The application can be execute with no argument where in the Euclidian distance formula is used based on the sequentially optimised algorithm
```
$ ./solution
or $ ./solution -a 4
```
The slowest of the algorithms that is based on the Harvesine distance formula can be executed as follows
```
$ ./solution -a 1
```
The sequentially optimised algorithm that is based on the Harvesine distance formula can be executed as follows
```
$ ./solution -a 2
```
The sequentially optimised algorithm that is based on the Pythegorean distance formula can be executed as follows
```
$ ./solution -a 3
```

# Results
```
# ./solution -a 1
cpu-time (reading binary file): 0.000135 s
cpu-time (algorithm): 5.934566 s
# vehicle nearest to co-ordinate 1 is at position ID 1017652 (40075.017000 km away, lat:34.545033, lon:-102.101509)
# vehicle nearest to co-ordinate 2 is at position ID 2000000 (399.966291 km away, lat:35.020683, lon:-96.232727)
# vehicle nearest to co-ordinate 3 is at position ID 2000000 (416.795714 km away, lat:35.020683, lon:-96.232727)
# vehicle nearest to co-ordinate 4 is at position ID 2000000 (82.719882 km away, lat:35.020683, lon:-96.232727)
# vehicle nearest to co-ordinate 5 is at position ID 2000000 (376.270958 km away, lat:35.020683, lon:-96.232727)
# vehicle nearest to co-ordinate 6 is at position ID 2000000 (564.228375 km away, lat:35.020683, lon:-96.232727)
# vehicle nearest to co-ordinate 7 is at position ID 2000000 (379.173344 km away, lat:35.020683, lon:-96.232727)
# vehicle nearest to co-ordinate 8 is at position ID 2000000 (458.344509 km away, lat:35.020683, lon:-96.232727)
# vehicle nearest to co-ordinate 9 is at position ID 2000000 (211.646122 km away, lat:35.020683, lon:-96.232727)
# vehicle nearest to co-ordinate 10 is at position ID 2000000 (482.036466 km away, lat:35.020683, lon:-96.232727)


$ ./solution -a 2
cpu-time (reading binary file): 0.000137 s
cpu-time (algorithm): 3.799461 s
# vehicle nearest to co-ordinate 1 is at position ID 1017652 (0.062534 km away, lat:34.545033, lon:-102.101509)
# vehicle nearest to co-ordinate 2 is at position ID 609817 (0.249706 km away, lat:32.347542, lon:-99.121910)
# vehicle nearest to co-ordinate 3 is at position ID 230140 (0.146053 km away, lat:33.233234, lon:-100.215141)
# vehicle nearest to co-ordinate 4 is at position ID 284382 (0.072297 km away, lat:35.196331, lon:-95.349228)
# vehicle nearest to co-ordinate 5 is at position ID 1724293 (0.085459 km away, lat:31.895187, lon:-97.789093)
# vehicle nearest to co-ordinate 6 is at position ID 1442878 (0.208791 km away, lat:32.895908, lon:-101.787338)
# vehicle nearest to co-ordinate 7 is at position ID 108054 (0.038982 km away, lat:34.115654, lon:-100.225372)
# vehicle nearest to co-ordinate 8 is at position ID 1247417 (0.158088 km away, lat:32.334568, lon:-99.991478)
# vehicle nearest to co-ordinate 9 is at position ID 87700 (0.156121 km away, lat:33.536709, lon:-94.791862)
# vehicle nearest to co-ordinate 10 is at position ID 282385 (0.224669 km away, lat:32.236225, lon:-100.221809)

$ ./solution -a 3
# vehicle nearest to co-ordinate 1 is at position ID 1017652 (0.061502 km away, lat:34.545033, lon:-102.101509)
# vehicle nearest to co-ordinate 2 is at position ID 609817 (0.164715 km away, lat:32.347542, lon:-99.121910)
# vehicle nearest to co-ordinate 3 is at position ID 230140 (0.112564 km away, lat:33.233234, lon:-100.215141)
# vehicle nearest to co-ordinate 4 is at position ID 284382 (0.048300 km away, lat:35.196331, lon:-95.349228)
# vehicle nearest to co-ordinate 5 is at position ID 1724293 (0.059321 km away, lat:31.895187, lon:-97.789093)
# vehicle nearest to co-ordinate 6 is at position ID 1442878 (0.208691 km away, lat:32.895908, lon:-101.787338)
# vehicle nearest to co-ordinate 7 is at position ID 108054 (0.035065 km away, lat:34.115654, lon:-100.225372)
# vehicle nearest to co-ordinate 8 is at position ID 915004 (0.089192 km away, lat:32.337315, lon:-99.992401)
# vehicle nearest to co-ordinate 9 is at position ID 87700 (0.090852 km away, lat:33.536709, lon:-94.791862)
# vehicle nearest to co-ordinate 10 is at position ID 282385 (0.124253 km away, lat:32.236225, lon:-100.221809)
wall-time: execution took 2.229362 s

$ ./solution -a 4
# position ID 1017652 cpu-time (clock()): 3.962559 s / wall-time 0.502507 s
# vehicle nearest to co-ordinate 1 is at position ID 1017652 (0.000677 km away, lat:34.545033, lon:-102.101509)
# position ID 87700 cpu-time (clock()): 3.951474 s / wall-time 0.501901 s
# vehicle nearest to co-ordinate 9 is at position ID 87700 (0.001419 km away, lat:33.536709, lon:-94.791862)
# position ID 609817 cpu-time (clock()): 3.995188 s / wall-time 0.507625 s
# vehicle nearest to co-ordinate 2 is at position ID 609817 (0.002338 km away, lat:32.347542, lon:-99.121910)
# position ID 108054 cpu-time (clock()): 4.051441 s / wall-time 0.514823 s
# vehicle nearest to co-ordinate 7 is at position ID 108054 (0.000404 km away, lat:34.115654, lon:-100.225372)
# position ID 282385 cpu-time (clock()): 4.484888 s / wall-time 0.588267 s
# vehicle nearest to co-ordinate 10 is at position ID 282385 (0.002032 km away, lat:32.236225, lon:-100.221809)
# position ID 284382 cpu-time (clock()): 4.689148 s / wall-time 0.628631 s
# vehicle nearest to co-ordinate 4 is at position ID 284382 (0.000677 km away, lat:35.196331, lon:-95.349228)
# position ID 1724293 cpu-time (clock()): 4.723861 s / wall-time 0.636750 s
# vehicle nearest to co-ordinate 5 is at position ID 1724293 (0.000809 km away, lat:31.895187, lon:-97.789093)
# position ID 230140 cpu-time (clock()): 4.844273 s / wall-time 0.673994 s
# vehicle nearest to co-ordinate 3 is at position ID 230140 (0.001427 km away, lat:33.233234, lon:-100.215141)
# position ID 1247417 cpu-time (clock()): 4.729184 s / wall-time 0.664399 s
# vehicle nearest to co-ordinate 8 is at position ID 1247417 (0.001478 km away, lat:32.334568, lon:-99.991478)
# position ID 1442878 cpu-time (clock()): 4.792767 s / wall-time 0.701869 s
# vehicle nearest to co-ordinate 6 is at position ID 1442878 (0.002236 km away, lat:32.895908, lon:-101.787338)
```
The results show that both shortest distance formulas return very similar results with the exception of the difference in the position ID of co-ordinate 8 influenced by the apparent error difference.

# Observations and conclusions
The results show that the closest position distance (position ID) to co-ordinate 8 differs for the Harvesine and Euclidean formula. In comparison, the distances calculated vary by a considerable margin which can be significant for co-ordinates that maybe close to each other. It should also be noted that decimal rounding and data type conversion errors in code can affects the accuracy for both formula. Considering the results above, both formula return the same position ID's for 9 of the 10 given position. However, the resultant distances calculated by both formulas are different for each of the 10 given reference positions.

The Euclidian distance formula is easier to mathematically compute when compared to the optimised Harvesince distance formula, however no execution time (wall time) improvement was observed. It is apparent that the major difference between the two is the computational accuracy than computational complexity for this challenge. The sequentially optimised algorithm where in the binary file is only read once and the shortest distance is calculated for all 10 reference coordinates showed an improvement in the execution time of the application.

The Pythagoras theorem based distance formula achieved about 60% reduction in cpu time improvement from 2.5 seconds down to 1 second. 9 of the 10 position ID solutions are identical to the Euclidean distance based formular besides the position ID solution for coordinate 8. It is observable that the choice of distance calculation formula and algorithm will depend on the accuracy required and how fast the solution needs to be calculated.

The threaded implementation spawns 10 concurrent threads, each with its own file stream and a single unique coordinate of which the closest distance is required. Each thread read through the entire file, calculates and prints out the shortest distance for the respective coordinate before existing.

In order to shorten the execution time further, more effort should be spent to reduce the binary data file search complexity (i.e. optimised sequential access or concurrency). This could be potentially achieved by revising the threaded solution. Alternatively, some clever sorting based algorithm could be used on the binary data to ensure that only relevant closest candidate co-ordinates are considered.

# Bugs(s):
The threaded approach correctly calculates the position ID's however the CPU time calculated using the clock() method prints values in the 4 seconds region for each thread. I do not know why this is however I implemented a wall time alternative using gettimeofday() and the results show the threaded approach achieved 0.5 ~ 0.7 seconds execution time no withstanding the fact the OS may prioritise any thread in a manner that may increase the time slightly above a second.

[Back To Top](#vehicle-position-challenge)
