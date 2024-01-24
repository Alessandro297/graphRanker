#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define DK_LEN 23
#define COMMAND_LEN 15

//nodo del grafo
typedef struct Node{
    int index;
    unsigned int cost;
}Node;

//struttura Heap: Min Heap per la coda a priorità per Dijkstra, Max Heap per la gestione della classifica
typedef struct Heap{
    int size;
    int count;
    Node *array;
}Heap;

//legge i pesi degli archi del grafo (matrice di adiacenza) in input
unsigned int readMat(){
    unsigned int num = 0;
    int c;

    c = getchar_unlocked();
    if(c == '\n'){
        c = getchar_unlocked();
    }
    for(; (c > 47 && c < 58); c = getchar_unlocked()){
        num = num * 10 + c - 48;
    }
    if(num == 0){
        num = UINT_MAX;
    }

    return num;
}

/*
void printMat(int index, int d, unsigned int mat[][d]){
    printf("Matrice di adiacenza grafo %d:\n", index);
    for(int i=0; i<d; i++){
        for(int j=0; j<d; j++){
            if(j==(d-1)){
                printf("%u", mat[i][j]);
            }else {
                printf("%u,", mat[i][j]);
            }
        }
        printf("\n");
    }
}
*/

//creazione e inizializzazione Heap
Heap *initHeap(int size){
    Heap *heap = (Heap *)malloc(sizeof(struct Heap));

    if(heap == NULL){
        printf("Errore memoria heap\n");
        exit(100);
    }

    heap->size = size;
    heap->count = 0;
    heap->array = (Node *)malloc(size*sizeof(Node));

    if(heap->array == NULL){
        printf("Errore memoria array\n");
        exit(150);
    }

    return heap;
}

//funzione swap
void swap(Node *n1, Node *n2){
    Node tempNode = *n1;
    *n1 = *n2;
    *n2 = tempNode;
}

//costruzione bottom-up del min heap
void minHeapifyBottomUp(Heap *heap, int c){
    int parentNode = (c-1)/2;

    if(heap->array[parentNode].cost > heap->array[c].cost){
        swap(&heap->array[parentNode],&heap->array[c]);
        minHeapifyBottomUp(heap,parentNode);
    }
}

//inserimento in min heap
void insertMinHeap(Heap *heap, Node *node){
    if(heap->count < heap->size){
        heap->array[heap->count].index = node->index;
        heap->array[heap->count].cost = node->cost;

        minHeapifyBottomUp(heap,heap->count);
        heap->count++;
    }
}

//costruzione bottom-up del max heap
void maxHeapifyBottomUp(Heap *heap, int c){
    int parentNode = (c-1)/2;

    if(heap->array[parentNode].cost < heap->array[c].cost){
        swap(&heap->array[parentNode],&heap->array[c]);
        maxHeapifyBottomUp(heap,parentNode);
    }
}

//inserimento in max heap
void insertMaxHeap(Heap *heap, Node *node){
    if(heap->count < heap->size){
        heap->array[heap->count].index = node->index;
        heap->array[heap->count].cost = node->cost;

        maxHeapifyBottomUp(heap,heap->count);
        heap->count++;
    }
}

//funzione min heapify
void minHeapify(Heap *heap, int parent){
    int left = parent*2 +1;
    int right = parent*2 +2;
    int min;

    if(left < heap->count && heap->array[left].cost < heap->array[parent].cost){
        min = left;
    }else{
        min = parent;
    }
    if(right < heap->count && heap->array[right].cost < heap->array[min].cost){
        min = right;
    }

    if(min != parent){
        swap(&heap->array[min],&heap->array[parent]);
        minHeapify(heap,min);
    }
}

//estrazione minimo dal min heap
Node extractMin(Heap *heap){
    Node minNode;

    if(heap->count == 0){
        printf("Min Heap vuoto\n");
        exit(300);
    }

    minNode = heap->array[0];
    heap->array[0] = heap->array[heap->count -1];
    heap->count--;
    minHeapify(heap,0);

    return minNode;
}

//funzione max heapify
void maxHeapify(Heap *heap, int parent){
    int left = parent*2 +1;
    int right = parent*2 +2;
    int max;

    if(left < heap->count && heap->array[left].cost > heap->array[parent].cost){
        max = left;
    }else{
        max = parent;
    }
    if(right < heap->count && heap->array[right].cost > heap->array[max].cost){
        max = right;
    }

    if(max != parent){
        swap(&heap->array[parent],&heap->array[max]);
        maxHeapify(heap,max);
    }
}

//decrementa il valore del costo del nodo del grafo in min heap
void decreaseCost(Heap *heap, int index, unsigned int newCost){
    for(int i=0; i<heap->count; i++){
        if(heap->array[i].index == index){
            heap->array[i].cost = newCost;

            minHeapifyBottomUp(heap,i);
            break;
        }
    }
}

//decrementa il valore del costo del nodo del grafo in max heap
void decreaseMaxCost(Heap *heap, int index, unsigned int newCost){
    heap->array[0].index = index;
    heap->array[0].cost = newCost;

    maxHeapify(heap,0);
}

