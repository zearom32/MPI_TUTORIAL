// Author: Wes Kendall
// // Copyright 2011 www.mpitutorial.com
// // This code is provided freely with the tutorials on mpitutorial.com. Feel
// // free to modify it for your own use. Any distribution of the code must
// // either provide a link to www.mpitutorial.com or keep this header intact.
// //
// // Example of checking the MPI_Status object from an MPI_Recv call
// //

#include <mpi.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char** argv) {
    const int master = 0;
    int world_size, world_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int number_amount;
    if (world_rank == master) {
        const int MAX_NUMBERS = 100;
        int numbers[MAX_NUMBERS];
        srand(2017);
        number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;
        MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
        std::cout << master <<" sent " << number_amount <<" numbers to 1" << std::endl;
    } else if (world_rank == 1) {
        MPI_Status status;

        MPI_Probe(master, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &number_amount);
        
        int* number_buf = new int[number_amount];
        MPI_Recv(number_buf, number_amount, MPI_INT, master, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << world_rank <<" dynamically received " << number_amount << " numbers from " << master << std::endl;
        delete [] number_buf;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
