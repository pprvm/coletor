#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int count;
    void* endereco;
} Referencia;

Referencia* referencias = NULL;
int numReferencias = 0;

void* malloc2(size_t tamanho) {
    void* endereco = malloc(tamanho);
    
    if (endereco != NULL) {
        // Adiciona a refer�ncia � lista de refer�ncias
        referencias = realloc(referencias, (numReferencias + 1) * sizeof(Referencia));
        referencias[numReferencias].count = 1;
        referencias[numReferencias].endereco = endereco;
        numReferencias++;
    }
    
    return endereco;
}

void atrib2(void** destino, void* origem) {
    // Procura o endere�o de destino na lista de refer�ncias
    for (int i = 0; i < numReferencias; i++) {
        if (referencias[i].endereco == *destino) {
            // Decrementa o contador de refer�ncias do destino atual
            referencias[i].count--;
            
            if (referencias[i].count == 0) {
                // Se o contador chegar a zero, libera a mem�ria
                free(referencias[i].endereco);
                
                // Remove a refer�ncia da lista
                for (int j = i; j < numReferencias - 1; j++) {
                    referencias[j] = referencias[j + 1];
                }
                
                numReferencias--;
                referencias = realloc(referencias, numReferencias * sizeof(Referencia));
            }
            
            break;
        }
    }
    
    // Atualiza o endere�o de destino com o novo valor
    *destino = origem;
    
    if (origem != NULL) {
        // Procura o novo endere�o na lista de refer�ncias
        int encontrado = 0;
        
        for (int i = 0; i < numReferencias; i++) {
            if (referencias[i].endereco == origem) {
                // Incrementa o contador de refer�ncias do novo endere�o
                referencias[i].count++;
                encontrado = 1;
                break;
            }
        }
        
        if (!encontrado) {
            // Adiciona o novo endere�o � lista de refer�ncias
            referencias = realloc(referencias, (numReferencias + 1) * sizeof(Referencia));
            referencias[numReferencias].count = 1;
            referencias[numReferencias].endereco = origem;
            numReferencias++;
        }
    }
}

int main() {
    int* ptr1 = malloc2(sizeof(int));
    *ptr1 = 10;
    
    int* ptr2 = malloc2(sizeof(int));
    *ptr2 = 20;
    
    int* ptr3 = ptr1;
    
    printf("*ptr1 = %d\n", *ptr1);
    printf("*ptr2 = %d\n", *ptr2);
    printf("*ptr3 = %d\n", *ptr3);
    
    atrib2((void**)&ptr3, ptr2);
    
    printf("*ptr1 = %d\n", *ptr1);
    printf("*ptr2 = %d\n", *ptr2);
    printf("*ptr3 = %d\n", *ptr3);
    
    free(ptr1);
    free(ptr2);
    
    return 0;
}
