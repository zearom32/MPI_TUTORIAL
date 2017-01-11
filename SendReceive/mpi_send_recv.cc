// Author: Wes Kendall
// // Copyright 2011 www.mpitutorial.com
// // This code is provided freely with the tutorials on mpitutorial.com. Feel
// // free to modify it for your own use. Any distribution of the code must
// // either provide a link to www.mpitutorial.com or keep this header intact.
// //
// // MPI_Send, MPI_Recv example. Communicates the number -1 from process 0
// // to process 1.
// //

#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int world_rank, world_size;
    const int master = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size < 2) {
        std::cerr << "World size must be greater than 1 for "<< argv[0] << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int number;
    if (world_rank == master) {
        number = -1;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (world_rank == 1) {
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process 1 received number " << number << " from master" << std::endl;
    }

    return 0;
}

