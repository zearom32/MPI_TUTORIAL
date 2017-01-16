// Author: Wesley Bland
// Copyright 2015 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// Example using MPI_Comm_split to divide a communicator into subcommunicators
//

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
	const int master = 0;
	int world_size, world_rank;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	MPI_Group world_group;
	MPI_Comm_group(MPI_COMM_WORLD, &world_group);

	int n = 7;
	const int ranks[] = {1, 2, 3, 5, 7, 11, 13};

	MPI_Group prime_group;
	MPI_Group_incl(world_group, 7, ranks, &prime_group);

	MPI_Comm prime_comm;
	MPI_Comm_create_group(MPI_COMM_WORLD, prime_group, 0, &prime_comm);

	int prime_rank = -1, prime_size = -1;
	if (MPI_COMM_NULL != prime_comm) {
		MPI_Comm_rank(prime_comm, &prime_rank);
		MPI_Comm_size(prime_comm, &prime_size);
	}

	std::cout <<"WORLD RANK/SIZE: " << world_rank << "/" << world_size
	<< " --- PRIME RANK/SIZE: " << prime_rank << "/" << prime_size << std::endl;

	MPI_Group_free(&world_group);
	MPI_Group_free(&prime_group);

	if (MPI_COMM_NULL != prime_comm) {
		MPI_Comm_free(&prime_comm);
	}

	MPI_Finalize();
	return 0;
}