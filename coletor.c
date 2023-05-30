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
        // Adiciona a referência à lista de referências
        referencias = realloc(referencias, (numReferencias + 1) * sizeof(Referencia));
        referencias[numReferencias].count = 1;
        referencias[numReferencias].endereco = endereco;
        numReferencias++;
    }
    
    return endereco;
}

void atrib2(void** destino, void* origem) {
    // Procura o endereço de destino na lista de referências
    for (int i = 0; i < numReferencias; i++) {
        if (referencias[i].endereco == *destino) {
            // Decrementa o contador de referências do destino atual
            referencias[i].count--;
            
            if (referencias[i].count == 0) {
                // Se o contador chegar a zero, libera a memória
                free(referencias[i].endereco);
                
                // Remove a referência da lista
                for (int j = i; j < numReferencias - 1; j++) {
                    referencias[j] = referencias[j + 1];
                }
                
                numReferencias--;
                referencias = realloc(referencias, numReferencias * sizeof(Referencia));
            }
            
            break;
        }
    }
    
    // Atualiza o endereço de destino com o novo valor
    *destino = origem;
    
    if (origem != NULL) {
        // Procura o novo endereço na lista de referências
        int encontrado = 0;
        
        for (int i = 0; i < numReferencias; i++) {
            if (referencias[i].endereco == origem) {
                // Incrementa o contador de referências do novo endereço
                referencias[i].count++;
                encontrado = 1;
                break;
            }
        }
        
        if (!encontrado) {
            // Adiciona o novo endereço à lista de referências
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
