# paris-museum-pass


studying abroad and found this pass that was 70 monuments/ museums in paris but only had 48 hours to do them all 

wanted to see if this was actually possible with only a metro pass and walking

had working solution to optimal tsp for another project, so thought i could edit it to find the best hamiltonian path between all 70 monuments. 

first i used a heuristic solution to find a semi accurate shortest path

then used a minimum spanning tree as my branch and bound algorthim to find the optimal path from my heuristic starting point

needed to make some algo changes since I no longer needed to close the loop to form a cycle


as a starting point i used only walking distances i took from the google maps api. I knew this would be impossible to do IRL cause some of the monuments are 100km apart, but a starting point nevertheless.

heuristic was quick, but optimal solution was super slow. did some research and found that anything over 30ish locations was slow.

decided to reduce dataset to monuments within 5 km of paris. was able to find optimal solution for those 30 monuments. 


am going to clean this up and then try to max out number of monuments for 10 mins of runtime as well as introduce metro. right now it is all in kilometers, becasue walking times on google are slow, however with metro i might have to use time as units instead of distance. 