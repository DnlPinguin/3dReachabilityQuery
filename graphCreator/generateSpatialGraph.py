from tkinter import Y
import networkx
import random
import sys
import matplotlib.pyplot as plt
import numpy as np
from sklearn.datasets import make_blobs
from math import floor
import shutil

def construct_random_coordinate(lower_cor, upper_cor):
    return str("{:.5f}".format((random.uniform(float(lower_cor), float(upper_cor)))))


def construct_file_entry(node, x, y):  # Construct entry for file
    return (str(node) + '\t' + str(x) + '\t' + str(y) + '\n')

def count_social_nodes(file_name):
    social_file = open("./data/raw/" + file_name + "_social", "r")
    nodes = []
    for line in social_file:
        line = line.replace("/n", "").split("\t")
        nodes.append(int(line[1]))
        nodes.append(int(line[0]))

    return len(set(nodes))

def construct_geo_graph(geo_nodes_amount, bottom_left_coords, top_right_coords, file_name):
    print('Construct spatial graph with {} nodes'.format(geo_nodes_amount))

    social_file = open("./data/raw/" + file_name + "_social", "r")
    nodes = []
    for line in social_file:
        line = line.replace("/n", "").split("\t")
        nodes.append(int(line[1]))
        nodes.append(int(line[0]))

    
    geo_file = open("./data/raw/" + file_name + "_spatial", "w")
    unique_nodes = set(nodes)

    for index in range(geo_nodes_amount):
        spatial_node = random.sample(unique_nodes, 1)[0]
        unique_nodes.remove(spatial_node)
        x = construct_random_coordinate(
                bottom_left_coords[0], top_right_coords[0])
        y = construct_random_coordinate(
                bottom_left_coords[1], top_right_coords[1])
        geo_file.write(construct_file_entry(spatial_node, x, y))


def construct_geo_graph_on_top( geo_nodes_amount, file_name):
    social_nodes_amount = count_social_nodes(file_name)

    starting_index = social_nodes_amount + 1
    spatial_file = open("./data/raw/" + file_name + "_layer_spatial", "w")
    spatial_file.truncate(0)
    

    shutil.copyfile("./data/raw/" + file_name + "_social", "./data/raw/" + file_name + "_layer_social")
    for geo_nodes_index in range(starting_index, geo_nodes_amount + social_nodes_amount):
        [x,y] = [construct_random_coordinate(0, 1),construct_random_coordinate(0, 1)]
        spatial_file.write(str(geo_nodes_index) + '\t' + str(x) + '\t' + str(y) + "\n")

    social_file = open('./data/raw/' + file_name + '_layer_social', 'a')
    for geo_nodes_index in range(starting_index, geo_nodes_amount + social_nodes_amount):
        curr_social_node = random.randint(0, social_nodes_amount-1)
        social_file.write(str(curr_social_node) + "\t" +  str(geo_nodes_index) + "\n")


def construct_integrated_clustered_geo_graph(geo_nodes_amount, file_name, amount_of_clusters=6):
    G = networkx.DiGraph()
    spatial_points = []
    spatial_points_in_cluster = floor(geo_nodes_amount / amount_of_clusters)

    graph_file = open("./data/raw/" + file_name + "_social")
    nodes = []
    for edge in graph_file:
        edge = edge.replace("\n", "").split("\t")
        G.add_edge(int(edge[0]), int(edge[1]))
        nodes.append(int(edge[0]))
        nodes.append(int(edge[1]))


    unique_nodes = set(nodes)
    for index in range(geo_nodes_amount):
        spatial_node = random.sample(unique_nodes, 1)[0]
        unique_nodes.remove(spatial_node)
        spatial_points.append(spatial_node)


    spatial_file = open("./data/raw/" + file_name + "_spatial", "w")
    spatial_file.truncate(0)
    spatial_data = []
    distance = 100 
    max_x = float('-inf')
    max_y = float('-inf')
    min_x = float('inf')
    min_y = float('inf')

    for cluster_index in range(0,amount_of_clusters):
        center = [random.randint(0,1000), random.randint(0,1000)]
        x = np.random.normal(center[0], distance, size=(spatial_points_in_cluster,))
        y = np.random.normal(center[1], distance, size=(spatial_points_in_cluster,)) 
        if x > max_x: 
            max_x = x
        if y > max_y: 
            max_y = y
        if x < min_x: 
            min_x = x
        if y < min_y: 
            min_y = y

        for index in (range(0,spatial_points_in_cluster)):
            index_in_spatial_points = index + cluster_index * spatial_points_in_cluster
            spatial_data.append([str(spatial_points[index_in_spatial_points]), x[index], y[index]])

    for entry in spatial_data:
        x = ((entry[1] - min_x) / (max_x - min_x))
        y = ((entry[2] - min_y) / (max_y - min_y))
        spatial_file.write(str(entry[0]) + '\t' + str(x[0]) + '\t' + str(y[0]) + "\n")


