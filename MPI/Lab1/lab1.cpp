#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MSG_SEND_STRING = 33;

int main(int argc, char** argv) {
    int world_rank, world_size;
    int count, source;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_rank != 0) {
        // Prepare data to send
        char str[50];
        sprintf(str, "Hello from %d process! -> ", world_rank);
        for (int i = 0; i < world_rank; i++) {
            strcat(str, "*");
        }
        
        // Send data to 0 process
        MPI_Send(str, (int) strlen(str) + 1, MPI_CHAR, 
                 0, MSG_SEND_STRING, MPI_COMM_WORLD);
    } else {
        printf("===== START MAIN PROCESS =====\n\n");

        // Prepare result 2D char array
        char** result = (char**) malloc(sizeof(char*) * (world_size - 1));

        // Listen for first incoming message and get info from it 
        for (int i = 1; i < world_size; i++) {    
            MPI_Probe(MPI_ANY_SOURCE, MSG_SEND_STRING, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_CHAR, &count);
            source = status.MPI_SOURCE;
            result[source - 1] = (char*) malloc(sizeof(char) * count);
            MPI_Recv(result[source - 1], count, MPI_CHAR, source, 
                     MSG_SEND_STRING, MPI_COMM_WORLD, &status);
            printf("Receive string from %d process: %d symbols\n", source, count);
        }

        // Show results
        printf("\n===== RPINTING RESULTS =====\n\n");
        for (int i = 0; i < world_size - 1; i++) {
            printf("Process %d: %s\n", (i + 1), result[i]);
        }

        // Free memory
        free(result);

        printf("\n=====      ENDING      =====\n");
    }

    MPI_Finalize();
    return 0;
}