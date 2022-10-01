#include "main.h"

/**
 * @brief Create a Interval Scheme object by traversing the graph in a topogical sorting. 
 * This is realized by traversing the postorder and updating the child/parent Nodes with the
 * postorder of the current inspected node
 * 
 * After the traversing is complete. The uncompressed scheme will be traversed to compress the 
 * existing interal data.
 * 
 * @param filename 
 * @param reversed 
 * @return double 
 */
double createIntervalScheme(string filename, bool reversed){
    
    Graph SocialGraph;
	readPostorder(filename + "_postorder", &SocialGraph);
    readReducedGraph(filename + "_reduced_scheme", &SocialGraph);

    auto start_time = std::chrono::high_resolution_clock::now();

    ofstream file;
    if (reversed){
        file.open(filename + "_interval_scheme_reversed");
    } else {
        file.open(filename + "_interval_scheme");
    }

    vector<int> postOrderVector = SocialGraph.postOrder;
    if (!reversed){
        std::reverse(postOrderVector.begin(), postOrderVector.end());
    }

    map<int, set<int>> intervalSchemeMap;
    for (int source : postOrderVector){
        intervalSchemeMap[source].insert(SocialGraph.postOrderWithIndex[source]);
        vector<int> reachableNodes;
        
        if (!reversed)
            reachableNodes = SocialGraph.GraphScheme[source];
        else 
            reachableNodes = SocialGraph.GraphSchemeReverse[source];

        for (int reachable : reachableNodes){
            intervalSchemeMap[reachable].insert(SocialGraph.postOrderWithIndex[source]);
            for (int postorder : intervalSchemeMap[source])
                intervalSchemeMap[reachable].insert(postorder);
        }
    };


    for(auto uncompressed_labels : intervalSchemeMap){
        vector<pair<int,int>> compressed_intervals;
        pair <int,int> current_interval;
        for (int i : uncompressed_labels.second){
            if(current_interval.first == 0) {
                current_interval.first = i;
                current_interval.second = i;
                continue;
            } 
            if(current_interval.second + 1 == i){
                current_interval.second = i;
            } else {
                compressed_intervals.push_back(current_interval);
                current_interval.first = i;
                current_interval.second = i;
            }
        }
            compressed_intervals.push_back(current_interval);
        file << uncompressed_labels.first;
        for (auto intervalPair : compressed_intervals){
            file << "\t" << intervalPair.first << "\t" << intervalPair.second;
        }
        file << "\n";
    }
     
    auto time = chrono::high_resolution_clock::now() - start_time;
    cout << "IntervalScheme Transitive Closure took " << time/chrono::milliseconds(1) << endl;
    return time/chrono::milliseconds(1);
}

int main(int argc, char **argv){
    string filename;
    if (argc == 2){
        filename = argv[1];
    }  else {
        cout << "Enter filename: \n";
        cin >> filename; 
    }

    double time_reverse = createIntervalScheme(filename, true);
    double time_normal = createIntervalScheme(filename, false);

    cout << "Time reverse " << time_reverse << " ms Time normal " << time_normal << "ms" << endl;     
}