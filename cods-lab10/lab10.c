/* Disciplina: Computacao Concorrente */
/* Aluno: Douglas Quintanilha */
/* Laboratório: 10 */
/* Codigo: Comunicação entre threads usando variável compartilhada e sincronização com semáforos */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS  4

sem_t em1,em2;


void* A (void* arg){
	printf("olá,tudo bem?\n");
	sem_wait(&em1);
	sem_post(&em1);
	sem_post(&em2);
	pthread_exit(NULL);
}

void* B (void* arg){
	printf("hello!\n");
	sem_wait(&em1);
	sem_post(&em1);
	sem_post(&em2);
	pthread_exit(NULL);
}

void* C (void* arg){
	sem_wait(&em2);
	printf("até mais tarde.\n");
	sem_post(&em2);
	pthread_exit(NULL);
}

void* D (void* arg){
	sem_wait(&em2);
	printf("tchau!\n");
	sem_post(&em2);
	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
	pthread_t threads[NTHREADS]; 
	int t;
	sem_init(&em1, 0, 1);
	sem_init(&em2, 0, 0);


    /* Cria as threads */
    pthread_create(&threads[0], NULL, A, NULL);
    pthread_create(&threads[1], NULL, B, NULL);

    pthread_create(&threads[2], NULL, C, NULL);
    pthread_create(&threads[3], NULL, D, NULL);


	//--espera todas as threads terminarem
	for (t=0; t<NTHREADS; t++) {
		if (pthread_join(threads[t], NULL)) {
		    printf("--ERRO: pthread_join() \n"); exit(-1); 
		} 
	} 
	pthread_exit(NULL);
	return 0;
}
