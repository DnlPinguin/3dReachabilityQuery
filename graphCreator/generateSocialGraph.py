import networkx
import sys

def construct_scale_free_graph(social_nodes_amount, file_name):
    print('Construct social graph with {} nodes'.format(social_nodes_amount))
    scale_free_graph = networkx.scale_free_graph(social_nodes_amount, alpha=0.41, beta=0.54,
                                                 gamma=0.05, delta_in=0.2, delta_out=0.2, create_using=networkx.MultiDiGraph, seed=None)
    social_graph_file = open("./data/raw/" + file_name + "_social", "w")
    social_graph_file.truncate(0)
    for edge in scale_free_graph.edges:
        social_graph_file.write(str(edge[0]) + '\t' + str(edge[1]) + '\n')


def main():
    
    file_name = sys.argv[1]

    social_nodes = int(sys.argv[2])


    print(sys.argv)
    construct_scale_free_graph(social_nodes, file_name)

main()