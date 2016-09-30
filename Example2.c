#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
        int rank;int buf;const int root=0;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        if(rank == root)
        {
        printf("please enter an integer that is positive and less than 100,000 (mm bs ls)\n");//This will determine number of random numbers will be evaluated by bubble sort, linear search, and matrix multiplication.\n");
        scanf("%d", &buf);//might need "x" instead

        if (buf < 0 || buf > 100000)
                {
                printf("invalid number entered\n");
                buf = -1;
                }
        }

        printf("[%d]: Before Bcast, buf is %d\n", rank, buf);

        /* everyone calls bcast, data is taken from root and ends up in everyone's buf */
        MPI_Bcast(&buf, 1, MPI_INT, root, MPI_COMM_WORLD);

        printf("[%d]: After Bcast, buf is %d\n", rank, buf);

        MPI_Finalize();
        return 0;
}
