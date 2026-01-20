#include <stdio.h>
#include <stdlib.h>

#include "filaprocessos.h"

int main() {
  int n;
  scanf("%d", &n);

  // Cria a fila de processos
  FilaProcessos* fila = criarFila();
  if (fila == NULL) {
    fprintf(stderr, "Erro ao criar fila\n");
    return 1;
  }

  // Aloca array temporario para armazenar processos antes da ordenacao
  Processo** processos = (Processo**)malloc(n * sizeof(Processo*));
  if (processos == NULL) {
    fprintf(stderr, "Erro ao alocar memoria\n");
    destroiFila(fila);
    return 1;
  }

  // Le todos os processos
  for (int i = 0; i < n; i++) {
    int id, prioridade, ciclos;
    float tempo;
    scanf("%d %f %d %d", &id, &tempo, &prioridade, &ciclos);

    // Cria o processo
    processos[i] = criarProcesso(id, tempo, prioridade, ciclos);
    if (processos[i] == NULL) {
      fprintf(stderr, "Erro ao criar processo\n");
      // Libera processos ja criados
      for (int j = 0; j < i; j++) {
        free(processos[j]);
      }
      free(processos);
      destroiFila(fila);
      return 1;
    }
  }

  // ORDENACAO INICIAL COM MERGESORT MODIFICADO (BONUS +1.0 ponto!)
  // Considera prioridade e tempo de chegada
  adicionaProcessosIniciais(fila, processos, n);

  // Libera array temporario (os processos estao agora na fila)
  free(processos);

  // Imprime cabecalho do log
  printf("Log CPU:\n");

  // Executa o escalonador
  // Reinsercoes usam Insertion Sort (considera apenas prioridade)
  escalonador(fila);

  // Libera memoria da fila
  destroiFila(fila);

  return 0;
}