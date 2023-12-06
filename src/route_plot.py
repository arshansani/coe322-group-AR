import json
import matplotlib.pyplot as plt
import os
import re

def plot_route(filenames, title, subplot_position):
    plt.subplot(1, 4, subplot_position)
    
    for i, filename in enumerate(filenames):
        with open(filename, 'r') as file:
            data = json.load(file)

        x = [point['x'] for point in data]
        y = [point['y'] for point in data]

        plt.plot(x, y, marker='o')
    
    plt.title(title)
    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.grid(True)

# Function to find all the files
def find_files(pattern):
    files = [f for f in os.listdir() if re.match(pattern, f)]
    return sorted(files, key=lambda x: int(re.search(r'\d+', x).group()))

# Create a pre-sized figure for the plots
plt.figure(figsize=(24, 6))

# Find and plot routes
route_patterns = ['initial_route', 'greedy_route', 'individual_opt2_route', 'combined_opt2_route']
for i, pattern in enumerate(route_patterns):
    files = find_files(f"{pattern}_\d+.json")
    if files:
        plot_route(files, pattern.replace('_', ' ').title(), i + 1)

# Show the plots
plt.tight_layout()
plt.show()