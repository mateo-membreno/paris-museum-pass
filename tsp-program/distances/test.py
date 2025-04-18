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

print(locations)
n = len(locations)


for i in range(36,37):
    for j in range(i,n):
        monument_a = locations[i].replace(' ', '+')
        monument_b = locations[j].replace(' ', '+')
        if monument_a == monument_b:
            pass
        else:
            url = f"https://maps.googleapis.com/maps/api/directions/json?origin={monument_a}&destination={monument_b}&mode=walking&key={API_KEY}"
            results = requests.get(url).json()
            distance = results["routes"][0]["legs"][0]["distance"]["text"]
            print(f"{i} {monument_a} --> {j} {monument_b}")
            print(distance)


