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
        vector<string> locations; // holds monument names in order of adjacency matrix
        vector<Node> initial_graph; // holds x
        vector< vector<int> > adj_matrix; // holds google maps walking distances multiplied by 10x
        vector<size_t> curr_path;  // holds google maps walking distances
        vector<size_t> best_path;  // holds google maps walking distances
        double curr_distance;
        double best_distance;

    
    public:
        // input is in x coord, y coord
        void read_adj_matrix(const string& filename) {
            vector<vector<string> > matrix;
            ifstream file(filename);
        
            if (!file.is_open()) {
                cerr << "Failed to open file." << endl;
                return;
            }
        
            string line;
            while (getline(file, line)) {
                vector<string> row;
                stringstream ss(line);
                string cell;
        
                while (getline(ss, cell, ',')) {
                    row.push_back(cell);
                }
        
                matrix.push_back(row);
            }
            file.close();

            adj_matrix.resize(locations.size());
            for (int i = 0; i < matrix.size(); i++){
                adj_matrix[i].resize(locations.size());
                for (int j = 0; j < matrix.size(); j++){
                    adj_matrix[i][j] = stoi(matrix[i][j]);
                }
            }
            cout << "read in adj matrix" << endl;

        }
        void read_input(){
            string name;

            while(cin >> name){ 
                locations.push_back(name);
            }
            initial_graph.resize(locations.size());
            locations.resize(locations.size());
            
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
                    double curr_distance = adj_matrix[vertex][curr] + adj_matrix[curr][initial_graph[vertex].next_node];
                    if (curr_distance < min_distance){
                        curr_distance = min_distance;
                        insertion_point = vertex;
                        next = initial_graph[vertex].next_node;
                    }
                }
                // update initial graph with node that minimizes increase in path length
                initial_graph[insertion_point].next_node = curr;
                initial_graph[insertion_point].distance = adj_matrix[insertion_point][curr];

                initial_graph[curr].next_node = next;
                initial_graph[curr].distance = adj_matrix[curr][next];

            }
            return;
        }

        // set curr path, best path and save distance to the heuristic solution for initial distance comparisons
        // TODO: fix adding the last node that loops back to 0
        void set_best_path(){
            curr_path.resize(locations.size());
            best_path.reserve(locations.size());
            for (int i = 0; i < locations.size(); i++){           
                curr_path[i] = i;
            }

            size_t curr_node = initial_graph[0].next_node;
            best_path.push_back(curr_node);
            best_distance += initial_graph[0].distance;
            while(curr_node != 0){
                best_path.push_back(curr_node);
                best_distance += initial_graph[curr_node].distance;
                curr_node = initial_graph[curr_node].next_node;
            }
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
            for (int i = 0; i < prims_table.size(); i++){
                mst_cost += prims_table[i].distance;
            }
    
            // MST ALGO WORKS 
    
            double curr_edge = 0;
            
            //connect end of current path with mst to get estimate
            double min_edge = numeric_limits<double>::infinity();
            for (size_t i = permLength; i < curr_path.size(); i++){
                curr_edge = adj_matrix[curr_path[permLength - 1]][curr_path[i]];
                if (curr_edge < min_edge) min_edge = curr_edge;
            }
            return mst_cost + sqrt(min_edge);
        }

        // check if remainder of locations create a promising path that is less than the 
        bool promising(size_t permLength) {
            // estimate is mst NOT full cycle
            // estimate <= tsp
            // this guarantees that if curr_distance + estimate >= best distance, our current distance isn't optimal
            // so we can stop searching that branch becasue everything after it will be worse
    
            if(curr_path.size() - permLength < 6){
                return true;
            }
    
            double estimate = find_opttsp_mst(permLength);
            curr_distance = calculate_distance(permLength);
            if (curr_distance + estimate >= best_distance) return false;
            return true;
        }
    
        void genPerms(size_t permLength) {
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
                genPerms(permLength + 1);
                swap(curr_path[permLength], curr_path[i]);
            }
        }
    
        void print_best_path(){
            cout << fixed << setprecision(2) << best_distance << '\n';
            cout << best_path[0];
            for (size_t i = 1; i < best_path.size(); i++){
                cout << ' ' << best_path[i];
            }
            cout << '\n';
        }
    
        void main(){
            read_input();
            arbitrary_selection();
            set_best_path();

            genPerms(1);

        }


};