// Author: Wes Kendall
// // Copyright 2011 www.mpitutorial.com
// // This code is provided freely with the tutorials on mpitutorial.com. Feel
// // free to modify it for your own use. Any distribution of the code must
// // either provide a link to www.mpitutorial.com or keep this header intact.
// //
// // Example using MPI_Send and MPI_Recv to pass a message around in a ring.
// //

#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    const int master = 0;
    MPI_Init(NULL, NULL);
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int token;

    if (world_rank != master) {
        MPI_Recv(&token, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process " << world_rank << " received token " << token << " from process " << world_rank-1 << std::endl;
        token++;
    } else {
        token = 0;
    }

    MPI_Send(&token, 1, MPI_INT, (world_rank + 1) % world_size, 0, MPI_COMM_WORLD);

    if (world_rank == master) {
        MPI_Recv(&token, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process " << world_rank << " received token " << token << " from process " << world_size - 1 << std::endl;
    }
    MPI_Finalize();
}
