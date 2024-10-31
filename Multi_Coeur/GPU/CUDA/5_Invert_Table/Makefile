#CC=gcc

CUDA_HOME=/opt/nvidia/hpc_sdk/Linux_x86_64/23.9/cuda/11.8
CUDASDK=/opt/nvidia/hpc_sdk/Linux_x86_64/23.9/examples/OpenACC/SDK/include/
CUDANVIDIA=/usr/lib/nvidia
NVCC=/usr/bin/nvcc

NVCFLAGS= -I $(CUDA_HOME)/include -I$(CUDASDK)

CFLAGS= -L $(CUDA_HOME)/lib64 -L $(CUDANVIDIA)

inversion_shared: inversion_shared.o
	$(NVCC) $(CFLAGS)  -o $@ $^ 

inversion_no_shared: inversion_no_shared.o
	$(NVCC) $(CFLAGS)  -o $@ $^ 

clean:
	rm -f *.o *.linkinfo inversion

%.o: %.cu
	$(NVCC) $(NVCFLAGS) -c -o $@ $<