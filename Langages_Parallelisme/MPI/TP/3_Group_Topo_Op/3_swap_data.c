#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

struct Mystruct {
	int a;
	double b;
};


int main(int argc, char** argv) {

	int me, size, leftid, rightid;
	int tag = 99;
	
    MPI_Status wait_status;
    MPI_Request recv_request;

    MPI_Init(&argc,&argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &me);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

	if ((leftid=(me - 1)) < 0) leftid = size - 1;
    if ((rightid=(me + 1)) == size) rightid = 0;

	struct Mystruct send_buf = {100 + me, 2.3 + me};
	struct Mystruct recv_buf = {0, 0.0};

	MPI_Aint disp[2], base;

	MPI_Get_address(&send_buf, disp);
	MPI_Get_address(&send_buf.b, disp + 1);

	base = disp[0];
	for (int i = 0; i < 2; i++) 
        disp[i] = MPI_Aint_diff(disp[i], base);

	MPI_Datatype myType;
	MPI_Datatype type[2] = {MPI_INT, MPI_DOUBLE};
	int blocklen[2] = {1, 1};

	MPI_Type_create_struct(2, blocklen, disp, type, &myType);
	MPI_Type_commit(&myType);


	MPI_Irecv(&recv_buf,1, myType, leftid, tag, MPI_COMM_WORLD, &recv_request);
    MPI_Send(&send_buf, 1, myType, rightid, tag, MPI_COMM_WORLD);

	printf("Proc %d sends (%d,%.2f) to P%d\n", me, send_buf.a, send_buf.b, leftid);
    MPI_Wait(&recv_request, &wait_status);
	printf("Proc %d recvs (%d,%.2f) from P%d\n", me, recv_buf.a, recv_buf.b, rightid);

	
  MPI_Finalize();
  return 0;
}
