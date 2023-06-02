/*
 * Our graph implementation.
 *
 * Author: A. Tafliovich.
 */

#include "graph.h"

/*********************************************************************
 ** Required functions
 *********************************************************************/
Edge* newEdge(int fromVertex, int toVertex, int weight){
    Edge *newEdge = malloc(sizeof (Edge));
    newEdge->fromVertex = fromVertex;
    newEdge->toVertex = toVertex;
    newEdge->weight = weight;
    return newEdge;
}

AdjList* newAdjList(Edge* edge, AdjList* next){
    AdjList *newAdjList = malloc(sizeof (AdjList));
    newAdjList->edge = edge;
    newAdjList->next = next;
    return newAdjList;
}

Graph* newGraph(int numVertices){
    Graph *newGraph = calloc(1, sizeof(Graph));
    newGraph->numVertices = numVertices;
    newGraph->vertices = calloc(numVertices, sizeof (Vertex));

    return newGraph;
}

void deleteAdjList(AdjList* head){
    AdjList *p = head;
    while (head != NULL){
        head = head->next;
        free(p);
        p = head;
    }
}

void deleteVertex(Vertex* vertex){
    free(vertex);
}

void deleteGraph(Graph* graph){
    free(graph->vertices);
    free(graph);
}
/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/

void printEdge(Edge* edge) {
  if (edge == NULL) return;
  printf("(%d -- %d, %d)", edge->fromVertex, edge->toVertex, edge->weight);
}

void printAdjList(AdjList* head) {
  while (head != NULL) {
    printEdge(head->edge);
    printf("  ");
    head = head->next;
  }
}

void printVertex(Vertex* vertex) {
  if (vertex == NULL) return;
  printf("%d: ", vertex->id);
  printAdjList(vertex->adjList);
}

void printGraph(Graph* graph) {
  if (graph == NULL) return;

  printf("Number of vertices: %d. Number of edges: %d.\n\n", graph->numVertices,
         graph->numEdges);

  for (int i = 0; i < graph->numVertices; i++) {
    printVertex(&graph->vertices[i]);
    printf("\n");
  }
  printf("\n");
}
