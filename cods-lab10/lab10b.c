/* Disciplina: Computacao Concorrente */
/* Aluno: Douglas Quintanilha */
/* Laboratório: 10 */
/* Codigo: Comunicação entre threads usando variável compartilhada e sincronização com semáforos */

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

#define PRODUTORES 5
#define CONSUMIDORES 2

sem_t slotCheio,slotVazio;
sem_t mutexProd,mutexCons;

int buffer[BUFFER_SIZE];
int in,out;

void imprime_buffer(int * buffer){
    int i;
    for (i = 0; i < BUFFER_SIZE; ++i)
    {
        printf("BUFFER: [%d]\n", buffer[i]);
    }
    printf("\n");
    return;
}

void insere(int item){
	sem_wait(&slotVazio);
	sem_wait(&mutexProd);
	buffer[in] = item;
	in = (in + 1) % BUFFER_SIZE;
	imprime_buffer(buffer);
	sem_post(&mutexProd);
	sem_post(&slotCheio);
}
int retira(){
	int item;
	sem_wait(&slotCheio);
	sem_wait(&mutexCons);
	item = buffer[out];
	buffer[out] = -1;
	out = (out + 1) % BUFFER_SIZE;
	imprime_buffer(buffer);
	sem_post(&mutexCons);
	sem_post(&slotVazio);
	return item;
}


void* produtor (void* arg){
	int* p = (int*) arg;
	int pid = (int) *p;
	printf("[Thread Produtora %d criada] \n\n", pid);
	while(1){
		printf("Produtor[%d] quer escrever\n\n", pid );
		insere(pid);		
		printf("Produtor[%d] escreveu\n\n", pid );
		sleep(1);
	}
	free(p);
	pthread_exit(NULL);
}

void* consumidor (void* arg){
	int* p = (int*) arg;
	int pid = (int) *p;
	printf("[Thread Consumidora %d criada]\n", pid);
	int item;
	while(1){
		printf("Consumidor[%d] quer ler\n\n", pid );
		item = retira();
		printf("Consumidor [%d] removeu o item: [%d]:\n\n",pid, item);
		sleep(1);
	}
	free(p);
	pthread_exit(NULL);
	
}


int main(int argc, char *argv[]) {
	pthread_t threads[CONSUMIDORES+PRODUTORES]; 
	int i,j;
	int * pid;

	sem_init(&mutexCons,0,1);
	sem_init(&mutexProd,0,1);
	sem_init(&slotCheio,0,0);
	sem_init(&slotVazio,0,BUFFER_SIZE);


    /* Cria as threads */

	// Inicializa Threads Produtoras
    for ( i = 0; i < PRODUTORES; ++i)
    {
    	pid = (int*) malloc(sizeof(int));
    	*pid = i;
		pthread_create(&threads[i],NULL, produtor,(void *) pid);
    }

    // Inicializa Threads Consumidores
    for ( j = 0; j < CONSUMIDORES; ++j)
    {
    	pid = (int*) malloc(sizeof(int));
    	*pid = j;
    	pthread_create(&threads[j+i],NULL, consumidor,(void *) pid);
    }



	// Espera todas as threads terminarem
	for (i=0; i < PRODUTORES + CONSUMIDORES ; i++) {
		if (pthread_join(threads[i], NULL)) {
		    printf("--ERRO: pthread_join() \n"); exit(-1); 
		} 
	} 
	pthread_exit(NULL);
	sem_destroy(&mutexCons);
	sem_destroy(&mutexProd);
	sem_destroy(&slotVazio);
	sem_destroy(&slotCheio);
	return 0;
}
