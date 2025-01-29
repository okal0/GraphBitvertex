#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <string.h>


#include "bfs.h"
#include "dfs.h"
#include "topological.h"



// Function to generate a random graph (same for both BitVertex and Standard)
void generateRandomGraph(Graph *bitGraph, StandardGraph *stdGraph, int numEdges) {
    for (int i = 0; i < numEdges; i++) {
        int u = rand() % bitGraph->numVertices;
        int v = rand() % bitGraph->numVertices;
        if (u != v) { // avoid self-loops
            setEdge(bitGraph, u, v);  // BitVertex
            setStandardEdge(stdGraph, u, v);  // Standard graph
        }
    }
}



double measureExecutionTime(void (*function)(void *, int), void *graph, int startVertex) {
    clock_t start = clock();
    function(graph, startVertex);
    clock_t end = clock();
    return ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;  // Convert to milliseconds
}


void saveExecutionTimesToFile(const char *filename, const char *algorithm, double bitVertexTime, double standardTime, int graphSize, int numEdges) {
    FILE *file = fopen(filename, "a");  // Open file in append mode
    if (file == NULL) {
        perror("Unable to open file");
        return;
    }
    
    // Append the execution times for the given algorithm
    fprintf(file, "Graph Size: %d, Edges: %d\n", graphSize, numEdges);
    fprintf(file, "Algorithm,BitVertex (ms),Standard (ms)\n");
    fprintf(file, "%s,%f,%f\n", algorithm, bitVertexTime, standardTime);
    
    fclose(file);
}


int main() {
    srand(time(NULL)); 
    // Define graph sizes and edges for comparison
    int graphSizes[5] = {50, 100, 250, 500, 1000}; // Different graph sizes
    int numEdges[5] = {250, 500,1250, 3000, 5000}; // Different numbers of edges

    // Open the files and write the headers only once
    FILE *bfsFile = fopen("bfs_execution_times.txt", "w");
    if (bfsFile == NULL) {
        perror("Unable to open file");
        return 1;
    }
    fprintf(bfsFile, "Graph Size,Edges,Algorithm,BitVertex (ms),Standard (ms)\n");
    fclose(bfsFile);

    FILE *dfsFile = fopen("dfs_execution_times.txt", "w");
    if (dfsFile == NULL) {
        perror("Unable to open file");
        return 1;
    }
    fprintf(dfsFile, "Graph Size,Edges,Algorithm,BitVertex (ms),Standard (ms)\n");
    fclose(dfsFile);

    FILE *tpsFile = fopen("topological_execution_times.txt", "w");
    if (tpsFile == NULL) {
        perror("Unable to open file");
        return 1;
    }
    fprintf(tpsFile, "Graph Size,Edges,Algorithm,BitVertex (ms),Standard (ms)\n");
    fclose(tpsFile);


    // Loop over different graph sizes and edge counts
    for (int i = 0; i < 5; i++) {
        int graphSize = graphSizes[i];
        int numEdge = numEdges[i];

        // Create graphs
        Graph *bitGraph = createGraph(graphSize);
        StandardGraph *stdGraph = createStandardGraph(graphSize);

        // Generate random graphs
        generateRandomGraph(bitGraph, stdGraph, numEdge);

        // Measure BFS execution time for BitVertex and Standard graphs
        double bitVertexBFS = measureExecutionTime((void (*)(void *, int))BFS, bitGraph, 0);
        double standardBFS = measureExecutionTime((void (*)(void *, int))BFSStandard, stdGraph, 0);


        // Measure DFS execution time 
        double bitVertexDFS = measureExecutionTime((void (*)(void *, int))DFS, bitGraph, 0);
        double standardDFS = measureExecutionTime((void (*)(void *, int))DFSStandard, stdGraph, 0);
        

        // Measure topological sort execution time
        double bitVertexTPS = measureExecutionTime((void (*)(void *, int))topologicalSort, bitGraph, 0);
        double standardTPS = measureExecutionTime((void (*)(void *, int))topologicalSortStandard, stdGraph, 0);


        saveExecutionTimesToFile("bfs_execution_times.txt", "BFS", bitVertexBFS, standardBFS, graphSize, numEdge);
        saveExecutionTimesToFile("dfs_execution_times.txt", "DFS", bitVertexDFS, standardDFS, graphSize, numEdge);
        saveExecutionTimesToFile("topological_execution_times.txt", "TPS", bitVertexTPS, standardTPS, graphSize, numEdge);


        // Free allocated memory
        free(bitGraph->adjacencyMatrix);
        free(bitGraph);
        for (int j = 0; j < stdGraph->numVertices; j++) {
            free(stdGraph->adjacencyMatrix[j]);
        }
        free(stdGraph->adjacencyMatrix);
        free(stdGraph);
    }

    return 0;
}


