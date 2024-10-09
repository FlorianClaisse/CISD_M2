#!/bin/bash

suffix="_strong"
for ((exp=0; exp<7; exp++))
do
  # Compute the number of cores as a power of 2
  nproc=$((2**exp))
  # On miriel there are 24 cores per node, so compute the number of nodes
  # accordingly
  nnodes=$((nproc/24 + 1))
  # Compute the number of DOFs for strong or weak scaling
  #nxtot=$((100000*nproc))
  nxtot=1000000
  echo "Launching job on " $nproc "cores (" $nnodes " nodes)," $nxtot "DOFs"
  sbatch --job-name=ring${nproc}${suffix} --output=out_ring${nproc}${suffix} --error=err_ring${nproc}${suffix} \
   --nodes=$nnodes --ntasks=$nproc job_ring.slm $nxtot
done
