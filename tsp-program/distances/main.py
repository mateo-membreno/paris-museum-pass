import requests

with open("google_api.txt") as f:
    API_KEY = f.read().strip()

monuments = []
with open('locations.txt', 'r') as file:
    for line in file:
        monuments.append(line.strip()) 

n = len(monuments)
adj_matrix = [[0 for _ in range(n)] for _ in range(n)]



for i in range(n):
    for j in range(i,n):
        monument_a = monuments[i].replace(' ', '+')
        monument_b = monuments[j].replace(' ', '+')
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