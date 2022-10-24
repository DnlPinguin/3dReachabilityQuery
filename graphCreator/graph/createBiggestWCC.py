import networkx

print("Specify file name:")

file_name = input()

G = networkx.DiGraph()

graph_file = open(
    "./data/raw/{}_social".format(file_name), "r")

counter = 0
for edge in graph_file:
    edge = edge.replace("\n", "").split(",")
    G.add_edge(int(edge[0]), int(edge[1]))
    counter = counter + 1
    if (counter % 3000000 == 0):
        print(counter, "edges read")

print("Graph read")
wcc = networkx.weakly_connected_components(G)
print("WCCs created")
biggest_wcc = []

for connected_component in wcc:
    connected_component = list(connected_component)
    if (len(connected_component) > len(biggest_wcc)):
        biggest_wcc = connected_component

print("biggest wcc found")
biggest_wcc = set(biggest_wcc)
graph_file_wcc = open(
    "./data/raw/{}_wcc_social".format(file_name), "w")

for edge in G.edges:
    if (edge[0] in biggest_wcc and edge[1] in biggest_wcc):
        graph_file_wcc.write(
            str(edge[0]) + "," + str(edge[1]) + "\n"
        ) 

print("wcc graph file written")

spatial_file_wcc = open(
    "./data/raw/{}_wcc_spatial".format(file_name), "w")

spatial_file = open(
    "./data/raw/{}_spatial".format(file_name), "r")


counter = 0
for point in spatial_file:
    point = point.replace("\n", "").split(",")
    if (int(point[0]) in biggest_wcc):
        spatial_file_wcc.write(str(point[0]) + "," + str(point[1]) + "," + str(point[2]) + "\n")
    counter = counter + 1
    if (counter % 500000 == 0):
        print(counter, " spatial points transformed")
print("finished")