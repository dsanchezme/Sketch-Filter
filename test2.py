# fit a straight line to the economic data
import numpy as np
from pandas import read_csv
from scipy.optimize import curve_fit
from matplotlib import pyplot
 
# define the true objective function
def objective(x, a, b):
	return a * (x ** b)
 
# load the dataset
url = 'https://raw.githubusercontent.com/jbrownlee/Datasets/master/longley.csv'
dataframe = read_csv(url, header=None)
data = dataframe.values
# choose the input and output variables
x_init = [1,2,4,8,16]
y_init = [1.574, 0.9, 0.643, 0.7, 0.688]

# y_init = [
# 5.2370,
# 2.7576,
# 1.4460,
# 0.8851,
# 0.8739
# ]

x = []
y = []

for i in range(len(x_init)-1):
    [x.append(xi) for xi in np.linspace(x_init[i], x_init[i+1], 100)]
    [y.append(np.random.uniform(y_init[i], y_init[i+1])) for j in range(100)]

# curve fit
popt, _ = curve_fit(objective, x, y)
# summarize the parameter values
a, b = popt
print('y = %.5f * x^%.5f' % (a, b))
# plot input vs output
pyplot.scatter(x, y)
# define a sequence of inputs between the smallest and largest known inputs
x_line = np.arange(min(x), max(x), 1)
# calculate the output for the range
y_line = objective(x_line, a, b)
# create a line plot for the mapping function
pyplot.plot(x_line, y_line, '--', color='red')
pyplot.show()