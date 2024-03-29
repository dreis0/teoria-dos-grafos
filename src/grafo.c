#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

struct grafo
{
    int v; //vértices
    int m; //arestas
    int **adjacency;
};

/**************** Funções Auxiliares ****************/

void DFSRec(grafo_t *G, int s);

int existe_nao_visitado(int *visitados, int num_vertices);

/****************************************************/

/* cria_grafo:
 *    - recebe número n de vértices e m de arestas
 *    - devolve um novo grafo vazio (aloca memória necessária)
 */
grafo_t *cria_grafo(int n /*vértices*/, int m /*arestas*/)
{
    grafo_t *g = (grafo_t *)malloc(sizeof(grafo_t));

    g->v = n;
    g->m = m;
    g->adjacency = (int **)malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++)
        g->adjacency[i] = (int *)malloc(n * sizeof(int));

    return g;
}

/* adiciona_aresta:
 *    - recebe um grafo G, e dois vértices u e v
 *    - adiciona a aresta uv no grafo 
 */
void adiciona_aresta(grafo_t *G, int u, int v)
{
    G->adjacency[u][v] = 1;
    G->adjacency[v][u] = 1;
}

/* imprime_grafo:
 *    - recebe um grafo G
 *    - imprime a vizinhança de cada vértice de G no formato:
 *          v: a b c d e
 */
void imprime_grafo(grafo_t *G)
{
    printf("Matriz de Adjacências de G: \n");
    for (int i = 0; i < G->v; i++)
    {
        for (int j = 0; j < G->v; j++)
        {
            printf("%d  ", G->adjacency[i][j]);
        }
        printf("\n");
    }

    printf("Vizinhança dos Vértices: \n");
    for (int i = 0; i < G->v; i++)
    {
        printf("%d: ", i);
        for (int j = 0; j < G->v; j++)
        {
            if (G->adjacency[i][j] == 1)
            {
                printf("%d ", j);
            }
        }
        printf("\n");
    }
}

void remove_aresta(grafo_t *G, int u, int v)
{
    G->adjacency[u][v] = 0;
    G->adjacency[v][u] = 0;
}

/* deleta_grafo:
 *    - recebe um grafo G
 *    - desaloca memória utilizada por G 
 */
void deleta_grafo(grafo_t *G)
{
    free(G);
}

/* grau:
 *    - recebe um grafo G e um vértice v
 *    - retorna o grau de v
 */
int grau(grafo_t *G, int v)
{
    int grau = 0;

    for (int i = 0; i < G->v; i++)
    {
        if (G->adjacency[v][i] == 1)
        {
            grau++;
        }
    }

    return grau;
}

/* grau_maximo:
 *    - recebe um grafo G
 *    - retorna o grau máximo de G
 */
int grau_maximo(grafo_t *G)
{
    int grau_max = 0;
    int aux_grau;

    for (int i = 0; i < G->v; i++)
    {
        aux_grau = 0;
        for (int j = 0; j < G->v; j++)
        {
            if (G->adjacency[i][j] == 1)
            {
                aux_grau++;
            }
        }

        if (aux_grau > grau_max)
        {
            grau_max = aux_grau;
        }
    }

    return grau_max;
}

int *array_visitados;

/* DFS:
 *    - recebe um grafo G e um vértice s
 *    - aplica a busca em largura em G a partir de s
 *    - retorna um vetor indexado por vértices que indica se um vértice foi visitado ou não
 */
int *DFS(grafo_t *G, int s)
{
    array_visitados = malloc(G->v * sizeof(int));

    for (int i = 0; i < G->v; i++)
        array_visitados[i] = 0;

    DFSRec(G, s);

    return array_visitados;
}

void DFSRec(grafo_t *G, int s)
{
    array_visitados[s] = 1;

    for (int i = 0; i < G->v; i++)
    {
        if (G->adjacency[s][i] == 1 && array_visitados[i] == 0)
            DFSRec(G, i);
    }
}

/* eh_aresta_corte:
 *    - recebe um grafo G e dois vértices u e v
 *    - retorna 0 se a aresta uv não é de corte
 *    - retorna 1 caso contrário
 *    (utilize a função DFS)
 */
