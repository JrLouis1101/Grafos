#include <stdio.h>
#include <stdlib.h>

struct aresta{
	int vertice;
	int peso;
	struct aresta *prox;
};
typedef struct aresta Aresta;

struct grafo{
	int num_vertices;
	int num_arestas;
	Aresta **arestas;
};
typedef struct grafo Grafo;

int tamanho = 0;

Grafo* cria_grafo(int);
void insere_aresta(Grafo*, int, int, int);
Aresta* nova_aresta(int, int, Aresta*);
int **dijkstra(Grafo*, int);
void inicia_vetores(Grafo*, int*, int*, int);
int verifica_verticeAberto(Grafo*, int*);
int menor_distancia(Grafo*, int*, int*);
void relaxa_vertice(Grafo*, int*, int*, int, int);
int calculaCost(int*, int*);
void printCost(int*, int*, int, int);
int get_precedentes(int*, int*, int, int);

int main(int argc, char const *argv[])
{
	Grafo* grafo = cria_grafo(6);

	insere_aresta(grafo, 0, 1, 10);
    insere_aresta(grafo, 0, 2, 5);
    insere_aresta(grafo, 1, 3, 1);
    insere_aresta(grafo, 2, 1, 3);
    insere_aresta(grafo, 2, 3, 8);
    insere_aresta(grafo, 2, 4, 2);
    insere_aresta(grafo, 3, 4, 4);
    insere_aresta(grafo, 3, 5, 4);
    insere_aresta(grafo, 4, 5, 6);

    int vertice_inicial = 0;
    int vertice_final   = 3;

    int **resultados = dijkstra(grafo, vertice_inicial);

    printCost(resultados[0], resultados[1], vertice_inicial, vertice_final);

    for(int i = 0; i < 2; i++){
    	free(resultados[i]);
    }
    free(resultados);
	return 0;
}

Grafo* cria_grafo(int total_vertices){
	int i;
	Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
	grafo->num_vertices = total_vertices;

	grafo->arestas = (Aresta**)malloc(grafo->num_vertices * sizeof(Aresta*));
	for(i = 0; i < grafo->num_vertices; i++){
		grafo->arestas[i] = NULL;
	}
	return grafo;
}

void insere_aresta(Grafo* grafo, int verticeA, int verticeB, int peso){
	Aresta *aux = grafo->arestas[verticeA];
	for(aux; aux != NULL; aux = aux->prox){
		if(aux->vertice == verticeB) return;
	}

	grafo->arestas[verticeA] = nova_aresta(verticeB, peso, grafo->arestas[verticeA]);
	grafo->num_arestas ++;
}

Aresta* nova_aresta(int vertice, int peso, Aresta* aresta){
	Aresta *aresta_nova = (Aresta*)malloc(sizeof(Aresta));
	aresta_nova->vertice = vertice;
	aresta_nova->peso = peso;
	aresta_nova->prox = aresta;

	return aresta_nova;
}

int** dijkstra(Grafo* grafo, int vertice_inicial){
	int *distancias       = (int*)malloc(sizeof(int) * grafo->num_vertices);
	int *precedentes      = (int*)malloc(sizeof(int) * grafo->num_vertices);
	int *vertices_abertos = (int*)malloc(sizeof(int) * grafo->num_vertices);
	int i;

	inicia_vetores(grafo, distancias, precedentes, vertice_inicial);
	
	for(i = 0; i < grafo->num_vertices; i++)
		vertices_abertos[i] = 1;

	while(verifica_verticeAberto(grafo, vertices_abertos) == 1){
		int u = menor_distancia(grafo, distancias, vertices_abertos);
		vertices_abertos[u] = 0;

		Aresta *aux = grafo->arestas[u];
		for(aux; aux != NULL; aux = aux->prox){
			relaxa_vertice(grafo, distancias, precedentes, u, aux->vertice);
		}
	}
	free(vertices_abertos);
	int ** resultados = (int**)malloc(2 * sizeof(int*));

	resultados[0] = distancias;
	resultados[1] = precedentes;
	//int a = 0;
	for(i = 0; i < grafo->num_vertices; i++){
		printf("%d ", precedentes[i]);
	}
	printf("\n");

	return resultados;
}

void inicia_vetores(Grafo *grafo, int* distancias, int* precedentes, int vertice_inicial){
	int i;
	for(i = 0; i < grafo->num_vertices; i++){
		distancias[i]  = 99999;
		precedentes[i] = -1;
	}

	distancias[vertice_inicial] = 0;
}

int verifica_verticeAberto(Grafo *grafo, int *vertices_abertos){
	int i;
	for(i = 0; i < grafo->num_vertices; i++){
		if(vertices_abertos[i] == 1)
			return 1;
	}
	return 0;
}

int menor_distancia(Grafo *grafo, int *distancias, int *vertices_abertos){
	int menor = 99999;
	int posicao = 0;
	int i;
	for(i = 0; i < grafo->num_vertices; i++){
		if(vertices_abertos[i] == 1){
			if(distancias[i] < menor){
				menor = distancias[i];
				posicao = i;
			}
		}
	}
	return posicao;
}

void relaxa_vertice(Grafo *grafo, int *distancias, int *precedentes, int u, int vertice){
	Aresta *aux = grafo->arestas[u];
	for(aux; aux != NULL && aux->vertice != vertice; aux = aux->prox);

	if(aux){
		if(distancias[vertice] > distancias[u] + aux->peso){
			distancias[vertice] = distancias[u] + aux->peso;
			precedentes[vertice] = u;
		}
	}
}

void printCost(int* distancias, int* precedentes, int vertice_inicial, int vertice_final){
	int *resultado = (int*)malloc(10 * sizeof(int));
	tamanho = 1;

	resultado[0] = vertice_final;
	get_precedentes(precedentes, resultado, vertice_inicial, vertice_final);

	printf("verto resultado com vertices -> ");
	for(int i = tamanho; i >= 0; i--){
		printf("%d ", resultado[i]);
	}
	printf("\n");

	printf("Custo: ");
	printf("%d\n", calculaCost(distancias, resultado));

	free(resultado);
}

int calculaCost(int *distancias, int *resultados){
	int custo = 0;
	int i;
	for(i = 0; i < tamanho; i++){
		custo += distancias[resultados[i]];
	}

	return custo;
}

int get_precedentes(int* precedentes, int* resultado, int vertice_inicial, int vertice_final){
	resultado[tamanho] = precedentes[vertice_final];

	if(resultado[tamanho] == vertice_inicial)
		return 1;

	tamanho ++;

	get_precedentes(precedentes, resultado, vertice_inicial, precedentes[vertice_final]);

	return 0;
}