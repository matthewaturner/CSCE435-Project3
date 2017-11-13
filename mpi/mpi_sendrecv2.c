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
    char send_stuff [LEN], recv_stuff [LEN], hostname [LEN], s [LEN] ;

    MPI_Init (&argc, &argv) ;
    MPI_Comm_size (MPI_COMM_WORLD, &nprocs) ;
    MPI_Comm_rank (MPI_COMM_WORLD, &id) ;
    MPI_Get_processor_name (hostname, &len) ;

    if (id < 2)
    {
        /* create a message and sent it to process 1-id */
        MPI_Status status ;
        int other = 1-id ;    /* 0 sends to 1, 1 sends to 0 */
        sprintf (send_stuff, "hi from me:%d", id) ;
        printf ("I am %d on %s sending [%s] to %d\n", id, hostname, send_stuff, other) ;
        status.MPI_ERROR = MPI_SUCCESS ;

        MPI_Send (send_stuff, LEN, MPI_CHAR, other, TAG, MPI_COMM_WORLD) ;
        MPI_Recv (recv_stuff, LEN, MPI_CHAR, other, TAG, MPI_COMM_WORLD, &status) ;

#if 0
        MPI_Sendrecv (
            send_stuff, LEN, MPI_CHAR, other, TAG,  /* send send_stuff to other */
            recv_stuff, LEN, MPI_CHAR, other, TAG,  /* recv recv_stuff from other */
            MPI_COMM_WORLD, &status) ;
#endif

        printf ("I am %d, got [%s] from %d, status: [src %d, tag %d, err %d]\n",
            id, recv_stuff, other,
            status.MPI_SOURCE, status.MPI_TAG, status.MPI_ERROR) ;

        printf ("I am %d and still have my own stuff [%s]\n", id, send_stuff) ;
        MPI_Error_string (status.MPI_ERROR, s, &len) ;
        printf ("%d got error: %s\n", id, s) ;
    }

    MPI_Finalize ( ) ;
}
