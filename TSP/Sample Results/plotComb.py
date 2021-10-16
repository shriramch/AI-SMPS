from matplotlib import pyplot as plt
import numpy as np
text_file = open("example7_rand.txt", "r")
lines = text_file.read().split(',')
text_file1 = open("example7_gen.txt", "r")
lines1 = text_file1.read().split(',')
lines = [float(i) for i in lines[:-1]]
lines1 = [float(i) for i in lines1[:-1]]
lines1 = lines1[:3000]
lines = lines[:3000]
xvals = np.arange(len(lines[:-1]))
plt.plot(xvals,lines[:-1])
plt.plot(xvals,lines1[:-1])
plt.xlabel('time in 100s of ms', fontsize=10)
plt.ylabel('cost', fontsize=10)
plt.locator_params(axis='y', nbins=10)
plt.legend(["rand", "gen"], loc ="upper right")
plt.show()