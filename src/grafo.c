#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

struct grafo
{
    int v; //vértices
    int m; //arestas
    int **adjacency;
};

void DFSRec(grafo_t *G, int s);

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

    return nao_visitados != nao_visitados_sem_uv;
}