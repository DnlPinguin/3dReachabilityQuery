import graph.scc
import graph.postorder

def main(file_name):
    graph.scc.construct_super_nodes_on_graph(file_name)
    graph.postorder.construct_postorder_on_nodes(file_name)


file_name = input("Enter filename: ")
main(file_name)