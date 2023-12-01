import json
import matplotlib.pyplot as plt

def plot_route(filename, title, subplot_position):
    with open(filename, 'r') as file:
        data = json.load(file)

    x = [point['x'] for point in data]
    y = [point['y'] for point in data]

    plt.subplot(1, 2, subplot_position)
    plt.plot(x, y, marker='o')
    plt.title(title)
    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.grid(True)

# Create a pre-sized figure for the plots
plt.figure(figsize=(12, 6))

# Plot the route before optimization
plot_route('route_before_optimization.json', 'Route Before Optimization', 1)

# Plot the route after optimization
plot_route('route_after_optimization.json', 'Route After Optimization', 2)

# Show the plots
plt.tight_layout()
plt.show()