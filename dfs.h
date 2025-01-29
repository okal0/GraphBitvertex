#ifndef DFS_H
#define DFS_H

#include "graph.h"

// DFS for Bitvertex
void DFSUtil(Graph *graph, int vertex, uint32_t *visited) {
    visited[vertex / 32] |= (1U << (vertex % 32));  // Mark the current vertex as visited


    // Check adjacent vertices using bitwise operations
    for (int i = 0; i < graph->numVertices; i++) {
        int row = vertex * (graph->numVertices / 32) + i / 32;
        if ((graph->adjacencyMatrix[row] & (1U << (i % 32))) && !(visited[i / 32] & (1U << (i % 32)))) {
            // If there is an edge and the vertex hasn't been visited
            DFSUtil(graph, i, visited);  // Recursively call DFS for the adjacent vertex
        }
    }
}

void DFS(Graph *graph, int startVertex) {
    int wordsPerRow = (graph->numVertices + 31) / 32;
    uint32_t *visited = (uint32_t *)calloc(wordsPerRow, sizeof(uint32_t));  // Bitmask for visited vertices
    if (!visited) return;

    DFSUtil(graph, startVertex, visited);  // Start DFS from the 'startVertex'


    free(visited);
}




// DFS for Standard graph representation
void DFSStandardUtil(StandardGraph *graph, int vertex, bool *visited) {
    visited[vertex] = true;
    for (int i = 0; i < graph->numVertices; i++) {
        if (hasStandardEdge(graph, vertex, i) && !visited[i]) {
            DFSStandardUtil(graph, i, visited);
        }
    }
}

void DFSStandard(StandardGraph *graph, int startVertex) {
    bool visited[graph->numVertices];
    for (int i = 0; i < graph->numVertices; i++) visited[i] = false;
    DFSStandardUtil(graph, startVertex, visited);
}

bool hasCycle(Graph *graph, int vertex, uint32_t *visited, uint32_t *recursionStack) {
    int wordsPerRow = (graph->numVertices + 31) / 32;
    
    // vertex işaretle
    visited[vertex / 32] |= (1U << (vertex % 32));
    recursionStack[vertex / 32] |= (1U << (vertex % 32));
    
    for (int i = 0; i < graph->numVertices; i++) {
        // edge check
        if (hasEdge(graph, vertex, i)) {
            // unvisited
            if (!(visited[i / 32] & (1U << (i % 32)))) {
                if (hasCycle(graph, i, visited, recursionStack))
                    return true;
            }
            // vertex stackde mi?
            else if (recursionStack[i / 32] & (1U << (i % 32)))
                return true;
        }
    }
    

    recursionStack[vertex / 32] &= ~(1U << (vertex % 32));
    return false;
}

#endif