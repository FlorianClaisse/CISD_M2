import matplotlib
import matplotlib.pyplot as plt
import numpy as np

# Set GUI backend
matplotlib.use('TkAgg')

# Read file
nproc = []
time  = []

f = open('out_ring_strong.stats')

for row in f:
  row = row.split(' ')
  nproc.append(int(row[0]))
  time.append(float(row[1]))

# Compute speedup
s = np.zeros(len(nproc))
for i in range(s.size):
  s[i] = time[0]/time[i]

# Plot
plt.figure(0)
plt.plot(nproc,time,'bo-')
plt.xlabel(' Nb. of cores')
plt.ylabel('Elapsed time, strong scaling test [s]')
plt.show()

plt.figure(1)
plt.loglog(nproc,s,'bo-',nproc,nproc,'r--')
plt.xlabel(' Nb. of cores')
plt.ylabel('Elapsed time, strong scaling test [s]')
plt.legend(['Actual speedup','Ideal speedup'])
plt.show()
