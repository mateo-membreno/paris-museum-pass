#include <cstdlib>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <iomanip> 
#include <fstream>
#include <sstream>
#

using namespace std;




struct Node{
    double distance;
    size_t next_node;
    Node()  : distance(numeric_limits<double>::infinity()), next_node(0) {}
};

struct Vertex{
    bool visited; // Kv
    double distance; // Dv
    size_t parent; // Pv
    Vertex()  : visited(false), distance(numeric_limits<double>::infinity()), parent(0) {}
};

class Paris {
    private:
        vector< pair<string, size_t> > locations; // holds monument names and index in full list of monumentrs
        vector<Node> initial_graph; // holds x
        vector< vector<int> > adj_matrix;  // holds google maps walking distances multiplied by 10x
        vector<size_t> curr_path;  // holds indecies of locations
        vector<size_t> best_path;  // holds indeces of locations
        double curr_distance;
        double best_distance;
        string adj_matrix_filename;
        string locations_filename;
        string dist_from_paris_filename;
        int perm_count;

    
    public:
        // input is in x coord, y coord
        Paris() : locations(), initial_graph(), adj_matrix(), curr_path(), best_path(), curr_distance(0), best_distance(0), adj_matrix_filename("distances/distances.csv"), locations_filename("distances/locations.txt"), dist_from_paris_filename("distances/distances_from_paris.txt"), perm_count(0) {}
        void initialize_vars(int max_kilometers){

            // read locations and distance from paris
            vector<string> all_locations;
            vector <int> dist_from_paris;

            ifstream file(locations_filename);
            if (!file.is_open()) {
                cerr << "Failed to open file.: " << locations_filename << endl;
                return;
            }
        
            string line;
            while (getline(file, line)) {
                all_locations.push_back(line);
            }
            file.close();
            
            file.open(dist_from_paris_filename);
            while (getline(file, line)) {
                dist_from_paris.push_back(stoi(line));
            }
            file.close();

            // get number of valid locations within 'max_kilometers' of paris

            for (size_t i = 0; i < all_locations.size(); i++){
                if (dist_from_paris[i] <= max_kilometers * 10){ // multipy by 10 because we store a decimal place in integer
                    locations.push_back(make_pair(all_locations[i], i));
                }
            }


            // read adj matrix from file
            vector<vector<int> > matrix;
            file.open(adj_matrix_filename);
        
            if (!file.is_open()) {
                cerr << "Failed to open file." << endl;
                return;
            }
        
            while (getline(file, line)) {
                vector<int> row;
                stringstream ss(line);
                string cell;
        
                while (getline(ss, cell, ',')) {
                    row.push_back(stoi(cell));
                }
        
                matrix.push_back(row);
            }
            file.close();
            cout << "read full adj matrix completed" << endl;

            adj_matrix.resize(locations.size());
            for (size_t i = 0; i < locations.size(); i++){
                adj_matrix[i].resize(locations.size());
                for (size_t j = 0; j< locations.size(); j++){
                    adj_matrix[i][j] = matrix[locations[i].second][locations[j].second];
                }

            }
            cout << "reduced adj matrix completed" << endl;



            initial_graph.resize(adj_matrix[0].size());

            cout << "arbitrary selection graph initialized" << endl;
            

        }
        void print_locations(){
            for (size_t i = 0; i < locations.size(); i++){
                cout << locations[i].first << endl;
            }
            cout << locations.size() << " locations to visit" << endl;
        }

        void print_adj_matrix(){
            for (size_t i = 0; i < adj_matrix[0].size(); i++){
                for (size_t j = 0; j < adj_matrix[0].size(); j++){
                    cout << adj_matrix[i][j] << ",";
                }
                cout << endl;
            }
        }

        // create initial path that isn't optimal but also isn't random using arbitrary selection algorithm
        void arbitrary_selection(){
            // create initial tsp using arbitrary selection
            // start with 2 nodes
            // for each subsequent node
                // try inserting between each pair
                // insert where path increase is minimized

            // initialize first two nodes with distance between themselves
            double distance = adj_matrix[0][1];
            initial_graph[0].next_node = 1;
            initial_graph[0].distance = distance;
            initial_graph[1].next_node = 0;
            initial_graph[1].distance = distance;

            for (size_t curr = 2; curr < initial_graph.size(); curr++){
                double min_distance = numeric_limits<double>::infinity();
                // these can be set arbitrarily bc they will always be updated at least once
                size_t insertion_point = 0;
                size_t next = 0;


                for (size_t vertex = 0; vertex < curr; vertex++){
                    double curr_distance = adj_matrix[vertex][curr] + adj_matrix[curr][initial_graph[vertex].next_node] - adj_matrix[vertex][initial_graph[vertex].next_node];
                    if (curr_distance < min_distance){
                        min_distance = curr_distance;
                        insertion_point = vertex;
                        next = initial_graph[vertex].next_node;
                    }
                }
                // update initial graph with node that minimizes increase in path length
                initial_graph[curr].next_node = next;
                initial_graph[curr].distance = adj_matrix[curr][next];

                initial_graph[insertion_point].next_node = curr;
                initial_graph[insertion_point].distance = adj_matrix[insertion_point][curr];


            }
            cout << "completed arbitrary selection" << endl;
            return;
        }

