// Author: Wes Kendall
// Copyright 2013 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// Program that computes the average of an array of elements in parallel using
// MPI_Reduce.

#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include <iostream>
#include <vector>

using std::vector;

vector<float> create_rand_nums(int num_elements) {
	vector<float> ans(num_elements);
	for (int i = 0; i < num_elements; i++) {
		ans[i] = rand() / (float)RAND_MAX;
	}
	return ans;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "Usage: avg num_elements_per_proc" << std::endl;
		return 1;
	}

	int num_elements_per_proc = atoi(argv[1]);
	int world_size, world_rank;
	const int master = 0;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	srand(2017 * world_rank);
	vector<float> rand_nums = create_rand_nums(num_elements_per_proc);

	float local_sum = 0;
	for (int i = 0; i < num_elements_per_proc; i++) {
		local_sum += rand_nums[i];
	}

	std::cout << "Local sum for process " << world_rank
				<< " - " << local_sum <<", avg =  " << local_sum / num_elements_per_proc
				<< std::endl;

	float global_sum;
	MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, master, MPI_COMM_WORLD);

	if (world_rank == master) {
		std::cout << "Total sum = " << global_sum
				  << ", avg = " << global_sum / (world_size * num_elements_per_proc)
				  << std::endl;
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
}