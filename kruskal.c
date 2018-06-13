#include <stdio.h>
#include <stdlib.h>

typedef struct aresta Aresta;
struct aresta
{
	int v1, v2;
	int peso;
	int flag;
};

typedef struct grafo Grafo;
struct grafo
{
	int numVertices;
	int numArestas;
	int *ciclo;
	Aresta* arestas;
};

Grafo* cria_grafo(int, int);
void insere_aresta(Grafo*, int, int, int);
void ordena_peso(Grafo*);
int verifica_pai(int, Grafo*);
void atualiza_pai(int, int, Grafo*);
void kruskal(Grafo*);

int main()
{
	Grafo* g = cria_grafo(5, 7);

	insere_aresta(g,1,2,12);
	insere_aresta(g,1,3,13);
	insere_aresta(g,1,4,10);
	insere_aresta(g,2,4,5);
	insere_aresta(g,3,4,50);
	insere_aresta(g,3,5,23);
	insere_aresta(g,4,5,17);
	
	ordena_peso(g);
	
	kruskal(g);

	return 0;
}

Grafo* cria_grafo(int numVertices, int numArestas){
	int i, j;
	Grafo* grafo = (Grafo*) malloc(sizeof(Grafo));
	grafo->numVertices = numVertices;
	grafo->numArestas = numArestas;
	grafo->ciclo = (int*)malloc(sizeof(int)* (numVertices+1));
	grafo->arestas = (Aresta*) malloc(sizeof(Aresta)* numArestas);
	
	for(j = 0; j < numVertices + 1; j++){
		grafo->ciclo[j] = j;
	}

	for(i = 0; i < numArestas; i++){
		grafo->arestas[i].flag = -1;
	}

	return grafo;
}

void insere_aresta(Grafo* grafo, int v1, int v2, int peso){
	Aresta nova;
	nova.v1 = v1;
	nova.v2 = v2;
	nova.peso = peso;
	int i;

	for(i = 0; i < grafo->numArestas; i++){
		if(grafo->arestas[i].flag == -1){
			grafo->arestas[i] = nova;
			grafo->arestas[i].flag = 0;
			break;
		}
	}
}

void ordena_peso(Grafo* grafo)
{
	int k = grafo->numArestas - 1;
	Aresta aux;

	for(int i = 0; i < grafo->numArestas; i++){
		for(int j =0 ; j < k; j++){
			if(grafo->arestas[j].peso > grafo->arestas[j+1].peso)
			{
				aux = grafo->arestas[j];
				grafo->arestas[j] = grafo->arestas[j+1];
				grafo->arestas[j+1] = aux;
			}
		}
		k--;
	}
}
int verifica_pai(int v, Grafo* grafo){
	if(grafo->ciclo[v] == v){
		return v;
	}
	grafo->ciclo[v] = verifica_pai(grafo->ciclo[v], grafo);
	return grafo->ciclo[v];
}

void atualiza_pai(int vertice1, int vertice2, Grafo* grafo){
	grafo->ciclo[verifica_pai(vertice1, grafo)] = verifica_pai(vertice2, grafo);
}

void kruskal(Grafo* grafo){
	int custo_minimo = 0;
	int i;
	for(i = 0; i < grafo->numArestas; i++){
		if(verifica_pai(grafo->arestas[i].v1, grafo) != verifica_pai(grafo->arestas[i].v2, grafo)){
			printf("Aresta: {%d,%d} || peso: %d\n", grafo->arestas[i].v1, grafo->arestas[i].v2, grafo->arestas[i].peso);
			
			custo_minimo += grafo->arestas[i].peso;
			
			atualiza_pai(grafo->arestas[i].v1, grafo->arestas[i].v2, grafo);		
		}

	}
	printf("Custo Minimo: %d\n", custo_minimo);
}