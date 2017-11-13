#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define LEN 256

main (int argc, char **argv)
{
    int nprocs, id, len ;
    char hostname [LEN+1] ;
    MPI_Init (&argc, &argv) ;
    MPI_Comm_size (MPI_COMM_WORLD, &nprocs) ;
    MPI_Comm_rank (MPI_COMM_WORLD, &id) ;
    MPI_Get_processor_name (hostname, &len) ;
    printf ("I am %2d out of %2d, on %s\n", id, nprocs, hostname) ;
    system ("hostname") ;
    MPI_Finalize ( ) ;
}