int eh_aresta_corte(grafo_t *G, int u, int v)
{
    int *visitados;
    int nao_visitados = 0;
    int nao_visitados_sem_uv = 0;

    visitados = DFS(G, u);

    for (int i = 0; i < G->v; i++)
        if (visitados[i] == 0)
            nao_visitados++;

    remove_aresta(G, u, v);

    visitados = DFS(G, u);
    for (int i = 0; i < G->v; i++)
        if (visitados[i] == 0)
            nao_visitados_sem_uv++;

    adiciona_aresta(G, u, v);

    return nao_visitados != nao_visitados_sem_uv;
}

/* eh_par:
 *    - recebe um grafo G
 *    - retorna 1 se G e impar
 *    - retorna 0 caso contrario
 */
int eh_par(grafo_t *G)
{
    int aux;

    for (int i = 0; i < G->v; i++)
    {
        aux = 0;
        for (int j = 0; j < G->v; j++)
        {
            aux += G->adjacency[i][j];
        }

        if (aux % 2 != 0)
            return 0;
    }

    return 1;
}

int *fleury_trilha;

/* Fleury:
 *    - recebe um grafo G *PAR* e um vertice v
 *    - retorna um vetor que contem a sequencia de vertices de uma trilha euleriana em G
 *      - em particular, esse vetor contem v na primeira e ultima posicoes
 *    - IMPORTANTE: quando houver dois ou mais vertices possiveis de serem escolhidos, comece pelo de menor indice
 */
int *Fleury(grafo_t *G, int v)
{
    fleury_trilha = malloc((G->m + 1) * sizeof(int));

    for (int i = 0; i <= G->m; i++)
    {
        fleury_trilha[i] = 0;
    }

    int qtd_vizinhos, aux_menor_vertice;
    int *vizinhos = malloc((G->v - 1) * sizeof(int));

    for (int i = 0; i < G->m; i++) //Executa uma vez por aresta
    {
        fleury_trilha[i] = v;
        qtd_vizinhos = 0;

        for (int j = 0; j < G->v; j++) //Encontra os vizinhos de v
        {
            if (G->adjacency[v][j])
            {
                vizinhos[qtd_vizinhos] = j;
                qtd_vizinhos++;
            }
        }

        aux_menor_vertice = G->v + 1;

        for (int j = 0; j < qtd_vizinhos; j++) //Encontra o menor vizinho que não forma aresta de corte
        {
            if (!eh_aresta_corte(G, v, vizinhos[j]) && vizinhos[j] < aux_menor_vertice)
                aux_menor_vertice = vizinhos[j];
        }

        if (aux_menor_vertice == G->v + 1)
        {
            for (int j = 0; j < qtd_vizinhos; j++) //Encontra o menor vizinho
            {
                if (vizinhos[j] < aux_menor_vertice)
                    aux_menor_vertice = vizinhos[j];
            }
        }

        remove_aresta(G, v, aux_menor_vertice);
        v = aux_menor_vertice;
    }

    return fleury_trilha;
}

/* Dijkstra:
 *    - recebe um grafo G conexo, um vertice inicio e uma matriz que contem os pesos das arestas
 *    - retorna um vetor que contem a distancia entre inicio e todos os outros vertices
 */
int *Dijkstra(grafo_t *G, int inicio, int **peso)
{
    printf("Dijkstra");
    int n = G->v;
    int *visitados = malloc(sizeof(int) * n);
    int *distancias = malloc(sizeof(int) * n);
    int *predecessores = malloc(sizeof(int) * n);

    for (int i = 0; i < G->v; i++)
    {
        distancias[i] = 9999;
        predecessores[i] = -1;
        visitados[i] = 0;
    }

    distancias[inicio] = 0;
    // visitados[inicio] = 1;
    int u = inicio;

    while (existe_nao_visitado(visitados, n))
    {
        int aux = 9999;
        for (int i = 0; i < n; i++)
        {
            if (visitados[i] == 0 && distancias[i] < aux)
            {
                aux = distancias[i];
                u = i;
            }
        }

        visitados[u] = 1;
        for (int i = 0; i < n; i++)
        {
            if (G->adjacency[u][i] == 1)
                if (visitados[i] == 0 && distancias[u] + peso[u][i] < distancias[i])
                {
                    distancias[i] = distancias[u] + peso[u][i];
                    predecessores[i] = u;
                }
        }
    }

    return distancias;
}

int existe_nao_visitado(int *visitados, int num_vertices)
{
    for (int i = 0; i < num_vertices; i++)
    {
        if (visitados[i] == 0)
            return 1;
    }

    return 0;
}
