# Intel MKL: sequential BLAS with 32-bit integer parameters, dynamic link
# see https://software.intel.com/en-us/articles/intel-mkl-link-line-advisor
#
#	module purge
#	module load intel/2017A
#       make
#       
# To run this with bsub, do the following, which runs 'make' and then
# runs mpi_hello on 40 cores and then again on 8.
#
#	bsub < hello.lsf

CC = icc
MPICC = mpiicc
CFLAGS += -O3 -I${MKLROOT}/include
LIB = -L${MKLROOT}/lib/intel64 -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl -lrt

SRC = mpi_hello.c

hi:
	$(MPICC) $(CFLAGS) mpi_hello.c -o mpi_hello $(LIB)
	# run on default # of processors:
	mpirun ./mpi_hello
	# run on 8 processors:
	mpirun -np 8 ./mpi_hello

sr:
	$(MPICC) $(CFLAGS) mpi_sendrecv.c -o mpi_sendrecv $(LIB)
	# to run on default # of processors:
	mpirun -np 8 ./mpi_sendrecv


sr2:
	$(MPICC) $(CFLAGS) mpi_sendrecv2.c -o mpi_sendrecv2 $(LIB)
	# to run on default # of processors:
	mpirun -np 8 ./mpi_sendrecv2


ring:
	$(MPICC) $(CFLAGS) ring_mpi.c -o ring_mpi $(LIB)
	# to run on default # of processors:
	mpirun -np 8 ./ring_mpi


comm:
	$(MPICC) $(CFLAGS) communicator_mpi.c -o communicator_mpi $(LIB)
	# to run on default # of processors:
	mpirun -np 8 ./communicator_mpi

isend:
	$(MPICC) $(CFLAGS) mpiRingISendIRecv.c -o mpiRingISendIRecv $(LIB)
	# to run on default # of processors:
	mpirun -np 8 ./mpiRingISendIRecv 40

bsend:
	$(MPICC) $(CFLAGS) mpiRingSendRecv.c -o mpiRingSendRecv $(LIB)
	# to run on default # of processors:
	mpirun -np 8 ./mpiRingSendRecv 40


intervals:
	$(MPICC) $(CFLAGS) intervals_mpi.c -o intervals_mpi $(LIB)
	# to run on default # of processors:
	mpirun -np 8 ./intervals_mpi

intervals2:
	$(MPICC) $(CFLAGS) intervals_mpi2.c -o intervals_mpi2 $(LIB)
	# to run on default # of processors:
	mpirun -np 8 ./intervals_mpi2

allred:
	$(MPICC) $(CFLAGS) allreduce.c -o allreduce $(LIB)
	# to run on default # of processors:
	mpirun -np 8 ./allreduce

clean:
	$(RM) -f communicator_mpi intervals_mpi mpi_hello mpiRingISendIRecv \
	    mpiRingSendRecv mpi_sendrecv mpi_sendrecv2 ring_mpi \
	    allreduce intervals_mpi2
