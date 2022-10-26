import networkx
import random
import sys
import matplotlib.pyplot as plt
import numpy as np
from sklearn.datasets import make_blobs
from math import floor


def construct_random_coordinate(lower_cor, upper_cor):
    return str("{:.5f}".format((random.uniform(float(lower_cor), float(upper_cor)))))


def construct_file_entry(social_node, x, y):  # Construct entry for file
    return (str(social_node) + '\t' + str(x) + '\t' + str(y) + '\n')


def construct_geo_graph(social_nodes_amount, geo_nodes_amount, bottom_left_coords, top_right_coords, file_name):
    print('Construct spatial graph with {} nodes'.format(geo_nodes_amount))

    geo_file = open("./data/raw/" + file_name + "_spatial", "w")
    percental_amount = (geo_nodes_amount/social_nodes_amount) * 100

    for node in range(0, social_nodes_amount):
        if random.randrange(100) < percental_amount:
            x = construct_random_coordinate(
                bottom_left_coords[0], top_right_coords[0])
            y = construct_random_coordinate(
                bottom_left_coords[1], top_right_coords[1])
            geo_file.write(construct_file_entry(node, x, y))

def construct_geo_graph_on_top(social_nodes_amount, geo_nodes_amount, file_name):
    starting_index = social_nodes_amount + 1
    spatial_file = open("./data/raw/" + file_name + "_spatial", "w")
    spatial_file.truncate(0)

    for geo_nodes_index in range(starting_index, geo_nodes_amount + social_nodes_amount):
        [x,y] = [construct_random_coordinate(0, 1),construct_random_coordinate(0, 1)]
        spatial_file.write(str(geo_nodes_index) + '\t' + str(x) + '\t' + str(y) + "\n")

    social_file = open('./data/raw/' + file_name + '_social', 'a')
    for geo_nodes_index in range(starting_index, geo_nodes_amount + social_nodes_amount):
        curr_social_node = random.randint(0, social_nodes_amount-1)
        print(curr_social_node)
        social_file.write(str(curr_social_node) + "\t" +  str(geo_nodes_index) + "\n")


def construct_scale_free_graph(social_nodes_amount, file_name):
    print('Construct social graph with {} nodes'.format(social_nodes_amount))
    scale_free_graph = networkx.scale_free_graph(social_nodes_amount, alpha=0.41, beta=0.54,
                                                 gamma=0.05, delta_in=0.2, delta_out=0.2, create_using=networkx.MultiDiGraph, seed=None)
    social_graph_file = open("./data/raw/" + file_name + "_social", "w")
    social_graph_file.truncate(0)
    for edge in scale_free_graph.edges:
        social_graph_file.write(str(edge[0]) + '\t' + str(edge[1]) + '\n')


def construct_integrated_clustered_geo_graph(social_nodes_amount, geo_nodes_amount, file_name, amount_of_clusters=6):
    G = networkx.DiGraph()
    spatial_points = []
    graph_file = open("./data/raw/" + file_name + "_social")
    for edge in graph_file:
        edge = edge.replace("\n", "").split("\t")
        G.add_edge(int(edge[0]), int(edge[1]))

    percental_amount = (geo_nodes_amount/social_nodes_amount) * 100
    for node in list(G.nodes):
        if(random.randrange(100) < percental_amount):
            spatial_points.append(node)

    spatial_file = open("./data/raw/" + file_name + "_spatial", "w")
    spatial_file.truncate(0)
    spatial_points_in_cluster = floor(len(spatial_points) / amount_of_clusters)
    distance = 100 
    for cluster_index in range(0,amount_of_clusters):
        center = [random.randint(0,1000), random.randint(0,1000)]
        x = np.random.normal(center[0], distance, size=(spatial_points_in_cluster,))
        y = np.random.normal(center[1], distance, size=(spatial_points_in_cluster,)) 
        for index in (range(0,spatial_points_in_cluster)):
            spatial_file.write((str(spatial_points[index + cluster_index * spatial_points_in_cluster]) + '\t' + str(x[index]) + '\t' + str(y[index]) + "\n"))

def construct_clustered_geo_graph_on_top(social_nodes_amount, geo_nodes_amount, file_name, amount_of_clusters=6):
    starting_index = social_nodes_amount + 1
    
    graph_file = open("./data/raw/" + file_name + "_social", 'a')

    spatial_file = open("./data/raw/" + file_name + "_spatial", "w")
    spatial_file.truncate(0)
    spatial_points_in_cluster = floor(geo_nodes_amount / amount_of_clusters)
    print("spatial_points in cluster", spatial_points_in_cluster)
    distance = 100 
    for cluster_index in range(0,amount_of_clusters):
        center = [random.randint(0,1000), random.randint(0,1000)]
        x = np.random.normal(center[0], distance, size=(spatial_points_in_cluster,))
        y = np.random.normal(center[1], distance, size=(spatial_points_in_cluster,)) 
        for index in (range(0,spatial_points_in_cluster)):
            geo_node_index = starting_index + (cluster_index * amount_of_clusters) + index
            spatial_file.write(str(geo_node_index) + '\t' + str(x[index]) + '\t' + str(y[index]) + "\n")

        
    for geo_nodes_index in range(starting_index, geo_nodes_amount + social_nodes_amount):
        curr_social_node = (random.randint(0, social_nodes_amount-1))
        graph_file.write(str(curr_social_node) + '\t' + str(geo_nodes_index) + '\n')



def main():
    wrong_input_message = "Input is wrong! Try python graphCreator/generateGraph.py FILE_NAME METHOD AMOUNT_OF_SOCIAL_NODES AMOUNT_OF_SPATIAL_NODES"
    wrong_method_message = "method is wrong! Try integrated_uniform, clustered_uniform, uniform_layer, clustered_layer"
    
    file_name = sys.argv[1]

    possible_method_of_spatial_allocation = ['integrated_uniform', 'integrated_clustered', 'layer_uniform', 'layer_clustered']
    method_of_spatial_allocation = sys.argv[2]
    if (method_of_spatial_allocation not in possible_method_of_spatial_allocation):
        print(wrong_method_message)
        return

    social_nodes = int(sys.argv[3])
    spatial_nodes = int(sys.argv[4])


    print(sys.argv)
    construct_scale_free_graph(social_nodes, file_name)
    
    if method_of_spatial_allocation == 'integrated_uniform':
        construct_geo_graph(social_nodes, spatial_nodes, [0,0], [1,1], file_name)

    elif method_of_spatial_allocation == 'integrated_clustered':
        construct_integrated_clustered_geo_graph(social_nodes, spatial_nodes, file_name)

    elif method_of_spatial_allocation == 'layer_uniform':
        construct_geo_graph_on_top(social_nodes, spatial_nodes, file_name)

    elif method_of_spatial_allocation == 'layer_clustered':
        construct_clustered_geo_graph_on_top(social_nodes, spatial_nodes, file_name)

    
    




main()