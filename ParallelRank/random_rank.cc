// Author: Wes Kendall
// Copyright 2013 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// Runs the TMPI_Rank function with random input.
//

#include <cstdio>
#include <mpi.h>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "tmpi_rank.hpp"

int main(int argc, char **argv) {
	MPI_Init(NULL, NULL);

	const int master = 0;
	int world_size, world_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	srand(2017 * world_rank);

	float rand_num = rand() / (float)RAND_MAX;
	int rank;
	TMPI_Rank(&rand_num, &rank, MPI_FLOAT, MPI_COMM_WORLD);

	std::cout <<"Rank for " << rand_num << " on process " << world_size <<" - " << rank << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}