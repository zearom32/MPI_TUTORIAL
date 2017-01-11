// Author: Wes Kendall
// // Copyright 2011 www.mpitutorial.com
// // This code is provided freely with the tutorials on mpitutorial.com. Feel
// // free to modify it for your own use. Any distribution of the code must
// // either provide a link to www.mpitutorial.com or keep this header intact.
// //
// // Example of checking the MPI_Status object from an MPI_Recv call
// //

#include <iostream>
#include <mpi.h>
#include <vector>
#include <ctime>
#include <cstdlib>

int main(int argc, char** argv) {
   const int master = 0; 
   int world_size, world_rank;
   MPI_Init(NULL, NULL);
   MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &world_size);

   const int MAX_NUMBERS = 100;
   int numbers[MAX_NUMBERS];

   if (world_rank == master) {
       // pick a random amount of integers to send to process 1
       srand(2017);
       int number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;
       MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
       std::cout << "send " <<  number_amount <<" numbers to 1" << std::endl;
   } else if (world_rank == 1) {
       MPI_Status status;
       int number_amount;
       MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, master, 0, MPI_COMM_WORLD, &status);
       MPI_Get_count(&status, MPI_INT, &number_amount);
       std::cout << world_rank <<" received " << number_amount << " numbers from master. Message source = " << status.MPI_SOURCE << ", tag = " << status.MPI_TAG << std::endl;
   }
   MPI_Barrier(MPI_COMM_WORLD);
   MPI_Finalize();
}
