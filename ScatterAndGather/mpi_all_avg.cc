// Author: Wes Kendall
// Copyright 2012 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// Program that computes the average of an array of elements in parallel using
// MPI_Scatter and MPI_Gather
//
#include <iostream>
#include <mpi.h>
#include <ctime>
#include <cstdlib>
#include <vector>

using std::vector;

vector<float> create_random_float(int n) {
	vector<float> ans(n);
	for (int i = 0; i != n; i++) {
		ans[i] = rand() / (float)RAND_MAX;
	}
	return ans;
}

float compute_avg(const vector<float>& v) {
	float sum = 0;
	for (int i = 0; i != v.size(); i++) {
		sum += v[i];
	}
	return sum / v.size();
}

int main(int argc, char** argv) {
	const int master = 0;
	int world_size, world_rank;
	if (argc != 2) {
		std::cerr << "Usage: avg num_elements_per_proc" << std::endl;
		return 1;
	}

	int num_elements_per_proc = atoi(argv[1]);
	srand(2017);
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	vector<float> rand_nums;
	if (world_rank == master) {
		rand_nums = create_random_float(num_elements_per_proc * world_size);
	}

	vector<float> sub_rand_nums = vector<float>(num_elements_per_proc);

	MPI_Scatter(rand_nums.data(), num_elements_per_proc, MPI_FLOAT, sub_rand_nums.data(), num_elements_per_proc, MPI_FLOAT, master, MPI_COMM_WORLD);
	float sub_avg = compute_avg(sub_rand_nums);

	vector<float> sub_avgs(world_size);
	MPI_Allgather(&sub_avg, 1, MPI_FLOAT, sub_avgs.data(), 1, MPI_FLOAT, MPI_COMM_WORLD);

	float avg = compute_avg(sub_avgs);
	std::cout <<" avg of all elements is " << avg << std::endl;
	if (world_rank == master) {
		float original_data_avg = compute_avg(rand_nums);
		std::cout <<" original avg of all elements is " << original_data_avg << std::endl;
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}