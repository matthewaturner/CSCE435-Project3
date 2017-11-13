#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define LEN 256
#define TAG 42

/* assumes there are at least 2 processes, 0 and 1.
 * Will deadlock if nprocs == 1 */

main (int argc, char **argv)
{
    int nprocs, id, len ;
    char stuff [LEN+1], hostname [LEN+1], s [LEN] ;

    MPI_Init (&argc, &argv) ;
    MPI_Comm_size (MPI_COMM_WORLD, &nprocs) ;
    MPI_Comm_rank (MPI_COMM_WORLD, &id) ;
    MPI_Get_processor_name (hostname, &len) ;

    if (id == 0)
    {
        /* create a message and sent it to process 1 */
        int dest = 1 ;
        sprintf (stuff, "hi from me:%d", id) ;
        printf ("I am %d on %s sending [%s] to %d\n", id, hostname, stuff, dest) ;
        MPI_Send (stuff, LEN+1, MPI_CHAR, dest, TAG, MPI_COMM_WORLD) ;
    }
    else if (id == 1)
    {
        int src = 0 ;
        MPI_Status status ;
        status.MPI_SOURCE = -8888 ;         /* fill with junk */
        status.MPI_TAG = 777333 ;           /* fill with junk */
        status.MPI_ERROR = MPI_SUCCESS ;    /* must be initialized */

        printf ("I am %d on %s waiting to get stuff from %d\n", id, hostname, src) ;
        MPI_Recv (stuff, LEN+1, MPI_CHAR, src, TAG, MPI_COMM_WORLD, &status) ;
        printf ("I am %d, got stuff [%s] from %d, status: [src %d, tag %d, err %d]\n",
            id, stuff, src,
            status.MPI_SOURCE, status.MPI_TAG, status.MPI_ERROR) ;
        MPI_Error_string (status.MPI_ERROR, s, &len) ;
        printf ("%d got error: %s\n", id, s) ;
    }

    MPI_Finalize ( ) ;
}