        // set curr path, best path and save distance to the heuristic solution for initial distance comparisons
        // TODO: fix adding the last node that loops back to 0
        void set_intial_best_path(){
            curr_path.resize(locations.size());
            best_path.reserve(locations.size());
            for (size_t i = 0; i < locations.size(); i++){           
                curr_path[i] = i;
            }

            size_t curr_node = initial_graph[0].next_node;
            best_path.push_back(0);
            best_distance += initial_graph[0].distance;
            while(curr_node != 0){
                best_path.push_back(curr_node);
                best_distance += initial_graph[curr_node].distance;
                curr_node = initial_graph[curr_node].next_node;
            }
            // remove loop back to start
            best_distance -= initial_graph[curr_node].distance;
        }

        void print_best_path(){
            for (size_t i = 0; i < best_path.size() - 1; i++){
                cout << locations[best_path[i]].first << " " << best_path[i] << endl;
                cout << adj_matrix[best_path[i]][best_path[i+1]]/10.0 <<  " km." << endl;
            }
            cout << locations[best_path[best_path.size() - 1]].first << " " << best_path[best_path.size() - 1] << endl;
            cout << "Best distance: " << best_distance/10.0 << " total km." << endl;
            cout << "Best Path location count: " << locations.size() << endl;

        }

        // calculates distance from node 0 to perm_length along path
        double calculate_distance(size_t permLenth) {
            double totalDistance = 0;
            for (size_t i = 0; i < permLenth - 1; i++) {
                totalDistance += adj_matrix[curr_path[i]][curr_path[i + 1]];
            }
            return totalDistance;
        }
        
        // only updates when perm length is enitre path size
        void update(size_t permLenth){
            double distance = calculate_distance(permLenth);
            if (distance < best_distance){
                best_distance = distance;
                best_path = curr_path;
            }
        }

        double find_opttsp_mst(size_t permLength){
            double mst_cost = 0;
            // need to initialize prim table to current path length
            vector<Vertex> prims_table;
            prims_table.resize(curr_path.size() - permLength);

            prims_table[0].distance = 0;

            for (size_t i = 0; i < prims_table.size(); i++){
                double min_distance = numeric_limits<double>::infinity();
                size_t min_vertex = 0;

                // step 1
                for (size_t v = 0; v < prims_table.size(); v++){
                    if (prims_table[v].visited == false && prims_table[v].distance < min_distance) {
                        min_vertex = v;
                        min_distance = prims_table[v].distance;
                    }
                }
                // step 2
                prims_table[min_vertex].visited = true;

                // step 3
                for (size_t v = 0; v < prims_table.size(); v++){
                    double new_distance = adj_matrix[curr_path[min_vertex + permLength]][curr_path[v + permLength]];
                    if (prims_table[v].visited == false && new_distance < prims_table[v].distance){
                        prims_table[v].distance = new_distance;
                        prims_table[v].parent = min_vertex;
                    }
                }
            }
            // find cost of mst
            for (size_t i = 0; i < prims_table.size(); i++){
                mst_cost += prims_table[i].distance;
            }

            // MST ALGO WORKS 

            double curr_edge = 0;
            //now connect beginning and end of curr_path with the closest vertex in path

            double min_edge_two = numeric_limits<double>::infinity();
            for (size_t i = permLength; i < curr_path.size(); i++){
                curr_edge = adj_matrix[curr_path[permLength - 1]][curr_path[i]];
                if (curr_edge < min_edge_two) min_edge_two = curr_edge;
            }
            return mst_cost + min_edge_two;
        }

        bool promising(size_t permLength) {
            // estimate is mst NOT full cycle
            // estimate <= tsp
            // this guarantees that if curr_distance + estimate >= best distance, our current distance isn't optimal
            // so we can stop searching that branch becasue everything after it will be worse

            // connect path[0] and path[permLength - 1] to mst 
            // add those lengths to estimate
            if(curr_path.size() - permLength < 6){
                return true;
            }

            double estimate = find_opttsp_mst(permLength);
            curr_distance = calculate_distance(permLength);
            if (curr_distance + estimate >= best_distance) return false;
            return true;
        }

        void genPerms(size_t permLength, int& count) {
            
            // counter for infinite recursion purposes
            //cout << counter << '\n';
            //counter++;
            if (permLength == curr_path.size()) {
                update(permLength);
                return;
            }
            if (!promising(permLength)) {
                return;
            }

            for (size_t i = permLength; i < curr_path.size(); ++i) {
                swap(curr_path[permLength], curr_path[i]);
                count += 1;
                if (count % 100000 == 0){
                    cout << count << " is the count" << endl;
                }
                if (count % 100000000 == 0){
                    print_best_path();
                }
                genPerms(permLength + 1, count);
                swap(curr_path[permLength], curr_path[i]);
            }
        }
    
        void main(){
            initialize_vars(5);
            arbitrary_selection();
            set_intial_best_path();
            print_best_path();

            genPerms(1, perm_count);

            print_best_path();
            
        }


};