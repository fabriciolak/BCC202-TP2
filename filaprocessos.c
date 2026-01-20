#include "filaprocessos.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_CICLOS_POR_VEZ 100

// Estrutura interna do Processo
struct Processo {
  int id;
  float tempo_chegada;
  int prioridade;
  int ciclos_restantes;
  struct Processo* proximo;
};

// Estrutura interna da Fila
struct FilaProcessos {
  Processo* inicio;
  int tamanho;
};

// ========== FUNCOES BASICAS DO TAD ==========

// Cria uma fila vazia
FilaProcessos* criarFila() {
  FilaProcessos* fila = (FilaProcessos*)malloc(sizeof(FilaProcessos));
  if (fila == NULL) {
    return NULL;
  }
  fila->inicio = NULL;
  fila->tamanho = 0;
  return fila;
}

// Cria um processo individual
Processo* criarProcesso(int id, float tempo, int prioridade, int ciclos) {
  Processo* novo = (Processo*)malloc(sizeof(Processo));
  if (novo == NULL) {
    return NULL;
  }

  novo->id = id;
  novo->tempo_chegada = tempo;
  novo->prioridade = prioridade;
  novo->ciclos_restantes = ciclos;
  novo->proximo = NULL;

  return novo;
}

// Destroi a fila e libera memoria
void destroiFila(FilaProcessos* fila) {
  if (fila == NULL) {
    return;
  }

  Processo* atual = fila->inicio;
  Processo* proximo;

  while (atual != NULL) {
    proximo = atual->proximo;
    free(atual);
    atual = proximo;
  }

  free(fila);
}

// ========== MERGESORT MODIFICADO (BONUS +1.0) ==========

// Funcao de comparacao para ordenacao inicial
// Retorna:  -1 se a vem antes, 1 se b vem antes, 0 se iguais
int compararProcessosInicial(Processo* a, Processo* b) {
  // Prioridade menor tem preferencia
  if (a->prioridade < b->prioridade) {
    return -1;
  } else if (a->prioridade > b->prioridade) {
    return 1;
  }

  // Se prioridades iguais, tempo de chegada menor tem preferencia
  if (a->tempo_chegada < b->tempo_chegada) {
    return -1;
  } else if (a->tempo_chegada > b->tempo_chegada) {
    return 1;
  }

  return 0;
}

// Divide a lista ao meio usando tecnica dos dois ponteiros
// Retorna o inicio da segunda metade
Processo* dividirLista(Processo* inicio) {
  if (inicio == NULL || inicio->proximo == NULL) {
    return inicio;
  }

  Processo* lento = inicio;
  Processo* rapido = inicio->proximo;

  // Ponteiro lento avanca 1, rapido avanca 2
  // Quando rapido chegar ao fim, lento estara no meio
  while (rapido != NULL && rapido->proximo != NULL) {
    lento = lento->proximo;
    rapido = rapido->proximo->proximo;
  }

  // Divide a lista
  Processo* meio = lento->proximo;
  lento->proximo = NULL;

  return meio;
}

// Mescla duas listas ordenadas em uma unica lista ordenada
Processo* mesclar(Processo* esquerda, Processo* direita) {
  // Casos base
  if (esquerda == NULL) {
    return direita;
  }
  if (direita == NULL) {
    return esquerda;
  }

  Processo* resultado = NULL;

  // Compara e escolhe qual processo vem primeiro
  if (compararProcessosInicial(esquerda, direita) <= 0) {
    resultado = esquerda;
    resultado->proximo = mesclar(esquerda->proximo, direita);
  } else {
    resultado = direita;
    resultado->proximo = mesclar(esquerda, direita->proximo);
  }

  return resultado;
}

// MergeSort recursivo para lista encadeada
// DIVISAO E CONQUISTA:
// 1. Divide a lista ao meio
// 2. Ordena recursivamente cada metade
// 3. Mescla as duas metades ordenadas
Processo* mergeSort(Processo* inicio) {
  // Caso base:  lista vazia ou com 1 elemento ja esta ordenada
  if (inicio == NULL || inicio->proximo == NULL) {
    return inicio;
  }

  // DIVISAO:  Divide a lista ao meio
  Processo* meio = dividirLista(inicio);

  // CONQUISTA: Ordena recursivamente cada metade
  Processo* esquerda = mergeSort(inicio);
  Processo* direita = mergeSort(meio);

  // COMBINACAO: Mescla as duas metades ordenadas
  return mesclar(esquerda, direita);
}

// Adiciona processos iniciais usando MergeSort (BONUS!)
void adicionaProcessosIniciais(FilaProcessos* fila, Processo** processos,
                               int n) {
  if (fila == NULL || processos == NULL || n <= 0) {
    return;
  }

  // Criar lista encadeada com os processos
  for (int i = 0; i < n - 1; i++) {
    processos[i]->proximo = processos[i + 1];
  }
  processos[n - 1]->proximo = NULL;

  // Ordenar usando MergeSort modificado
  fila->inicio = mergeSort(processos[0]);
  fila->tamanho = n;
}

// ========== INSERTION SORT PARA REINSERCAO ==========

// Reinserir processo usando Insertion Sort (considera apenas prioridade)
void reinserirProcesso(FilaProcessos* fila, Processo* processo) {
  if (fila == NULL || processo == NULL) {
    return;
  }

  processo->proximo = NULL;

  // Caso 1: Fila vazia ou inserir no inicio
  if (fila->inicio == NULL || processo->prioridade < fila->inicio->prioridade) {
    processo->proximo = fila->inicio;
    fila->inicio = processo;
    fila->tamanho++;
    return;
  }

  // Caso 2: Procurar posicao correta na fila
  // Reinsercao considera APENAS prioridade (nao tempo de chegada)
  Processo* atual = fila->inicio;

  while (atual->proximo != NULL) {
    if (processo->prioridade < atual->proximo->prioridade) {
      break;
    }
    atual = atual->proximo;
  }

  // Inserir na posicao encontrada
  processo->proximo = atual->proximo;
  atual->proximo = processo;
  fila->tamanho++;
}

// ========== ESCALONADOR ==========

// Processa a fila de processos
void escalonador(FilaProcessos* fila) {
  if (fila == NULL) {
    return;
  }

  while (fila->inicio != NULL) {
    // Remove o primeiro processo da fila
    Processo* processo = fila->inicio;
    fila->inicio = processo->proximo;
    fila->tamanho--;

    // Determina quantos ciclos vai executar
    int ciclos_executar;
    if (processo->ciclos_restantes > MAX_CICLOS_POR_VEZ) {
      ciclos_executar = MAX_CICLOS_POR_VEZ;
    } else {
      ciclos_executar = processo->ciclos_restantes;
    }

    // Simula execucao na CPU com loops vazios
    for (int i = 0; i < ciclos_executar; i++) {
      // Simula processamento (loop vazio)
    }

    // Imprime log do atendimento
    printf("%d %d %d\n", processo->id, processo->prioridade, ciclos_executar);

    // Atualiza ciclos restantes
    processo->ciclos_restantes -= ciclos_executar;

    // Se ainda faltam ciclos, reinserir na fila com prioridade incrementada
    if (processo->ciclos_restantes > 0) {
      processo->prioridade++;             // Incrementa prioridade
      reinserirProcesso(fila, processo);  // Usa Insertion Sort
    } else {
      // Processo finalizado, libera memoria
      free(processo);
    }
  }
}