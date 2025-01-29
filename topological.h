#ifndef TOPOLOGICAL_H
#define TOPOLOGICAL_H


#include "dfs.h"
#include "graph.h"

// BitVertex Topological sort helper function
void topologicalSortBitUtil(Graph *graph, int vertex, uint32_t *visited, int *stack, int *stackIndex) {
    visited[vertex / 32] |= (1U << (vertex % 32));  // Mark the current vertex as visited
    
    // Traverse adjacent vertices
    for (int i = 0; i < graph->numVertices; i++) {
        // If there is an edge and the vertex hasn't been visited
        if (hasEdge(graph, vertex, i) && !(visited[i / 32] & (1U << (i % 32)))) {
            topologicalSortBitUtil(graph, i, visited, stack, stackIndex);
        }
    }

    // Add vertex to stack
    stack[(*stackIndex)++] = vertex;
}

// Topological sort for BitVertex representation using DFS
bool topologicalSort(Graph *graph) {
    uint32_t *visited = (uint32_t *)calloc((graph->numVertices + 31) / 32, sizeof(uint32_t));
    int *stack = (int *)malloc(graph->numVertices * sizeof(int));
    int stackIndex = 0;
    
    if (!visited || !stack) {
        printf("Memory allocation failed!\n");
        free(visited);
        free(stack);
        return false;
    }
    
    // Perform DFS traversal
    //printf("\nPerforming DFS for BitVertex...\n");
    for (int i = 0; i < graph->numVertices; i++) {
        if (!(visited[i / 32] & (1U << (i % 32)))) {
            DFS(graph, i);  // Perform DFS from the starting vertex
        }
    }

    // Topological sort
    stackIndex = 0;
    for (int i = 0; i < graph->numVertices; i++) {
        if (!(visited[i / 32] & (1U << (i % 32)))) {
            topologicalSortBitUtil(graph, i, visited, stack, &stackIndex);
        }
    }

    // Clean up memory
    free(visited);
    free(stack);
    return true;
}

/// Standard graph topological sort helper function
void topologicalSortStandardUtil(StandardGraph *graph, int vertex, bool *visited, int *stack, int *stackIndex) {
    visited[vertex] = true;

    // Traverse adjacent vertices
    for (int i = 0; i < graph->numVertices; i++) {
        if (hasStandardEdge(graph, vertex, i) && !visited[i]) {
            topologicalSortStandardUtil(graph, i, visited, stack, stackIndex);
        }
    }

    // Add vertex to stack
    stack[(*stackIndex)++] = vertex;
}

// Topological sort for Standard graph using DFS
bool topologicalSortStandard(StandardGraph *graph) {
    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    int *stack = (int *)malloc(graph->numVertices * sizeof(int));
    int stackIndex = 0;
    
    if (!visited || !stack) {
        printf("Memory allocation failed!\n");
        free(visited);
        free(stack);
        return false;
    }
    
    // Perform DFS traversal
    //printf("\nPerforming DFS for Standard...\n");
    for (int i = 0; i < graph->numVertices; i++) {
        if (!visited[i]) {
            DFSStandard(graph, i);  // Perform DFS from the starting vertex
        }
    }

    // Topological sort
    stackIndex = 0;
    for (int i = 0; i < graph->numVertices; i++) {
        if (!visited[i]) {
            topologicalSortStandardUtil(graph, i, visited, stack, &stackIndex);
        }
    }

    free(visited);
    free(stack);
    return true;
}





#endif