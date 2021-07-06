#!/usr/bin/env python3

import os
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from scipy.special import expit

# define the true objective function
def objective(x, a, b, c):
    return a + b*expit(-c*x)

path = 'times/'
for filename in os.listdir(path):
    try:
        f = open(path + filename, "r")
        lines = [x for x in [line.strip() for line in f.readlines()] if len(x) > 0 and '-' not in x]
        threads = []
        times = []
        means = []
        # print("File: ", filename)
        for line in lines:
            if(line.split()[0] != 'TIMES'):
                pass
            else:
                if(len(threads) > 0):
                    means.append(np.mean(times))
                    # print(times)
                    times = []
                threads.append(int(line.split()[-2]))
                    
            try:
                time = float(line) 
                times.append(time)
            except:
                pass

            if(lines.index(line) == len(lines)-1):
                means.append(np.mean(times))

        xpoints = np.array(threads, dtype=np.float128)
        ypoints = np.array(means, dtype=np.float128)

        # curve fit
        popt, _ = curve_fit(objective, xpoints, ypoints)
        # summarize the parameter values
        a, b, c = popt
        # define a sequence of inputs between the smallest and largest known inputs
        x_line = np.arange(min(xpoints), max(xpoints), 1)
        # calculate the output for the range
        y_line = objective(x_line, a, b, c)

        img_name = os.path.splitext(os.path.basename(filename))[0]
        plot_name = 'plots/' + img_name + '_plot.jpg'

        plt.figure()
        # plot input vs output
        plt.scatter(xpoints, ypoints)
        # create a line plot for the mapping function
        plt.plot(x_line, y_line, '-', color='#ff5500')
        plt.xlabel('# Threads')
        plt.ylabel('Response Time (s)')
        plt.grid(linewidth=0.4)
        # displaying the title
        plt.title('Response time for ' + img_name[6:], fontweight='bold')
        plt.savefig(plot_name)
        # plt.show()
        if(1 in threads):
            sequential = means[threads.index(1)]
            speedups = []
            for i in range(len(means)):
                speedups.append(sequential / means[i])

            xpoints = np.array(threads, dtype=np.float128)
            ypoints = np.array(speedups, dtype=np.float128)
            
            
            # curve fit
            popt, _ = curve_fit(objective, xpoints, ypoints)
            # summarize the parameter values
            a, b, c = popt
            # define a sequence of inputs between the smallest and largest known inputs
            x_line = np.arange(min(xpoints), max(xpoints), 1)
            # calculate the output for the range
            y_line = objective(x_line, a, b, c)


            img_name = os.path.splitext(os.path.basename(filename))[0][6:]
            plot_name = 'plots/speedup_' + img_name + '_plot.jpg'
            
            plt.figure()
            # plot input vs output
            plt.scatter(xpoints, ypoints)
            # create a line plot for the mapping function
            plt.plot(x_line, y_line, '-', color='#ff5500')
            plt.xlabel('# Threads')
            plt.ylabel('Speedup')
            plt.grid(linewidth=0.4)
            # displaying the title
            plt.title('Speedup for ' + img_name, fontweight='bold')
            plt.savefig(plot_name)
            # plt.show()

        else:
            print("It is not possible to plot the speedup graph.")
            print("Try running the sketch filter for 1 thread as well")
        # break
    except:
        print(f"Unable to graph {filename} file")