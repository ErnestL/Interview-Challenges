# Interview-Challenges

## Collection of interview challenges:

- [Vehicle-Positions](https://github.com/ErnestL/Interview-Challenges/tree/main/Vehicle-Positions/README.md)

# Problem Solving Approach:

A fairly accurate but computationally more expensive approach to calculating distances between two points using latitude and longitude is by using the Haversine formula. The approach taken here was rather to use another formula that was found to be accurate to within a few metres and computationally less expensive. Although this method of computation is known to produce increasingly higher levels of errors with increasing distances between two points it was found to be adequate for our purposes.

Another approach that was taken to reduce search time was through the use of multi-threading. Two seperate threads were spawned from our main application whereby each thread would only be responsible for searching through one half of the file data i.e. the first thread would search from the start of the file data until mid-point while the second thread would start from mid-point until the end of the file data.

# Assumptions:

Due to the way the search algorithm works you will find that should identical coordinates exist then only the most recent coordinates will be be stored. No mention of any coding standards was maentioned and therefore assumed carte blanche.

The code was compiled using the C11 language standard on an 64-bit Ubuntu machine using the GNU GCC C compiler version 9.4.0

Compiler optimizations were disabled for both debug and release versions as stipulated.

When executed, the program(solution) will display all of the ten supplied coordinates along with the Position ID extracted from the positions.dat file of the closest point.

# Shortcomings:

Unfortunately the code currently won't scale beyond two threads due to the way it was written. More thought would need to be applied here although modest gains were observed when compared to the single threaded version.

#

All code was also checked and compiled on a 32-bit Ubuntu 16.04 virtual machine running GNU GCC C compiler version 5.4.0

The makefile was written to be reasonably generic and accomodates for both debug and release builds through command-line variables.