def construct_clustered_geo_graph_on_top( geo_nodes_amount, file_name, amount_of_clusters=6):
    social_nodes_amount = count_social_nodes(file_name) 
    starting_index = social_nodes_amount + 1
    

    shutil.copyfile("./data/raw/" + file_name + "_social", "./data/raw/" + file_name + "_cluster_social")
    graph_file = open('./data/raw/' + file_name + '_cluster_social', 'a')

    spatial_file = open("./data/raw/" + file_name + "_cluster_spatial", "w")
    spatial_file.truncate(0)

    spatial_points_in_cluster = floor(geo_nodes_amount / amount_of_clusters)
    distance = 100 
    max_x = float('-inf')
    max_y = float('-inf')
    min_x = float('inf')
    min_y = float('inf')

    spatial_data = []
    for cluster_index in range(0,amount_of_clusters):
        center = [random.randint(0,1000), random.randint(0,1000)]
        x = np.random.normal(center[0], distance, size=(spatial_points_in_cluster,))
        y = np.random.normal(center[1], distance, size=(spatial_points_in_cluster,)) 
        if x > max_x: 
            max_x = x
        if y > max_y: 
            max_y = y
        if x < min_x: 
            min_x = x
        if y < min_y: 
            min_y = y

        for index in (range(0,spatial_points_in_cluster)):
            spatial_data.append([x[index], y[index]])


    counter = 1
    for entry in spatial_data:
        x = ((entry[0] - min_x) / (max_x - min_x))
        y = ((entry[1] - min_y) / (max_y - min_y))
        geo_node_index = social_nodes_amount + counter
        spatial_file.write(str(geo_node_index) + '\t' + str(x[0]) + '\t' + str(y[0]) + "\n")
        
        curr_social_node = (random.randint(0, social_nodes_amount-1))
        graph_file.write(str(curr_social_node) + '\t' + str(geo_node_index) + '\n')
        counter += 1



def main():
    wrong_input_message = "Input is wrong! Try python graphCreator/generateGraph.py FILE_NAME METHOD AMOUNT_OF_SPATIAL_NODES"
    wrong_method_message = "method is wrong! Try integrated_uniform, integrated_clustered, layer_uniform, layer_clustered"
    
    file_name = sys.argv[1]

    possible_method_of_spatial_allocation = ['integrated_uniform', 'integrated_clustered', 'layer_uniform', 'layer_clustered']
    method_of_spatial_allocation = sys.argv[2]
    if (method_of_spatial_allocation not in possible_method_of_spatial_allocation):
        print(wrong_method_message)
        return

    spatial_nodes = int(sys.argv[3])


    
    if method_of_spatial_allocation == 'integrated_uniform':
        construct_geo_graph(spatial_nodes, [0,0], [1,1], file_name)

    elif method_of_spatial_allocation == 'integrated_clustered':
        construct_integrated_clustered_geo_graph( spatial_nodes, file_name)

    elif method_of_spatial_allocation == 'layer_uniform':
        construct_geo_graph_on_top( spatial_nodes, file_name)

    elif method_of_spatial_allocation == 'layer_clustered':
        construct_clustered_geo_graph_on_top(spatial_nodes, file_name)

    
    




main()