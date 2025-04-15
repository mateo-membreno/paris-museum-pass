#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

struct Location {
    double longitude;
    double latitude;
    Location(double x_val, double y_val) : longitude(x_val), latitude(y_val) {};
};


struct Node{
    double distance;
    size_t next_node;
    Node()  : distance(numeric_limits<double>::infinity()), next_node(0) {}
};

class Paris {
    private:
        char mode;
        vector<Location> locations;
        vector<Node> initial_graph; 
        vector< vector<double> > adj_matrix; 

    
    public:
        // input is in x coord, y coord
        void read_input(){
            int longitude;
            int latitude;
            int name;

            while(cin >> name){
                cin >> longitude;
                cin >> latitude;
                Location new_coord = Location(longitude, latitude);
                locations.push_back(new_coord);
            }
            initial_graph.resize(locations.size());
            
        }

        double find_distance(Location& a, Location& b){
            // fill in with straight line distance between coords
            // later update with google maps walking distance
            // then with metro or bus
            return -1;
        }
    
        // create initial path that isn't optimal but also isn't random using arbitrary selection algorithm
        double arbitrary_selection(){
            // create initial tsp using arbitrary selection
            // start with 2 nodes
            // for each subsequent node
                // try inserting between each pair
                // insert where path increase is minimized
            return -1;
        }


};