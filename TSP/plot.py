from matplotlib import pyplot as plt
import numpy as np
text_file = open("plot.txt", "r")
lines = text_file.read().split(',')
xvals = np.arange(len(lines[:-1]))
plt.plot(xvals,lines[:-1])
plt.gca().invert_yaxis()
plt.show()