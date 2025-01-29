#ifndef GRAPH_H
#define GRAPH_H


// --------------------------- Standard Graph Representation ---------------------------

// Standard graph structure (Adjacency List or Matrix)
typedef struct StandardGraph{
    int **adjacencyMatrix; // Adjacency matrix
    int numVertices;       // Number of vertices
} StandardGraph;


// Function to create a standard graph (Adjacency Matrix)
StandardGraph *createStandardGraph(int numVertices) {
    StandardGraph *graph = (StandardGraph *)malloc(sizeof(StandardGraph));
    graph->numVertices = numVertices;
    graph->adjacencyMatrix = (int **)malloc(numVertices * sizeof(int *));
    for (int i = 0; i < numVertices; i++) {
        graph->adjacencyMatrix[i] = (int *)calloc(numVertices, sizeof(int));
    }
    return graph;
}

// Function to set an edge in the adjacency matrix for the standard graph
void setStandardEdge(StandardGraph *graph, int u, int v) {
    graph->adjacencyMatrix[u][v] = 1;
    graph->adjacencyMatrix[v][u] = 1;  // If undirected graph
}

// Function to check if an edge exists in the adjacency matrix for the standard graph
bool hasStandardEdge(StandardGraph *graph, int u, int v) {
    return graph->adjacencyMatrix[u][v] == 1;
}



// --------------------------- BitVertex Graph Representation ---------------------------

// Define the graph structure using BitVertex representation
typedef struct Graph {
    uint32_t *adjacencyMatrix; // Adjacency matrix stored as bits
    int numVertices;           // Number of vertices
} Graph;

Graph *createGraph(int numVertices) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (!graph) return NULL;
    
    graph->numVertices = numVertices;
    // Her satır için gereken uint32_t sayısı
    int wordsPerRow = (numVertices + 31) / 32;
    // Toplam gereken uint32_t sayısı
    int totalWords = wordsPerRow * numVertices;
    
    graph->adjacencyMatrix = (uint32_t *)calloc(totalWords, sizeof(uint32_t));
    if (!graph->adjacencyMatrix) {
        free(graph);
        return NULL;
    }
    return graph;
}

void setEdge(Graph *graph, int u, int v) {
    // Ensure that indices are within bounds
    if (u >= graph->numVertices || v >= graph->numVertices) return;

    // Set the bit for the edge u -> v
    graph->adjacencyMatrix[u * (graph->numVertices / 32) + v / 32] |= (1U << (v % 32));

    // If undirected, set the reverse edge v -> u
    graph->adjacencyMatrix[v * (graph->numVertices / 32) + u / 32] |= (1U << (u % 32));
}
bool hasEdge(Graph *graph, int u, int v) {
    if (u >= graph->numVertices || v >= graph->numVertices) return false;

    int wordsPerRow = (graph->numVertices + 31) / 32;  // Words per row
    int wordIndex = (u * wordsPerRow) + (v / 32);      // Word index for u -> v
    int bitPosition = v % 32;                         // Bit position within the word

    return (graph->adjacencyMatrix[wordIndex] & (1U << bitPosition)) != 0;
}
// Standard Graph için yönlü kenar ekleme
void setStandardDirectedEdge(StandardGraph *graph, int u, int v) {
    if (u >= graph->numVertices || v >= graph->numVertices) return;
    graph->adjacencyMatrix[u][v] = 1;  // Sadece u'dan v'ye
}

// BitVertex Graph için yönlü kenar ekleme
void setBitVertexDirectedEdge(Graph *graph, int u, int v) {
    if (u >= graph->numVertices || v >= graph->numVertices) return;
    
    int wordsPerRow = (graph->numVertices + 31) / 32;
    int wordIndex = (u * wordsPerRow) + (v / 32);
    int bitPosition = v % 32;
    
    graph->adjacencyMatrix[wordIndex] |= (1U << bitPosition);
}

bool hasStandardCycle(StandardGraph *graph, int vertex, bool *visited, bool *recursionStack) {
    visited[vertex] = true;
    recursionStack[vertex] = true;
    
    for (int i = 0; i < graph->numVertices; i++) {
        if (hasStandardEdge(graph, vertex, i)) {
            if (!visited[i]) {
                if (hasStandardCycle(graph, i, visited, recursionStack))
                    return true;
            } else if (recursionStack[i])
                return true;
        }
    }
    
    recursionStack[vertex] = false;
    return false;
}

#endif