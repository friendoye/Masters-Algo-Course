#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

const int MSG_SEND_BLOCK_NUM               = 15;
const int MSG_WORKER_READY                 = 16;
const int MSG_SEND_LOCAL_RESULT            = 17;
const int MSG_SEND_LOCAL_SUM_AND_TERMINATE = 18;

float A;
float B;
int N;
int R;
int blocks_amount;

float f(float x) {
    return x * x;
}

float integral_f(float a, float b) {
    return b * b * b / 3 - a * a * a / 3;
}

int parse_args(int world_rank, int world_size, int argc, char** argv) {
    if (argc < 5) {
        if (world_rank == 0) {
            printf("Lower and Upper bounds, N, R should be specified!\n");
        }
    } else {
        A = atof(argv[1]);
        B = atof(argv[2]);
        N = atoi(argv[3]);
        R = atoi(argv[4]);
        blocks_amount = N / R + (N % R > 0);
        if (blocks_amount <= world_size) {
            if (world_rank == 0) {
                printf("Blocks amount(%d) should be greater than number of processes(%d)!\n", blocks_amount, world_size);
            }
        } else {
            return 0;
        }
    }
    return -1;
}

float trapezium_method(int block_num) {
    float local_result = 0;
    float dx = (B - A) / N;
    int a = R * block_num;
    int b;
    if (a + R > N) {
        b = N;
    } else {
        b = a + R;
    }

    for (int t = a; t < b; t++) {
        float f_left  = f(A + t * dx);
        float f_right = f(A + (t + 1) * dx);
        local_result += (f_left + f_right) / 2 * dx;
    }

    return local_result;
}

int main(int argc, char** argv) {
    int world_rank, world_size;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Parse args
    if (parse_args(world_rank, world_size, argc, argv) != 0) {
        MPI_Finalize();
        return 0;
    }
    
    float sum = 0.0;
    
    // Start MPI routine
    if (world_rank == 0) {
        // Master listen for available process
        for (int k = 0; k < blocks_amount; k++) {
            MPI_Recv(NULL, 0, MPI_CHAR, MPI_ANY_SOURCE, 
                     MSG_WORKER_READY, MPI_COMM_WORLD, &status);
            int dest = status.MPI_SOURCE;
            MPI_Send(&k, 1, MPI_INT, dest, 
                     MSG_SEND_BLOCK_NUM, MPI_COMM_WORLD);
        }

        // Say workers to send back results and terminate
        for (int k = 1; k < world_size; k++) {
            MPI_Send(0, 0, MPI_INT, k, 
                     MSG_SEND_LOCAL_SUM_AND_TERMINATE, MPI_COMM_WORLD);
        }

        // Gather local sums
        for (int k = 1; k < world_size; k++) {
            float local_sum;
            MPI_Recv(&local_sum, 1, MPI_FLOAT, MPI_ANY_SOURCE,
                     MSG_SEND_LOCAL_RESULT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += local_sum;
        }

        printf("MPI sum:    %.8f\n", sum);
        printf("Actual sum: %.8f\n", integral_f(A, B));
    } else {
        int block_num;
        float result = 0.0f;
        do {
            // Notify master that we're ready to compute
            MPI_Send(NULL, 0, MPI_CHAR, 0, 
                     MSG_WORKER_READY, MPI_COMM_WORLD);
            MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (status.MPI_TAG == MSG_SEND_BLOCK_NUM) {
                MPI_Recv(&block_num, 1, MPI_INT, 0, 
                         MSG_SEND_BLOCK_NUM, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                result += trapezium_method(block_num);
            } else {
                // If status.MPI_TAG == MSG_SEND_LOCAL_SUM_AND_TERMINATE,
                // that send local sum and terminate.
                break;
            }
        } while (true);
        MPI_Send(&result, 1, MPI_FLOAT, 0, 
                 MSG_SEND_LOCAL_RESULT, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}