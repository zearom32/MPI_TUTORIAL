#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>

const int master = 0;

void my_bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator) {
	int world_size, world_rank;
	MPI_Comm_size(communicator, &world_size);
	MPI_Comm_rank(communicator, &world_rank);
	int re_rank = (world_rank - root + world_size) % world_size;
	int add = 1;
	while(re_rank + add < world_size) {
		if (add <= re_rank && add * 2 > re_rank) {
			int coming = (world_rank - add + world_size) % world_size;
			MPI_Recv(data, count, datatype, coming, 0, communicator, MPI_STATUS_IGNORE);
		}
		if (add > re_rank) {
			int going = (world_rank + add) % world_size;
			MPI_Send(data, count, datatype, going, 0, communicator);
		}
		add = add * 2;
	}
}

int main(int argc, char** argv) {
	if (argc != 3) {
		std::cout << "Usage: broadcast, num_elements num_trails" << std::endl;
		return 1;
	}
	int num_elements = atoi(argv[1]);
	int num_trails = atoi(argv[2]);
	MPI_Init(NULL, NULL);
	int world_size, world_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	double total_my_bcast_time = 0;
	double total_mpi_bcast_time = 0;
	int* data = new int[num_elements];

	srand(time(NULL));
	int root = rand() % world_size;
	for (int i = 0; i < num_trails; i++) {
		MPI_Barrier(MPI_COMM_WORLD);
		total_my_bcast_time -= MPI_Wtime();
		my_bcast(data, num_elements, MPI_INT, root, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
		total_my_bcast_time += MPI_Wtime();

		MPI_Barrier(MPI_COMM_WORLD);
		total_mpi_bcast_time -= MPI_Wtime();
		MPI_Bcast(data, num_elements, MPI_INT, root, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
		total_mpi_bcast_time += MPI_Wtime();
	}

	if (world_rank == root) {
		std::cout <<"Data size = " << num_elements * sizeof(int)
		<<", Trials = " << num_trails << std::endl;
		std::cout <<"Avg my_bcast time = " << total_my_bcast_time / num_trails << std::endl;
		std::cout <<"Avg mpi_bcast time = " << total_mpi_bcast_time / num_trails << std::endl;
	}

	delete [] data;
	MPI_Finalize();
	return 0;
}
