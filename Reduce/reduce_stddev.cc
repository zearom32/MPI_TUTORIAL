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
#include <cmath>

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
		std::cerr <<"Usage: avg num_elements_per_proc" << std::endl;
		return 1;
	}

	int num_elements_per_proc = atoi(argv[1]);
	MPI_Init(NULL, NULL);

	int world_rank, world_size;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	const int master = 0;
	srand(2017 * world_rank);
	vector<float> rand_nums = create_rand_nums(num_elements_per_proc);

	float local_sum = 0;
	for (int i = 0; i < num_elements_per_proc; i++) {
		local_sum += rand_nums[i];
	}

	float global_sum;
	MPI_Allreduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
	float mean = global_sum / (num_elements_per_proc * world_size);

	float local_sq_diff = 0;
	for (int i = 0; i < num_elements_per_proc; i++) {
		local_sq_diff += (rand_nums[i] - mean) * (rand_nums[i] - mean);
	}

	float global_sq_diff;
	MPI_Reduce(&local_sq_diff, &global_sq_diff, 1, MPI_FLOAT, MPI_SUM, master, MPI_COMM_WORLD);


	if (world_rank == master) {
		float stddev = sqrt(global_sq_diff /
			(num_elements_per_proc * world_size));
		std::cout <<"Mean - " << mean <<" Standard deviation = " << stddev << std::endl;
	}


	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}