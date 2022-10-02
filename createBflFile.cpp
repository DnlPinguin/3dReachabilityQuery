#include "main.h"

int main(int argc, char **argv){
    string filename;
    if (argc == 2){
        filename = argv[1];
    }  else {
        cout << "Enter filename: \n";
        cin >> filename; 
    }

    Timer clock;
	Graph SocialGraph;
	LocationMap LocationGraph;

    readReducedGraph(filename + "_reduced_scheme", &SocialGraph);
    readPostorder(filename + "_postorder", &SocialGraph);

    unordered_map<int,vector<int>>::iterator graphIterator;
    unordered_map<int,int> bflIdToNodeIdentifier;

    string filepath = "data/bfl/" + filename + "_graph";
    string filepathForBflId = "data/bfl/" + filename + "_bfl_id";
    
    ofstream fileGraph;
    ofstream fileBflID;

    fileGraph.open(filepath);
    fileBflID.open(filepathForBflId);

    fileGraph << "graph_for_greach\n";
    fileGraph << SocialGraph.postOrder.size()  << "\n";

    int counter = 0;
    for ( graphIterator = SocialGraph.GraphScheme.begin(); graphIterator != SocialGraph.GraphScheme.end(); graphIterator++){
        int source = graphIterator->first;

        if (SocialGraph.nodeToBflIdentifier.count(source) == 0){
            SocialGraph.nodeToBflIdentifier[source] = counter;
            bflIdToNodeIdentifier[counter] = source;
            fileBflID << source << "\t" << counter << "\n";
            counter++;
        }
        for (int target : SocialGraph.GraphScheme[source]){
            if (SocialGraph.nodeToBflIdentifier.count(target) == 0){
                SocialGraph.nodeToBflIdentifier[target] = counter;
                bflIdToNodeIdentifier[counter] = target;
                fileBflID << target << "\t" << counter << "\n";
                counter++;
            }
        }
    };

    for (int bflSourceNode = 0; bflSourceNode < counter; bflSourceNode++){
        fileGraph << bflSourceNode << ": ";
        for (int target : SocialGraph.GraphScheme[bflIdToNodeIdentifier[bflSourceNode]]){
            fileGraph << SocialGraph.nodeToBflIdentifier[target] << " ";
        }
        fileGraph << "#\n";
    }
}