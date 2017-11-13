
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void collatz (int *sequence, int n, int s)
{
    int i ;
    for (i = 0 ; i < n ; i++)
    {
        /* run the Collatz conjector, starting with s = rank */
        sequence [i] = s ;
        s = (s % 2 == 0) ? (s/2) : (3*s + 1) ;
    }
}

int main(int argc, char *argv[])
{
    int p, i, rank, nprocs, s, n = 8, *in, *out, *sol ;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    in  = (int *) calloc (n, sizeof (int));
    out = (int *) calloc (n, sizeof (int));

    /* each process runs the Collatz sequence, starting with s = rank */
    collatz (in, n, rank) ;

    /* sum up the results ... this has nothing to do with the
     * Collatz conjecture but we're just trying out MPI.
     * Each process gets the summation of all the Collatz sequences. */
    MPI_Allreduce( in, out, n, MPI_INT, MPI_SUM, MPI_COMM_WORLD );

    /* check the result on each process.  this is costly */ 
    sol = (int *) calloc (n, sizeof (int));
    for (p = 0 ; p < nprocs ; p++)
    {
        collatz (in, n, p) ;
        for (i = 0 ; i < n ; i++)
        {
            sol [i] += in [i] ;
        }
        if (rank == 0 && n < 10)
        {
            printf ("sequence %3d: ", p) ;
            for (i = 0 ; i < n ; i++) printf (" %3d", in [i]) ;
            printf ("\n") ;
        }
    }
    for (i = 0 ; i < n ; i++)
    {
        if (rank == 0 && n < 10)
        {
            printf ("\nsummation   : ") ;
            for (i = 0 ; i < n ; i++) printf (" %3d", sol [i]) ;
            printf ("\n") ;
        }
        if (out [i] != sol [i])
        {
            printf("%d found an error!\n", rank) ;
        }
    }

    free( in );
    free( out );
    free( sol );
    MPI_Finalize();
}
