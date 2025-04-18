import requests

# printing locations from file read in
def print_locations(locations : list):
    for location in locations:
        print(location)


with open("google_api.txt") as f:
    API_KEY = f.read().strip()

locations = []
with open('locations.txt', 'r') as file:
    for line in file:
        locations.append(line.strip()) 


def find_distances_from_each_others(locations):
    n = len(locations)
    adj_matrix = [[0 for _ in range(n)] for _ in range(n)]
    for i in range(n):
        for j in range(i,n):
            monument_a = locations[i].replace(' ', '+')
            monument_b = locations[j].replace(' ', '+')
            if monument_a == monument_b:
                adj_matrix[i][j] = 0
            else:
                url = f"https://maps.googleapis.com/maps/api/directions/json?origin={monument_a}&destination={monument_b}&mode=walking&key={API_KEY}"
                results = requests.get(url).json()
                distance = results["routes"][0]["legs"][0]["distance"]["text"]
                walking_distance = int(float(distance.split()[0]) * 10)
                print(walking_distance)
                adj_matrix[i][j] = walking_distance
                adj_matrix[j][i] = walking_distance


    print("Done")
    with open("distances.txt", "w") as f:
        for row in adj_matrix:
            line = ', '.join(str(cell) for cell in row)
            f.write(line + '\n')

    print("written to distances.txt")

def find_distances_from_paris(locations):
    distances_from_paris = []
    n = len(locations)
    for i in range(n):
        paris = "Paris"
        monument = locations[i].replace(' ', '+')
        url = f"https://maps.googleapis.com/maps/api/directions/json?origin={paris}&destination={monument}&mode=walking&key={API_KEY}"
        results = requests.get(url).json()
        distance = results["routes"][0]["legs"][0]["distance"]["text"]
        walking_distance = int(float(distance.split()[0]) * 10)
        print(walking_distance)
        distances_from_paris.append(walking_distance)

    print("Done")
    with open("distances_from_paris.txt", "w") as f:
        for distance in distances_from_paris:
            f.write(str(distance) + '\n')
    print("written to distances_from_paris.txt")


find_distances_from_paris(locations)
    
