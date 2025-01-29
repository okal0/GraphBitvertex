#ifndef BFS_H
#define BFS_H

#include "graph.h"

// BitVertex
void BFS(Graph *graph, int startVertex) {
    int wordsPerRow = (graph->numVertices + 31) / 32;  // Words per row
    uint32_t *visited = (uint32_t *)calloc(wordsPerRow, sizeof(uint32_t));  // Bitmask for visited vertices
    if (!visited) return;

    int queue[graph->numVertices], front = 0, rear = 0;
    queue[rear++] = startVertex;

    // Mark the start vertex as visited
    visited[startVertex / 32] |= (1U << (startVertex % 32));

    while (front < rear) {
        int current = queue[front++];
        // Find the correct row

        int row = current * wordsPerRow;
        for (int i = 0; i < graph->numVertices; i++) {
   
            // Check if there is an edge from current to i
            if ((graph->adjacencyMatrix[row + i / 32] & (1U << (i % 32))) ) {
                
                if(!(visited[i / 32] & (1U << (i % 32)))){
                // If there's an edge and the vertex hasn't been visited
                queue[rear++] = i;  // Add to the queue
                visited[i / 32] |= (1U << (i % 32));  // Mark as visited
                continue;
                }
            }
        }
    }


    free(visited);
}





// BFS for Standard graph representation
void BFSStandard(StandardGraph *graph, int startVertex) {
    bool visited[graph->numVertices];
    int visited_count = 0;
    for (int i = 0; i < graph->numVertices; i++) visited[i] = false;

    int queue[graph->numVertices], front = 0, rear = 0;
    queue[rear++] = startVertex;
    visited[startVertex] = true;

    while (front < rear) {
        int current = queue[front++];

        for (int i = 0; i < graph->numVertices; i++) {

            if (hasStandardEdge(graph, current, i) && !visited[i])  {
                // If there is an edge and vertex hasn't been visited
                queue[rear++] = i;
                visited[i] = true;
                continue;
            }
        }
    }

}

#endif