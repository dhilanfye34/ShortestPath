# Geographic Navigation System

This project implements a Geographic Navigation System using Dijkstra's algorithm to find the shortest path between two locations based on intersections and road connections. The program utilizes a variety of data structures, including hash tables, vectors, and a priority queue, to efficiently handle and process large geographical data.

## Features

- **Dijkstra's Algorithm**: Calculates the shortest path between two intersections based on road distances.
- **Hash Table**: Efficiently stores and retrieves named places with associated metadata.
- **Custom Priority Queue**: Manages the priority of intersections based on their distance during pathfinding.
- **Geographical Data**:
  - Reads states, named places, intersections, and connections from input files.
  - Determines directions (e.g., N, NE, SW) between intersections using latitude and longitude.
- **Path Printing**:
  - Displays the route, road names, and distances for the shortest path.
  - Outputs the total distance for the journey.

## File Requirements

The program reads data from the following input files:
1. `states.txt` - List of state abbreviations and names.
2. `named-places.txt` - Metadata for named places (e.g., cities, towns).
3. `intersections.txt` - Details of intersections with latitude, longitude, and distances.
4. `connections.txt` - Road connections between intersections, including road names and distances.

Make sure these files are available in the working directory.

## How to Run

1. Clone the repository and navigate to the project directory:
   git clone <repository_url>
   cd <repository_directory>


Compile the program:

g++ -o geo_navigation main.cpp

Run the executable:

./geo_navigation

Follow the prompts to:

Enter the starting city and state.
Enter the destination city and state.

Example Workflow
User Input:
Starting city and state.
Destination city and state.
Output:
Shortest path between the starting and destination intersections.
Total distance of the route.
Directions for each road segment.

Key Classes and Functions
hashtable:
Stores places and supports retrieval by name and state.
Resizes dynamically as more places are added.

PriorityQueue:
Implements a min-heap to prioritize intersections with the smallest distances during Dijkstra's algorithm.

dijkstra:
Computes the shortest path from a starting intersection to a destination intersection.

print_path:
Outputs the step-by-step directions for the shortest path, including road names and distances.

