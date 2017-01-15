// Author: Wes Kendall
// Copyright 2013 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// Header file for TMPI_Rank
//
#include <iostream>
#include <mpi.h>
#include <cstring>
#include <algorithm>
#include <vector>

using std::vector;

typedef struct {
	int comm_rank;
	union {
		float f;
		int i;
	} number;
} CommRankNumber;


struct CommRankNumberFloatCmp {

	int operator() (const CommRankNumber &a, const CommRankNumber &b) {
		return a.number.f < b.number.f;
	}
};

struct CommRankNumberIntCmp {

	int operator() (const CommRankNumber &a, const CommRankNumber &b) {
		return a.number.i < b.number.i;
	}
};

static const int master = 0;

void *gather_number_to_root(void *number, MPI_Datatype datatype, MPI_Comm comm) {
	int comm_rank, comm_size;
	MPI_Comm_rank(comm, &comm_rank);
	MPI_Comm_size(comm, &comm_size);

	int datatype_size;
	MPI_Type_size(datatype, &datatype_size);
	void *gathered_numbers;
	if (comm_rank == master) {
		gathered_numbers = new char[datatype_size * comm_size];
	}

	MPI_Gather(number, 1, datatype, gathered_numbers, 1, datatype, master, comm);
	return gathered_numbers;
}

vector<int> get_ranks(void *gathered_numbers, int gathered_number_count, MPI_Datatype datatype) {
	int datatype_size;
	MPI_Type_size(datatype, &datatype_size);

	CommRankNumber *comm_rank_numbers = new CommRankNumber[gathered_number_count];
	for (int i = 0; i < gathered_number_count; i++) {
		comm_rank_numbers[i].comm_rank = i;
		memcpy(&(comm_rank_numbers[i].number), gathered_numbers + (i * datatype_size), datatype_size);
	}

	if (datatype == MPI_FLOAT) {
		std::sort(comm_rank_numbers, comm_rank_numbers + gathered_number_count, CommRankNumberFloatCmp());
	} else {
		std::sort(comm_rank_numbers, comm_rank_numbers + gathered_number_count, CommRankNumberIntCmp());
	}

	vector<int> ranks =  vector<int>(gathered_number_count);
	for (int i = 0; i < gathered_number_count; i++) {
		ranks[comm_rank_numbers[i].comm_rank] = i;
	}

	delete [] comm_rank_numbers;
	return ranks;
}

int TMPI_Rank(void *send_data, void *recv_data, MPI_Datatype datatype, MPI_Comm comm) {
	if (datatype != MPI_INT && datatype != MPI_FLOAT) {
		return MPI_ERR_TYPE;
	}

	int comm_size, comm_rank;
	MPI_Comm_size(comm, &comm_size);
	MPI_Comm_rank(comm, &comm_rank);

	void* gathered_numbers = gather_number_to_root(send_data, datatype, comm);

	vector<int> ranks;
	if (comm_rank == master) {
		ranks = get_ranks(gathered_numbers, comm_size, datatype);
	}

	MPI_Scatter(ranks.data(), 1, MPI_INT, recv_data, 1, MPI_INT, master, comm);

	if (comm_rank == master) {
		delete [] gathered_numbers;
	}
}