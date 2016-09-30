//http://stackoverflow.com/questions/13018790/c-mpi-array-passing
#include "mpi.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>

    //max size of the data array to split up
    #define MAXSIZE 1000000

    //methods
    int checkInput(int nprocs, int argc, char *argv[], int id);

    //mpi send & rec tags
    int ARSIZE = 0;     //array size
    int ARR = 1;        //array
    int MSM = 2;        //slave sum

    int main(int argc, char *argv[]) {

        int     ARsize;             /*size of the array to pre-sum*/
        int     id;                 /*process id number*/
        int     nprocs;             /*number of processors*/
        int     i, j, k;            /*counters*/
        int     muhsize;            /*size of personal array to calculate*/
        int     * muhray;           /**/

        //MPI framework
        MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
        MPI_Comm_rank(MPI_COMM_WORLD, &id);
        MPI_Barrier(MPI_COMM_WORLD);

        //pull input, check values, return ARsize
        ARsize = checkInput(nprocs, argc, argv, id);

        //set up array, serial run, send out chunks
        if (!id) {      

            //variables only the zero node needs
            int     data[ARsize];               /*full original array of numbers*/
            int     chunkSize, upper, lower;    /*vars to determine cunksize to send out*/
            int     smoothCount = 0;            /*BOOL for uneven division chunksize*/

            //fill array with numbers
            for (i = 0; i < ARsize; i++) {
                data[i] = 1;
            }

            //sequential solution here      


            //determine chunkSize
            chunkSize = (int) (ARsize/nprocs);  
            if (ARsize % nprocs != 0) {
                chunkSize = chunkSize + 1;
                smoothCount = 1;
            }

            //send chunks of data to procs
            for (i = 0; i < nprocs; i++) {          
                lower = i * chunkSize;
                upper = ((i+1) * chunkSize) - 1;
                if (i == nprocs-1 && smoothCount == 1) {
                    upper = ARsize-1;
                }
                int intarray[(upper-lower)];
                for (k = lower, j = 0; k <= upper; k++, j++) {
                    intarray[j] = data[k];   
                    }
                if(i > 0) {
                    //send array size
                    MPI_Send(&j, 1, MPI_INT, i, ARSIZE, MPI_COMM_WORLD);
                    //send actual array
                    MPI_Send(intarray, j, MPI_INT, i, ARR, MPI_COMM_WORLD); 
                }
                //zero no send to self, this data used later for all nodes calc
                else {
                    muhsize = j;
                    int muhray[muhsize];
                    for (j = 0; j <= chunkSize; j++) {
                        muhray[j] = intarray[j];
                    }   
                    printf("!! proc%d's array is size %d and goes from %d to %d\n", id, muhsize, muhray[0], muhray[(muhsize-1)]);
                }
            }       
        }
        else {

            MPI_Recv(&muhsize, 1, MPI_INT, 0, ARSIZE, MPI_COMM_WORLD, &status);
            int muhray[muhsize];
            MPI_Recv(muhray, muhsize, MPI_INT, 0, ARR, MPI_COMM_WORLD, &status);
            printf("!! proc%d's array is size %d and goes from %d to %d\n", id, muhsize, muhray[0], muhray[(muhsize-1)]);
            fflush(stdout);
        }

        printf("proc%d's array is size %d and goes from %d to %d\n", id, muhsize, muhray[0], muhray[muhsize]);
        fflush(stdout);
        //MPI_Send(&muhsize, 1, MPI_INT, 0, MSM, MPI_COMM_WORLD);       


        MPI_Finalize();

    }

//pull input, check values, return ARsize
int checkInput(int nprocs, int argc, char *argv[], int id) {

    int size;

    if (nprocs % 2 != 0 || nprocs == 6 || nprocs > 8) {
        if (!id) printf("run with 2^k procs, (1 >= k <= 3)\n");  
        fflush(stdout);
        MPI_Finalize();
        exit(1);
    }
    if (argc != 2) {
        if (!id) printf("Usage: presum [array size (max: %d)]\n", MAXSIZE);
        fflush(stdout);
        MPI_Finalize();
        exit(1);        
    }
    size = atoi(argv[1]);
    if (size <= nprocs) {
        if (!id) printf("search range must be greater than processor count\n");
        fflush(stdout);
        MPI_Finalize();
        exit(1);
    }
    if (size > MAXSIZE) {
        if (!id) printf("array size must be less than or equal to %d\n", MAXSIZE);
        fflush(stdout);
        MPI_Finalize();
        exit(1);
    }
    return size;
}