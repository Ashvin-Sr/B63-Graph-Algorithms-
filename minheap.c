 /*
 * Our min-heap implementation.
 *
 * Author (starter code): A. Tafliovich.
 */

#include "minheap.h"

#define ROOT_INDEX 1
#define NOTHING -1

/*************************************************************************
 ** Suggested helper functions to help with your program design
 *************************************************************************/

/* Returns priority of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int priorityAt(MinHeap* heap, int nodeIndex){
    return heap->arr[nodeIndex].priority;
}


/* Returns True if 'maybeIdx' is a valid index in minheap 'heap', and 'heap'
 * stores an element at that index. Returns False otherwise.
 */
bool isValidIndex(MinHeap* heap, int maybeIdx){
    return heap->size >= maybeIdx && maybeIdx > 0;
}
/* Returns the index of the left child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.  Returns NOTHING if there is no such left
 * child.
 */
int leftIdx(MinHeap* heap, int nodeIndex){
    if(!isValidIndex(heap, nodeIndex*2))
        return NOTHING;
    return nodeIndex*2;
}
/* Returns the index of the right child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.  Returns NOTHING if there is no such right
 * child.
 */

int rightIdx(MinHeap* heap, int nodeIndex){
    if(!isValidIndex(heap, nodeIndex*2 + 1))
        return NOTHING;
    return nodeIndex*2 + 1;
}

/* Returns the index of the parent of a node at index 'nodeIndex' in minheap
 * 'heap', if such exists.  Returns NOTHING if there is no such parent.
 */
int parentIdx(MinHeap* heap, int nodeIndex){
    if(nodeIndex == ROOT_INDEX || !isValidIndex(heap, nodeIndex))
        return NOTHING;
    return nodeIndex/2;
}


/* Swaps contents of heap->arr[index1] and heap->arr[index2] if both 'index1'
 * and 'index2' are valid indices for minheap 'heap'. Has no effect
 * otherwise.
 */
void swap(MinHeap* heap, int index1, int index2){
    HeapNode temp = heap->arr[index1];

    heap->arr[index1].priority = heap->arr[index2].priority;
    heap->arr[index1].id = heap->arr[index2].id;
    heap->indexMap[heap->arr[index1].id] = index1;

    heap->arr[index2].priority = temp.priority;
    heap->arr[index2].id = temp.id;
    heap->indexMap[heap->arr[index2].id] = index2;

}

/* Bubbles up the element newly inserted into minheap 'heap' at index
 * 'nodeIndex', if 'nodeIndex' is a valid index for heap. Has no effect
 * otherwise.
 */
void bubbleUp(MinHeap* heap, int nodeIndex){
    if(!isValidIndex(heap, nodeIndex))
        return;
    while(nodeIndex != ROOT_INDEX && isValidIndex(heap, nodeIndex) && priorityAt(heap, nodeIndex) < priorityAt(heap, parentIdx(heap, nodeIndex))){
        swap(heap, nodeIndex, parentIdx(heap, nodeIndex));
        nodeIndex = parentIdx(heap, nodeIndex);
    }
}
int lowerPriorityIndexChild(MinHeap* heap, int nodeIndex){
    if(!isValidIndex(heap, nodeIndex))
        return NOTHING;
    else if(leftIdx(heap, nodeIndex) == heap->size ||
            priorityAt(heap, leftIdx(heap, nodeIndex)) <= priorityAt(heap, rightIdx(heap, nodeIndex)))
        return leftIdx(heap, nodeIndex);
    return rightIdx(heap, nodeIndex);
}
/* Bubbles down the element newly inserted into minheap 'heap' at the root,
 * if it exists. Has no effect otherwise.
 */
void bubbleDown(MinHeap* heap){
    if(!isValidIndex(heap, 1))
        return;
    int nodeIndex = ROOT_INDEX;
    int lowerPriorityIndex;
    bool b = true;
    while(isValidIndex(heap ,nodeIndex) && b){
        lowerPriorityIndex = lowerPriorityIndexChild(heap, nodeIndex);
        if(priorityAt(heap, lowerPriorityIndex) < priorityAt(heap, nodeIndex) && lowerPriorityIndex != NOTHING) {
            swap(heap, lowerPriorityIndex, nodeIndex);
            nodeIndex = lowerPriorityIndex;
        }
        else
            b = false;
    }
}



/* Doubles the capacity of minheap 'heap'.
 */
void doubleCapacity(MinHeap* heap){
    HeapNode* arr = calloc(2*heap->capacity + 1, sizeof (HeapNode));
    int* indexMap = calloc(2*heap->capacity + 1, sizeof (int));

    for(int i = 1; i <= heap->size; i++){
        arr[i] = heap->arr[i];
        indexMap[i] = heap->indexMap[i];
    }

    free(heap->arr);
    free(heap->indexMap);

    heap->arr = arr;
    heap->indexMap = indexMap;
    heap->capacity = heap->capacity*2;
}

/*********************************************************************
 * Required functions
 ********************************************************************/
HeapNode getMin(MinHeap* heap){
    return heap->arr[ROOT_INDEX];
}

HeapNode extractMin(MinHeap* heap){
    swap(heap, ROOT_INDEX, heap->size);
    heap->size--;
    heap->indexMap[heap->size] = 0;
    bubbleDown(heap);
    return heap->arr[heap->size+1];
}

void insert(MinHeap* heap, int priority, int id){
    if(heap->size == heap->capacity)
        doubleCapacity(heap);
    heap->size++;
    heap->arr[heap->size].priority = priority;
    heap->arr[heap->size].id = id;
    heap->indexMap[id] = heap->size;
    bubbleUp(heap, heap->size);
}

bool decreasePriority(MinHeap* heap, int id, int newPriority){
    if(priorityAt(heap, heap->indexMap[id]) >= newPriority) {
        heap->arr[heap->indexMap[id]].priority = newPriority;
        bubbleUp(heap, heap->indexMap[id]);
        return true;
    }
    return false;
}
int idAt(MinHeap *heap, int index){
    return heap->arr[index].id;
}

int indexOf(MinHeap *heap, int id){
    return heap->indexMap[id];
}
int getPriority(MinHeap* heap, int id){
    return heap->arr[heap->indexMap[id]].priority;
}
/*********************************************************************
 ** Helper functions provided in the starter code
 *********************************************************************/
void printHeap(MinHeap* heap) {
  printf("MinHip with size: %d\n\tcapacity: %d\n\n", heap->size,
         heap->capacity);
  printf("index: priority [ID]\t ID: index\n");
  for (int i = 0; i < heap->capacity; i++)
    printf("%d: %d [%d]\t\t%d: %d\n", i, priorityAt(heap, i), idAt(heap, i), i,
           indexOf(heap, i));
  printf("%d: %d [%d]\t\t\n", heap->capacity, priorityAt(heap, heap->capacity),
         idAt(heap, heap->capacity));
  printf("\n\n");
}

MinHeap* newHeap(int capacity){
    MinHeap *heap = calloc(1, sizeof(MinHeap));
    heap->capacity = capacity;
    heap->arr = calloc(capacity + 1, sizeof (HeapNode));
    heap->indexMap = calloc(capacity + 1, sizeof (int));
    return heap;
}

void deleteHeap(MinHeap* heap){
    free(heap->arr);
    free(heap->indexMap);
    free(heap);
}