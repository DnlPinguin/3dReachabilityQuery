import networkx
import time


"""
Create the Postorder of a given graph file
"""

def construct_postorder_on_nodes(file_name):
    print("Create Postorder..")
    start = int(time.time())

    graph_file = open(
        "../data/processed/{}_reduced_scheme".format(file_name), "r")

    G = networkx.DiGraph()

    for i in graph_file:
        i = i.replace("\n", "").split("\t") 
        for j in range(1, len(i)):
            G.add_edge(i[0], i[j])
    
    print("Graph copied into memory ", int(time.time()) - start , " sec")
    start = int(time.time())
    post_order = list(networkx.dfs_postorder_nodes(G))
    post_order_file = open("../data/processed/{}_postorder".format(file_name), "w")

    for i in range(0,len(post_order)): 
        post_order[i]
        post_order_file.write(str(post_order[i]) + "\n")
    
    print("Postorder created: ", (int(time.time()) - start))

construct_postorder_on_nodes("yelp_wcc")

