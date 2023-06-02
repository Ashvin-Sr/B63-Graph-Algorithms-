/*
 * Graph algorithms.
 *
 * Author: A. Tafliovich.
 */

#include <limits.h>

#include "graph.h"
#include "minheap.h"

#define NOTHING -1

typedef struct records {
  int numVertices;    // total number of vertices in the graph
                      // vertex IDs are 0, 1, ..., numVertices-1
  MinHeap* heap;      // priority queue
  bool* finished;     // finished[id] is true iff vertex id is finished
                      //   i.e. no longer in the PQ
  int* predecessors;  // predecessors[id] is the predecessor of vertex id
  Edge* tree;         // keeps edges for the resulting tree
  int numTreeEdges;   // current number of edges in mst
} Records;

/*************************************************************************
 ** Suggested helper functions, to help with your program design
 *************************************************************************/
/* Creates, populates, and returns a MinHeap to be used by Prim's and
 * Dijkstra's algorightms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
MinHeap* initHeap(Graph* graph, int startVertex){
    MinHeap* heap = newHeap(graph->numVertices);
    for(int i = 0; i < graph->numVertices; i++){
        insert(heap, INT_MAX,graph->vertices[i].id);//look at this later
    }
    decreasePriority(heap, startVertex, 0);
    return heap;
}
/* Creates, populates, and returns all records needed to run Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
Records* initRecords(Graph* graph, int startVertex){
    Records *newRecords = calloc(1, sizeof(Records));
    newRecords->numVertices = graph->numVertices;
    newRecords->heap = initHeap(graph, startVertex);
    newRecords->finished = calloc(graph->numVertices, sizeof(bool));
    newRecords->predecessors = calloc(graph->numVertices, sizeof(int));
    newRecords->predecessors[startVertex] = startVertex;
    newRecords->tree = calloc(graph->numVertices, sizeof (Edge));
    newRecords->numTreeEdges = 0;
    return newRecords;
}

Edge* deleteRecords(Records *records){
    deleteHeap(records->heap);
    free(records->predecessors);
    free(records->finished);
    Edge *tree = records->tree;
    tree = records->tree;
    free(records);
    return tree;
}
/* Add a new edge to records at index ind. */
void addTreeEdge(Records* records, int ind, int fromVertex, int toVertex,
                 int weight){
    records->tree[ind].toVertex = toVertex;
    records->tree[ind].fromVertex = fromVertex;
    records->tree[ind].weight = weight;
}

/* Creates and returns a path from 'vertex' to 'startVertex' from edges
 * in the distance tree 'distTree'.
 */
AdjList* makePath(Edge* distTree, int vertex, int startVertex, AdjList *edges, int *dist_start){
    if (vertex == startVertex){
        return edges;
    }
    Edge edge = distTree[vertex];
    edges = makePath(distTree, distTree[vertex].toVertex, startVertex, NULL, dist_start);
    edges = newAdjList(newEdge(edge.fromVertex, edge.toVertex, edge.weight - *dist_start), edges);
    *dist_start = edge.weight;
    return edges;
}

/*************************************************************************
 ** Required functions.
 *************************************************************************/
/* Runs Prim's algorithm on Graph 'graph' starting from vertex with ID
 * 'startVertex', and return the resulting MST: an array of Edges.
 * Returns NULL is 'startVertex' is not valid in 'graph'.
 * Precondition: 'graph' is connected.
 */
Edge* primGetMST(Graph* graph, int startVertex){
    Records *record = initRecords(graph, startVertex);
    HeapNode heapNode;
    Vertex vertex;
    AdjList *edges;
    int id;
    for(int index = 0; index < graph->numVertices; index++){
        heapNode = extractMin(record->heap);
        record->finished[heapNode.id] = true;
        vertex = graph->vertices[heapNode.id];
        if(index != 0)
            addTreeEdge(record, index - 1, heapNode.id, record->predecessors[heapNode.id],heapNode.priority);
        edges = vertex.adjList;
        while(edges != NULL){
            id = edges->edge->toVertex;
            if(!record->finished[id] && (edges->edge->weight < getPriority(record->heap, id))){
                decreasePriority(record->heap, id, edges->edge->weight);
                record->predecessors[id] = edges->edge->fromVertex;
            }
            edges = edges->next;
        }
    }
    return deleteRecords(record);
}
/* Runs Dijkstra's algorithm on Graph 'graph' starting from vertex with ID
 * 'startVertex', and return the resulting distance tree: an array of edges.
 * Returns NULL is 'startVertex' is not valid in 'graph'.
 * Precondition: 'graph' is connected.
 */
Edge* getShortestPaths(Graph* graph, int startVertex){
    Records *record = initRecords(graph, startVertex);
    HeapNode heapNode;
    Vertex vertex;
    AdjList *edges;
    int id;
    for(int index = 0; index < graph->numVertices; index++){
        heapNode = extractMin(record->heap);
        record->finished[heapNode.id] = true;
        vertex = graph->vertices[heapNode.id];
        addTreeEdge(record, heapNode.id, heapNode.id, record->predecessors[heapNode.id], heapNode.priority);
        edges = vertex.adjList;
        while(edges != NULL){
            id = edges->edge->toVertex;
            if(!record->finished[id] && heapNode.priority + edges->edge->weight < getPriority(record->heap, id)){
                decreasePriority(record->heap, id, heapNode.priority + edges->edge->weight);
                record->predecessors[id] = heapNode.id;
            }
            edges = edges->next;
        }
    }
    return deleteRecords(record);
}

AdjList* getPaths(Edge* distTree, int numVertices, int startVertex){
    int dist_start = 0;
    AdjList *adjList = calloc(numVertices, sizeof(AdjList));
    AdjList *temp = NULL;
    for(int i = 0; i < numVertices; i++){
        dist_start = 0;
        temp = makePath(distTree, i, startVertex, NULL, &dist_start);
        if(temp != NULL)
            adjList[i] = *temp;
    }
    return adjList;
}

/*************************************************************************
 ** Provided helper functions -- part of starter code to help you debug!
 *************************************************************************/
void printRecords(Records* records) {
  if (records == NULL) return;

  int numVertices = records->numVertices;
  printf("Reporting on algorithm's records on %d vertices...\n", numVertices);

  printf("The PQ is:\n");
  printHeap(records->heap);

  printf("The finished array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->finished[i]);

  printf("The predecessors array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->predecessors[i]);

  printf("The TREE edges are:\n");
  for (int i = 0; i < records->numTreeEdges; i++) printEdge(&records->tree[i]);

  printf("... done.\n");
}
