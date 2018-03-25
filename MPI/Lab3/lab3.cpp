#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

const int MSG_LAST_STRING_BLOCK = 33;
const int MSG_GATHERING         = 34;

int N1, N2;
int P, B; // Processes, Block size

int parse_args(int world_rank, int world_size, int argc, char** argv) {
    if (argc < 3) {
        if (world_rank == 0) {
            printf("N1, N2 and block size are not specified!\n");
        }
    } else {
        N1 = atoi(argv[1]);
        N2 = atoi(argv[2]);
        P = world_size;
        B = atoi(argv[3]);
        if (N1 % P) {
            if (world_rank == 0) {
                printf("N1(%d) should be divisible by number of processes(%d)!\n", N1, P);
            }
        } else if (N2 % B) {
            if (world_rank == 0) {
                printf("N2(%d) should be divisible by block size(%d)!\n", N2, B);
            }
        } else {
            return 0;
        }
    }
    return -1;
}

int main(int argc, char** argv) {
    // Preinit MPI
    int world_rank, world_size;
    int count;
    MPI_Status status;
    MPI_Request recv_request = MPI_REQUEST_NULL, send_request = MPI_REQUEST_NULL;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Parse args
    if (parse_args(world_rank, world_size, argc, argv) != 0) {
        MPI_Finalize();
        return 0;
    }
    
    int block_rows_amount = N1 / P;
    int blocks_amount = N2 / B;
    int* buffer = new int[B];
    int* block  = new int[block_rows_amount * N2];

    // Init neighbor proc nums
    int prev_p, next_proc;
    if (world_rank == 0) {
        prev_p = MPI_PROC_NULL;
    } else {
        prev_p = world_rank - 1;
    }

    if (world_rank == world_size - 1) {
        next_proc = MPI_PROC_NULL;
    } else {
        next_proc = world_rank + 1;
    }
    
    // Init initial matrix line
    if (world_rank == 0) {
        for (int i = 0; i < N2; i++) {
            block[i] = i;
        }
    }

    // MPI sequentially initializes matrix
    for (int i = 0; i < blocks_amount; i++) {
        if (world_rank != 0) {
            if (i == 0) {
                MPI_Recv(buffer, B, MPI_INT, prev_p, 
                         MSG_LAST_STRING_BLOCK, MPI_COMM_WORLD, &status);
            } else {
                MPI_Wait(&recv_request, &status);
            }

            for (int k = 0; k < B; k++) {
                block[i * B + k] = buffer[k] + 1;
            }

            MPI_Irecv(buffer, B, MPI_INT, prev_p, 
                      MSG_LAST_STRING_BLOCK, MPI_COMM_WORLD, &recv_request);
        }

        for (int j = 1; j < block_rows_amount; j++) {
            for (int k = 0; k < B; k++) {
                block[j * N2 + i * B + k] = block[(j - 1) * N2 + i * B + k] + 1;
            }
        }

        MPI_Isend(block + (block_rows_amount - 1) * N2 + i * B, B, 
                  MPI_INT, next_proc, 
                  MSG_LAST_STRING_BLOCK, MPI_COMM_WORLD, &send_request);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Gathering matrix
    if (world_rank != 0) {
        // Send data to 0 process
        MPI_Send(block, block_rows_amount * N2, MPI_INT, 
                 0, MSG_GATHERING, MPI_COMM_WORLD);
    } else {
        int* result = new int[N1 * N2];
        
        for (int i = 0; i < block_rows_amount * N2; i++) {
            result[i] = block[i];
        }
        
        int source;
        for (int i = 1; i < world_size; i++) {
            // Listen for first incoming message and get info from it 
            MPI_Probe(MPI_ANY_SOURCE, MSG_GATHERING, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &count);
            source = status.MPI_SOURCE;
            MPI_Recv(&result[source * block_rows_amount * N2], count, 
                     MPI_INT, source, 
                     MSG_GATHERING, MPI_COMM_WORLD, &status);
        }

        // Print result
        for (int i = 0; i < N1; i++) {
            for (int j = 0; j < N2; j++) {
                printf("%2d ", result[i * N2 + j]);
            }  
            printf("\n");          
        }
    }

    MPI_Finalize();
    return 0;
}