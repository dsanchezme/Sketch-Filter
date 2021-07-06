#!/usr/bin/env python3

import os
import math
import numpy as np
import matplotlib.pyplot as plt

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

        xpoints = np.array(threads)
        ypoints = np.array(means)
        
        plt.figure()
        plt.xlabel('#Threads')
        plt.ylabel('Response Time (s)')

        img_name = os.path.splitext(os.path.basename(filename))[0]

        plt.plot(xpoints, ypoints, 'o', xpoints, ypoints, '-')
        # displaying the title
        plt.title('Response time for ' + filename)
        plt.grid()
        plt.axis()
        plot_name = 'plots/' + img_name + '_plot.jpg'
        # plt.savefig(plot_name)
        plt.show()
        break
        # if(1 in threads):
        #     sequential = means[threads.index(1)]
        #     speedups = []
        #     for i in range(len(means)):
        #         speedups.append(sequential / means[i])

        #     xpoints = np.array(threads)
        #     ypoints = np.array(speedups)
            
        #     plt.figure()
        #     plt.xlabel('#Threads')
        #     plt.ylabel('Speedup')

        #     img_name = os.path.splitext(os.path.basename(filename))[0][6:]
        #     # print(img_name)
        #     plt.plot(xpoints, ypoints, 'o', xpoints, ypoints, '-')
        #     # displaying the title
        #     plt.title('Speedup for ' + filename)
        #     plt.grid()
        #     plt.axis()
        #     plot_name = 'plots/' + img_name + '_plot.jpg'
        #     plt.savefig(plot_name)
        #     # plt.show()
        # else:
        #     print("It is not possible to plot the speedup graph.")
        #     print("Try running the sketch filter for 1 thread as well")
    except:
        print(f"Unable to graph {filename} file")