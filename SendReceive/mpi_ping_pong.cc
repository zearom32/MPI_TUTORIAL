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
    const int PING_PONG_LIMIT = 10;
    int world_size, world_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size != 2) {
        std::cerr << argv[0] << ": World size must be 2 since we are playing ping pong" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int ping_pong_count = 0;
    int partner_rank = world_rank ^ 1;

    // master should send the ping_pong_count firstly
   if (world_rank == master) {
       MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
       std::cout <<"master send out the first ping_pong_count" << std::endl;
   } 

    while (ping_pong_count < PING_PONG_LIMIT) {
            MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << world_rank << " received ping_pong_count " << ping_pong_count << " from " << partner_rank << std::endl;
            ping_pong_count ++;
            MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
            std::cout << world_rank << " sent and incremented ping_pong_count " << ping_pong_count << " to " << partner_rank << std::endl;
    }
    MPI_Finalize();
}
