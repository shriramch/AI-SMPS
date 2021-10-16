from matplotlib import pyplot as plt
import numpy as np

text_file = open("plot.txt", "r")
lines = text_file.read().split(',')
lines = [float(i) for i in lines[:-1]]
xvals = np.arange(len(lines[:-1]))
plt.plot(xvals, lines[:-1])
plt.xlabel('time in 100s of ms', fontsize=10)
plt.ylabel('cost', fontsize=10)
plt.locator_params(axis='y', nbins=10)
plt.show()