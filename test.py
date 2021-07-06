
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import savgol_filter

x = [1,2,4,8,16]
# x = np.linspace(0,2*np.pi,100)
# y = np.sin(x) + np.random.random(100) * 0.2
y = [1.574, 0.9, 0.643, 0.7, 0.688]

x_temp = []
y_temp = []

for i in range(len(x)-1):
    [x_temp.append(xi) for xi in np.linspace(x[i], x[i+1], 100)]
    [y_temp.append(yi) for yi in np.linspace(y[i], y[i+1], 100)]


yhat = savgol_filter(y_temp, len(x_temp)-1, 4) # window size 51, polynomial order 3
print(x_temp)
plt.plot(x_temp,y_temp)
plt.plot(x_temp,yhat, color='red')
plt.show()