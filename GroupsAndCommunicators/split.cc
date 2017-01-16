// Author: Wesley Bland
// Copyright 2015 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// Example using MPI_Comm_split to divide a communicator into subcommunicators
//

#include <cstdio>
#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
	int world_size, world_rank;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int color = world_rank / 4;

	MPI_Comm row_comm;
	MPI_Comm_split(MPI_COMM_WORLD, color, world_rank, &row_comm);

	int row_size, row_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &row_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &row_size);

	std::cout << "WORLD RANK/SIZE: " << world_rank << "/" << world_size
	<< " --- ROW RANK/SIZE: " << row_rank << "/" << row_size << std::endl;

	MPI_Comm_free(&row_comm);
	MPI_Finalize();
}