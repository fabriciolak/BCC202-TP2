#ifndef FILAPROCESSOS_H
#define FILAPROCESSOS_H

// Tipo opaco - esconde a implementacao
typedef struct FilaProcessos FilaProcessos;
typedef struct Processo Processo;

// Funcoes principais do TAD
FilaProcessos* criarFila();
void destroiFila(FilaProcessos* fila);

// Funcao para criar um processo individual
Processo* criarProcesso(int id, float tempo, int prioridade, int ciclos);

// Funcao para adicionar processos iniciais usando MergeSort (BONUS!)
void adicionaProcessosIniciais(FilaProcessos* fila, Processo** processos,
                               int n);

// Funcao para reinserir processo usando Insertion Sort
void reinserirProcesso(FilaProcessos* fila, Processo* processo);

// Funcao do escalonador
void escalonador(FilaProcessos* fila);

#endif