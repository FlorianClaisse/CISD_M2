#!/bin/bash
# Author: Luca Cirrottola (INRIA)
#
# November 2022

# Set jobname prefix
jobprefix=$1

# Set job suffix
jobsuffix=$2

# Set statistics file
statfile="${jobprefix}${jobsuffix}.stats"

# Create backup file if needed
if [ -f $statfile ]
then
  mv $statfile ${jobprefix}.back.stats
else
  touch $statfile
fi

# Loop on number of cores
for ((exp=0; exp<7; exp++))
do
  # Compute the number of cores as a power of 2
  nproc=$((2**exp))

  # Get job name
  jobname="${jobprefix}${nproc}${jobsuffix}"

  # Write number of cores to the statistics file
  echo -n "${nproc} " >> $statfile

  # Find elapsed time in the output file and write it in the statisics file
  echo -n `sed -n "/Elapsed\ time:/p" ${jobname} | awk '{ print $3 }'` >> $statfile

  # New line
  echo "" >> $statfile

done

