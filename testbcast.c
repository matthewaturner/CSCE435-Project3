# include <math.h>
# include <mpi.h>
# include <stdlib.h>
# include <stdio.h>
# include <time.h>

int ONE_TO_ALL_BC(
    void *buffer, 
    int count, 
    MPI_Datatype datatype, 
    int root, 
    MPI_Comm comm) 
{
  int my_id, size, d, mask;

  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  d = (int) (log(size)/log(2) + .5);
  mask = (int)pow(2, d) - 1;

  for(int i=d-1; i>=0; i--) 
  {
    mask = mask ^ (int)pow(2, i);
    if((my_id & mask) == 0)  
    {
      if((my_id & (int)pow(2, i)) == 0)
      {
        int msg_dest = my_id ^ (int)pow(2, i);
        MPI_Send(buffer, count, datatype, msg_dest, 0, comm);
      } 
      else 
      {
        int msg_source = my_id ^ (int)pow(2, i);
        MPI_Recv(buffer, count, datatype, msg_source, 0, comm, 
        MPI_STATUS_IGNORE);
      }
    }
  }
}

int main ( int argc, char *argv[] )
{
    ierr = MPI_Init ( &argc, &argv );

    double t1, t2;
    int *buffer = malloc(1000000*sizeof(int));

    // give values to the buffer
    for(int i=0; i<1000000; i++) 
    {
        buffer[i] = i;
    }

    t1 = MPI_Wtime(); 
    ONE_TO_ALL_BC(&buffer, 1000000, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    t2 = MPI_Wtime(); 
    printf( "Elapsed time is %f\n", t2 - t1 ); 
}