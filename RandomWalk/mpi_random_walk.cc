// Author: Wes Kendall
// // Copyright 2011 www.mpitutorial.com
// // This code is provided freely with the tutorials on mpitutorial.com. Feel
// // free to modify it for your own use. Any distribution of the code must
// // either provide a link to www.mpitutorial.com or keep this header intact.
// //
// // Example of checking the MPI_Status object from an MPI_Recv call
// //
//
#include <iostream>
#include <mpi.h>
#include <cstdio>
#include <cstdlib>
#include <vector>

using std::vector;
typedef struct {
    int location;
    int left_steps;
} Walker;


void decompose_domain(int domain_size, int world_rank, int world_size, int* subdomain_start, int* subdomain_size) {
    if (world_size > domain_size) {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    *subdomain_start = domain_size / world_size * world_rank;
    *subdomain_size = domain_size / world_size;
    if (world_rank == world_size - 1) {
        *subdomain_size += domain_size % world_size;
    }
}

void initialize_walker(int num_walkers_per_proc, int max_walk_size, int subdomain_start, int subdomain_size, vector<Walker>& incoming_walkers) {
    Walker walker;
    for (int i = 0; i< num_walkers_per_proc; i++) {
        walker.location = subdomain_start;
        walker.left_steps = (rand() / (float)RAND_MAX) * max_walk_size;
        incoming_walkers.push_back(walker);
    }
}

void walk(Walker& walker, int subdomain_start, int subdomain_size, int domain_size, vector<Walker>& outgoing_walkers) {
    while (walker.left_steps) {
        if (walker.location == subdomain_start + subdomain_size) {
            walker.location %= domain_size;
            outgoing_walkers.push_back(walker);
            break;
        } else {
            walker.left_steps --;
            walker.location ++;
        }
    }
}
void send_outgoing_walkers(vector<Walker>& outgoing_walkers, int world_rank, int world_size) {
    MPI_Send((void*)outgoing_walkers.data(), outgoing_walkers.size() * sizeof(Walker), MPI_BYTE, (world_rank + 1) % world_size, 0, MPI_COMM_WORLD);
    outgoing_walkers.clear();
}

void receive_incoming_walkers(vector<Walker>& incoming_walkers, int world_rank, int world_size) {
    MPI_Status status;

    int incoming_rank = (world_rank - 1 + world_size) % world_size;
    MPI_Probe(incoming_rank, 0, MPI_COMM_WORLD, &status);

    int incoming_walker_size;
    MPI_Get_count(&status, MPI_BYTE, &incoming_walker_size);
    incoming_walkers.resize(incoming_walker_size / sizeof(Walker));
    MPI_Recv((void*)incoming_walkers.data(), incoming_walker_size, MPI_BYTE, incoming_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}
int main(int argc, char** argv) {

    int domain_size;
    int max_walk_size;
    int num_walkers_per_proc;

    if (argc < 4) {
        std::cerr << "Usage: random_walk, domain_size, max_walk_size " << " num_walkers_per_proc" << std::endl;
        return 1;
    }
    domain_size = atoi(argv[1]);
    max_walk_size = atoi(argv[2]);
    num_walkers_per_proc = atoi(argv[3]);
    const int master = 0;
    int world_rank, world_size;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    srand(2017);

    int subdomain_start, subdomain_size;
    vector<Walker> incoming_walkers, outgoing_walkers;

    decompose_domain(domain_size, world_rank, world_size, &subdomain_start, &subdomain_size);

    initialize_walker(num_walkers_per_proc, max_walk_size, subdomain_start, subdomain_size, incoming_walkers);
    std::cout << "Process " << world_rank <<" initiated " << num_walkers_per_proc
              << " walkers in subdomain " << subdomain_start <<" - "
              << subdomain_start + subdomain_size -1 << std::endl;

    int maximum_sends_recvs = max_walk_size / (domain_size / world_size) + 1;

    for (int m = 0; m < maximum_sends_recvs; m++) {
        for (int i = 0; i < incoming_walkers.size(); i++) {
            walk(incoming_walkers[i], subdomain_start, subdomain_size, domain_size, outgoing_walkers);
        }

        std::cout << "Process " << world_rank <<" sending " << outgoing_walkers.size()
                   << " outgoing walkers to process " << (world_rank + 1) % world_size << std::endl;

        if (world_rank % 2 == 0) {
            send_outgoing_walkers(outgoing_walkers, world_rank, world_size);
            receive_incoming_walkers(incoming_walkers, world_rank, world_size);
        } else {
            receive_incoming_walkers(incoming_walkers, world_rank, world_size);
            send_outgoing_walkers(outgoing_walkers, world_rank, world_size);
        }
        std::cout << "Process " << world_rank <<" received " << incoming_walkers.size()
                << " incoming walkers" << std::endl;
    }

    std::cout <<"Process " << world_rank <<" done" << std::endl;
    MPI_Finalize();
    return 0;
}