/*
void printMinHeap(Heap *heap){
    printf("\nMin Heap:\n");
    for(int i=0; i<heap->count; i++){
        printf("-> %d - %u ", heap->array[i].index, heap->array[i].cost);
    }
    printf("//\n");
}

void printMaxHeap(Heap *maxHeap){
    printf("\nMax Heap:\n");
    for(int i=0; i<maxHeap->count; i++){
        printf("-> %d - %u ", maxHeap->array[i].index, maxHeap->array[i].cost);
    }
    printf("//\n");
}

void printDist(unsigned int dist[], int d){
    printf("Vettore distanze:\n");
    for(int i=0; i<d; i++){
        printf("%u ", dist[i]);
    }
}
*/

int compare(const void *x, const void *y){
    return (*(int*)x - *(int*)y);
}

//funzione ordina crescente e stampa per testing
void SortAndPrint(Heap *maxHeap){
    int ranking[maxHeap->count];

    for(int i=0; i<maxHeap->count; i++){
        ranking[i] = maxHeap->array[i].index;
    }

    qsort(ranking,maxHeap->count,sizeof(int),compare);

    printf("Classifica ordinata crescente:\n");
    for(int i=0; i<maxHeap->count; i++){
        if(i == maxHeap->count -1){
            printf("%d", ranking[i]);
        }else{
            printf("%d ", ranking[i]);
        }
    }
    printf("\n");
}

//funzione Aggiungi Grafo
void addGraph(int index, int d, Heap *maxHeap) {
    unsigned int mat[d][d];
    unsigned int dist[d];
    Heap *heap = NULL;
    Node *tempNode = (Node *)malloc(sizeof(Node));
    Node minNode;
    unsigned int sumMinimumPaths = 0;

    for(int i=0; i<d; i++){
        for(int j=0; j<d; j++){
            mat[i][j]=readMat();
        }
    }
    //printMat(index,d,mat);

    //algoritmo di Dijkstra
    //creazione coda a priorità e inizializzazione vettore dist
    heap = initHeap(d);
    for(int i=0; i<d; i++){
        if(i == 0){
            dist[i] = 0;
        }else {
            dist[i] = UINT_MAX;
        }
        tempNode->index = i;
        tempNode->cost = dist[i];
        insertMinHeap(heap,tempNode);
    }
    //printDist(dist,d);
    //printMinHeap(heap);

    //algoritmo
    while(heap->count != 0){
        minNode = extractMin(heap);

        if(dist[minNode.index] == UINT_MAX){
            break;
        }

        for(int j=0; j<d; j++){
            if(j != minNode.index && mat[minNode.index][j] != UINT_MAX && dist[j] > dist[minNode.index] + mat[minNode.index][j]){
                decreaseCost(heap,j,dist[minNode.index] + mat[minNode.index][j]);
                dist[j] = dist[minNode.index] + mat[minNode.index][j];
            }
        }
    }
    //printMinHeap(heap);
    //printDist(dist,d);

    free(heap->array);
    free(heap);

    for(int i=1; i<d; i++){
        if(dist[i] != UINT_MAX){
            sumMinimumPaths += dist[i];
        }
    }
    //printf("Somma cammini minimi grafo %d: %u\n", index, sumMinimumPaths);

    //inserimento in classifica (in max heap)
    if(maxHeap->count < maxHeap->size){
        tempNode->index = index;
        tempNode->cost = sumMinimumPaths;
        insertMaxHeap(maxHeap,tempNode);
    }else{
        if(maxHeap->array[0].cost > sumMinimumPaths){
            decreaseMaxCost(maxHeap,index,sumMinimumPaths);
        }
    }

    free(tempNode);
}

//funzione TopK
void topK_ranking(Heap *maxHeap){
    //ordinamento crescente della classifica per testing
    #ifndef EVAL
        SortAndPrint(maxHeap);
    #endif

    //stampa classifica
    if(maxHeap->count != 0){
        for(int i=0; i<maxHeap->count; i++){
            if(i == maxHeap->count -1){
                printf("%d", maxHeap->array[i].index);
            }else{
                printf("%d ", maxHeap->array[i].index);
            }
        }
    }
    printf("\n");
}

int main(int argc, char const *argv[]) {
    int d=0, k=0, indexGraph;
    char *dk_row = malloc(DK_LEN * sizeof(char));
    char *command_row = malloc(COMMAND_LEN * sizeof(char));
    Heap *maxHeapTopK = NULL;

    //lettura d, k
    if(fgets(dk_row,DK_LEN,stdin) != NULL){
        dk_row=strtok(dk_row," ");
        d = (int)strtol(dk_row,NULL,10);
        while(dk_row != NULL){
            k = (int)strtol(dk_row,NULL,10);
            dk_row=strtok(NULL," ");
        }
    }

    free(dk_row);

    maxHeapTopK = initHeap(k);

    //lettura comandi
    indexGraph=0;
    while(fgets(command_row,COMMAND_LEN,stdin)!=NULL && command_row[0] != '\n'){

        if(strncmp(command_row, "AggiungiGrafo",13) == 0){
            addGraph(indexGraph,d,maxHeapTopK);
            //printMaxHeap(maxHeapTopK);
            indexGraph++;
        }
        if(strncmp(command_row, "TopK",4) == 0){
            topK_ranking(maxHeapTopK);
        }
    }

    free(command_row);
    free(maxHeapTopK->array);
    free(maxHeapTopK);

    return 0;
}
