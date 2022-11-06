import numpy as np
import matplotlib.pyplot as plt
from numpy.lib.shape_base import apply_over_axes


def plot_histogramm_degree(file_name):
    degree = {}
    file = open("data/reduced_graph/" +
                file_name + "_reduced_scheme", "r")

    
    y = []
    next(file)
    for line in file:
        line = line.split("\t")
        y.append(len(line) - 1)
        # y.append(int(line[7]))

    t = []
    # print(y)
    for i in range(1, 100):
        t.append(i)
    print('Durschnitt:', np.average(y))
    old_bins = [1, 2, 4, 8, 16, 32, 64, 128,
                256, 512, 1024, 2048, 5096, 10192]
    plt.hist(y, bins=t)
    plt.xlabel('#Degree')
    plt.ylabel('Amount')
    plt.title(file_name)
    plt.grid(True)
    plt.show()

def plot_histogramm_cardinality(file_name):
    file = open("data/queries/" +
                file_name + "_queries", "r")

    y = []
    next(file)
    for line in file:
        line = line.split("\t")
        y.append(int(line[7]))

    t = []
    # print(y)

    print('Durschnitt:', np.average(y))
    old_bins = [1, 2, 4, 8, 16, 32, 64, 128,
                256, 512, 1024, 2048, 5096, 10192, 100000]
    plt.hist(y,  density=True)
    plt.xlabel('Cardinality')
    plt.ylabel('Amount')
    plt.title('Cardinality of queries')
    plt.grid(True)
    plt.show()

def plot_hist2d(file_name):
    file = open("data/raw/" +
                file_name + "_spatial", "r")
    x = []
    y = []
    for line in file:
        line = line.split(",")
        if (len(line) == 3):
            if (line[1] != '' and line[2] != ''):
                x.append(float(line[1]))
                y.append(float(line[2]))


    fig, axes = plt.subplots(ncols=1, nrows=2)
    axes[0].set_title((file_name + " " + str(len(x)) + " points"))
    axes[0].plot(x, y, 'ko', color="red", markersize=0.5)
    axes[1].hist2d(x, y, bins=30, cmap='Blues')

    plt.show()


file_name = input("Enter filename: ")


# plot_hist2d(file_name)

plot_histogramm_degree(file_name)
# plot_histogramm_cardinality("foursquare")