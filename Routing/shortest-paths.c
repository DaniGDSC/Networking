// Dijkstra's algorithm in C to calculate the shortest paths based on routing databases

#include <stdio.h>
#include <limits.h>

#define V 6  // Number of vertices in the graph

// Function to find the vertex with the minimum distance value
int minDistance(int dist[], int sptSet[]) {
    int min = INT_MAX, min_index = -1;

    for (int v = 0; v < V; v++) {
        if (sptSet[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

// Function to print the shortest path from the source to all vertices
void printSolution(int dist[]) {
    printf("Vertex \t Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t %d\n", i, dist[i]);
}

// Function that implements Dijkstra's single-source shortest path algorithm
void dijkstra(int graph[V][V], int src) {
    int dist[V];  // The output array. dist[i] will hold the shortest distance from src to i
    int sptSet[V]; // sptSet[i] will be true if vertex i is included in the shortest path tree

    // Initialize all distances as INFINITE and sptSet[] as false
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = 0;
    }

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Find the shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not yet processed
        int u = minDistance(dist, sptSet);

        // If minDistance returns -1, it means all remaining vertices are inaccessible from the source
        if (u == -1) {
            break;
        }

        // Mark the picked vertex as processed
        sptSet[u] = 1;

        // Update dist[] value of the adjacent vertices of the picked vertex
        for (int v = 0; v < V; v++) {
            // Update dist[v] if it's not in sptSet, there's an edge from u to v, and total weight of path from src to v through u is smaller than the current value of dist[v]
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
        }
    }

    // Print the calculated shortest distances
    printSolution(dist);
}

int main() {
    // Example graph represented as an adjacency matrix
    int graph[V][V] = {
        {0, 10, 0, 0, 0, 8},
        {10, 0, 5, 0, 0, 0},
        {0, 5, 0, 3, 0, 0},
        {0, 0, 3, 0, 7, 6},
        {0, 0, 0, 7, 0, 1},
        {8, 0, 0, 6, 1, 0}
    };

    int src = 0;  // Source vertex (starting point)
    dijkstra(graph, src);

    return 0;
}