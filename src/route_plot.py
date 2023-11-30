import json
import matplotlib.pyplot as plt

def plot_route(filename, title):
    with open(filename, 'r') as file:
        data = json.load(file)

    x = [point['x'] for point in data]
    y = [point['y'] for point in data]

    plt.figure()
    plt.plot(x, y, marker='o')
    plt.title(title)
    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.grid(True)
    plt.show()

# Plot the route before optimization
plot_route('route_before_optimization.json', 'Route Before Optimization')

# Plot the route after optimization
plot_route('route_after_optimization.json', 'Route After Optimization')
