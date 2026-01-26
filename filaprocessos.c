#include "filaprocessos.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_CICLOS_POR_VEZ 100

struct Processo {
  int id;
  float tempo_chegada;
  int prioridade;
  int ciclos_restantes;
  struct Processo* proximo;
};

struct FilaProcessos {
  Processo* inicio;
  int tamanho;
};

FilaProcessos* criarFila() {
  FilaProcessos* fila = (FilaProcessos*)malloc(sizeof(FilaProcessos));
  if (fila == NULL) {
    return NULL;
  }

  fila->inicio = NULL;
  fila->tamanho = 0;
  return fila;
}

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

void destroiFila(FilaProcessos* fila) {
  if (fila == NULL) {
    return;
  }

  Processo* atual = fila->inicio;
  Processo* proximo;

  // Libera cada processo da lista encadeada
  while (atual != NULL) {
    proximo = atual->proximo;
    free(atual);
    atual = proximo;
  }

  free(fila);
}

// compara os dois processos para decidir qual vem primeiro na ordenacao inicial
// retorna -1 se 'a' vem antes, 1 se 'b' vem antes, 0 se sao iguais
int compararProcessosInicial(Processo* a, Processo* b) {
  // prioridade menor vem primeiro (2 antes de 4)
  if (a->prioridade < b->prioridade) {
    return -1;
  } else if (a->prioridade > b->prioridade) {
    return 1;
  }

  // segundo criterio, quem chegou primeiro vem antes
  if (a->tempo_chegada < b->tempo_chegada) {
    return -1;
  } else if (a->tempo_chegada > b->tempo_chegada) {
    return 1;
  }

  return 0;
}

// divide a lista ao meio e retorna o inicio da segunda metade
// um anda 1 posicao, outro anda 2 (dois ponteiros)
Processo* dividirLista(Processo* inicio) {
  if (inicio == NULL || inicio->proximo == NULL) {
    return inicio;
  }

  Processo* lento = inicio;
  Processo* rapido = inicio->proximo;

  // ponteiro rapido chega no fim, o lento vai ta no meio
  while (rapido != NULL && rapido->proximo != NULL) {
    lento = lento->proximo;
    rapido = rapido->proximo->proximo;
  }

  // corta a lista no meio
  Processo* meio = lento->proximo;
  lento->proximo = NULL;

  return meio;
}

// junta duas listas ja ordenadas em uma so
Processo* mesclar(Processo* esquerda, Processo* direita) {
  if (esquerda == NULL) {
    return direita;
  }
  if (direita == NULL) {
    return esquerda;
  }

  Processo* resultado = NULL;

  // compara os primeiros elementos e escolhe o menor
  // depois chama recursivamente para o resto da lista
  if (compararProcessosInicial(esquerda, direita) <= 0) {
    resultado = esquerda;
    resultado->proximo = mesclar(esquerda->proximo, direita);
  } else {
    resultado = direita;
    resultado->proximo = mesclar(esquerda, direita->proximo);
  }

  return resultado;
}

// MergeSort: divide a lista ao meio, ordena cada parte, e depois junta tudo
Processo* mergeSort(Processo* inicio) {
  // lista com 0 ou 1 elemento ja esta ordenada
  if (inicio == NULL || inicio->proximo == NULL) {
    return inicio;
  }
  Processo* meio = dividirLista(inicio);
  // ordena pela metada recursivamente
  Processo* esquerda = mergeSort(inicio);
  Processo* direita = mergeSort(meio);

  // junta
  return mesclar(esquerda, direita);
}

// aq ordenacao inicial que considera prioridade e tempo de chegada
void adicionaProcessosIniciais(FilaProcessos* fila, Processo** processos,
                               int n) {
  if (fila == NULL || processos == NULL || n <= 0) {
    return;
  }

  // liga todos os processos em uma lista encadeada
  for (int i = 0; i < n - 1; i++) {
    processos[i]->proximo = processos[i + 1];
  }
  processos[n - 1]->proximo = NULL;

  // ordena a lista usando MergeSort e guarda na fila
  fila->inicio = mergeSort(processos[0]);
  fila->tamanho = n;
}

// coloca dnv um processo na fila usando o Insertion Sort (aqui so considera
// prioridade (ignora tempo))
void reinserirProcesso(FilaProcessos* fila, Processo* processo) {
  if (fila == NULL || processo == NULL) {
    return;
  }

  processo->proximo = NULL;

  // ae fila vazia ou processo tem prioridade menor que o primeiro, insere no
  // inicio
  if (fila->inicio == NULL || processo->prioridade < fila->inicio->prioridade) {
    processo->proximo = fila->inicio;
    fila->inicio = processo;
    fila->tamanho++;
    return;
  }

  // percorre a fila ate achar onde inserir e
  // coloca antes do primeiro processo com prioridade maior
  Processo* atual = fila->inicio;

  while (atual->proximo != NULL) {
    if (processo->prioridade < atual->proximo->prioridade) {
      break;
    }
    atual = atual->proximo;
  }

  // coloca o processo na posicao correta
  processo->proximo = atual->proximo;
  atual->proximo = processo;
  fila->tamanho++;
}

// aq simula o uso da CPU processando tudo que ta na fila
void escalonador(FilaProcessos* fila) {
  if (fila == NULL) {
    return;
  }

  while (fila->inicio != NULL) {
    // remove o primeiro processo da fila
    Processo* processo = fila->inicio;
    fila->inicio = processo->proximo;
    fila->tamanho--;

    // quantas vezes vai executar
    int ciclos_executar;
    if (processo->ciclos_restantes > MAX_CICLOS_POR_VEZ) {
      ciclos_executar = MAX_CICLOS_POR_VEZ;
    } else {
      ciclos_executar = processo->ciclos_restantes;
    }

    // simula uso da CPU com loop vazio
    for (int i = 0; i < ciclos_executar; i++) {
      // consome tempo de processamento
    }

    // output: ID, prioridade atual, quantos ciclos executou
    printf("%d %d %d\n", processo->id, processo->prioridade, ciclos_executar);

    processo->ciclos_restantes -= ciclos_executar;

    // aumenta a prioridade e coloca de volta na fila se ainda faltam ciclos
    if (processo->ciclos_restantes > 0) {
      processo->prioridade++;
      reinserirProcesso(fila, processo);
    } else {
      free(processo);
    }
  }
}