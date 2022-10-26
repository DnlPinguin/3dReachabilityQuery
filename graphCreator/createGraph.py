from ast import main
import networkx
import time
import sys

"""
Create postorder for a graph
"""
def construct_postorder_on_nodes(file_name):
    print("Create Postorder..")
    start = int(time.time())
    graph_file = open(file_name + "_reduced_scheme", "r")
    G = networkx.DiGraph()
    for i in graph_file:
        i = i.replace("\n", "").split("\t") 
        for j in range(1, len(i)):
            G.add_edge(i[0], i[j])
    print("Graph copied into memory ", int(time.time()) - start , " sec")

    start = int(time.time())
    post_order = list(networkx.dfs_postorder_nodes(G))
    print("Postorder created: ", (int(time.time()) - start))

    post_order_file = open("../data/processed/{}_postorder".format(file_name), "w")
    for i in range(0,len(post_order)): 
        post_order[i]
        post_order_file.write(str(post_order[i]) + "\n")
    

file = input("specify graph name")
construct_postorder_on_nodes(file)


