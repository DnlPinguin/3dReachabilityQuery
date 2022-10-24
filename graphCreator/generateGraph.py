import networkx
import random
import sys

def construct_random_coordinate(lower_cor, upper_cor):
    return str("{:.5f}".format((random.uniform(float(lower_cor), float(upper_cor)))))


def construct_file_entry(social_node, x, y):  # Construct entry for file
    return (str(social_node) + ',' + str(x) + ',' + str(y) + '\n')


def construct_geo_graph(social_nodes_amount, geo_nodes_amount, bottom_left_coords, top_right_coords, file_name):
    print('Construct spatial graph with {} nodes'.format(geo_nodes_amount))

    geo_file = open("./data/raw/" + file_name + "_spatial", "w")
    for node in range(0, social_nodes_amount):
        if ((random.random()*100) < geo_nodes_amount):
            x = construct_random_coordinate(
                bottom_left_coords[0], top_right_coords[0])
            y = construct_random_coordinate(
                bottom_left_coords[1], top_right_coords[1])
            geo_file.write(construct_file_entry(node, x, y))


def construct_scale_free_graph(social_nodes_amount, file_name):
    print('Construct social graph with {} nodes'.format(social_nodes_amount))
    scale_free_graph = networkx.scale_free_graph(social_nodes_amount, alpha=0.41, beta=0.54,
                                                 gamma=0.05, delta_in=0.2, delta_out=0.2, create_using=networkx.MultiDiGraph, seed=None)
    social_graph_file = open("./data/raw/" + file_name + "_social", "w")
    social_graph_file.truncate(0)
    for edge in scale_free_graph.edges:
        social_graph_file.write(str(edge[0]) + ',' + str(edge[1]) + '\n')


def main():
    wrong_input_message = "Input is wrong! Try python graphCreator/generateGraph.py FILE_NAME METHOD AMOUNT_OF_SOCIAL_NODES AMOUNT_OF_SPATIAL_NODES"
    wrong_method_message = "method is wrong! Try integrated_uniform, clustered_uniform, uniform_layer, clustered_layer"
    
    file_name = sys.argv[1]

    possible_method_of_spatial_allocation = ['integrated_uniform', 'clustered_uniform', 'uniform_layer', 'clustered_layer']
    method_of_spatial_allocation = sys.argv[2]
    if (method_of_spatial_allocation not in possible_method_of_spatial_allocation):
        print(wrong_method_message)
        return

    social_nodes = int(sys.argv[3])
    spatial_nodes = int(sys.argv[4])


    print(sys.argv)
    if (method_of_spatial_allocation == 'integr')
    construct_scale_free_graph(social_nodes, file_name)

    # construct_geo_graph(social_nodes, spatial_percentage, [0,0], [1,1], file_name)



main()