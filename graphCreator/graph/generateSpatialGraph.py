from math import floor
import networkx
import matplotlib.pyplot as plt
import numpy as np
import random

from sklearn.datasets import make_blobs

file_name = input("Enter filename: ")

percental_amount = int(input("Enter percentual amount of spatial nodes (0-100): "))

amount_of_clusters = int(input("Enter amount of cluster: "))

G = networkx.DiGraph()

print("read graph ... ")
spatial_points = []
graph_file = open("./data/raw/" + file_name + "_social")
for edge in graph_file:
    edge = edge.replace("\n", "").split(",")
    G.add_edge(int(edge[0]), int(edge[1]))

degree_sum = 0
for node in list(G.nodes):
    degree_sum += G.degree[node]
    if(random.randrange(100) < percental_amount):
        spatial_points.append(node)


print("graph_read")
amount_of_nodes = G.number_of_nodes()
amount_of_edges = G.number_of_edges()

average_degree = degree_sum / amount_of_nodes

print("Nodes ->", amount_of_nodes, "\nEdges ->", amount_of_edges, "\nAverage Degree->", average_degree, "\n Spatial Points", len(spatial_points))



spatial_data = []

spatial_file = open("./data/raw/" + file_name + "_spatial", "w")
spatial_file.truncate(0)
colors = ['blue', 'green', 'red', 'yellow', 'purple']

spatial_points_in_cluster = floor(len(spatial_points) / amount_of_clusters)

centers = [[10,10], [50,20], [90,40]]
distance = 100 #random.randint(50,80)

for i in range(0,amount_of_clusters):
    center = [random.randint(0,1000), random.randint(0,1000)]

    x = np.random.normal(center[0], distance, size=(spatial_points_in_cluster,))
    y = np.random.normal(center[1], distance, size=(spatial_points_in_cluster,)) 

    for index in (range(0,spatial_points_in_cluster)):
        spatial_file.write((str(spatial_points[index + i * spatial_points_in_cluster]) + ',' + str(x[index]) + ',' + str(y[index]) + "\n"))

    plt.scatter(x, y)

plt.show()