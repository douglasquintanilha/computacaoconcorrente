/* Disciplina: Computacao Concorrente */
/* Aluno: Douglas Quintanilha */
/* Laboratório: 6 */
/* Codigo: Comunicação entre threads usando variável compartilhada e exclusao mutua com bloqueio */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"
#include <math.h>

pthread_mutex_t mutex; //variavel de lock para exclusao mutua

long long int N,i_global;
int n_threads;
int * total_primos;


int ehPrimo(long long int n) {
	int i;
	if (n<=1) return 0;
	if (n==2) return 1;
	if (n%2==0) return 0;
	for (i=3; i<sqrt(n)+1; i+=2)
		if(n%i==0) return 0;
	return 1;
}


void * contraPrimosConcorrente(void* t) {
	int* tid;
	tid = (int *) t;
	long long int i_local = 0,n_primos = 0;
  	
  	pthread_mutex_lock(&mutex);
	i_local = i_global;
  	i_global++;
  	pthread_mutex_unlock(&mutex);

  	while(i_local < N){
  		if(ehPrimo(i_local)){
  			n_primos++;
  		}
  		pthread_mutex_lock(&mutex);
  		i_local = i_global;
  		i_global++;
  		pthread_mutex_unlock(&mutex);
  	}
  	total_primos[*tid] = n_primos;
  	pthread_exit(NULL);
}


long long int contaPrimos(){
	long long int i;
	long long n_primos = 0;
	for ( i = 0; i < N; ++i)
	{
		if(ehPrimo(i)) n_primos++;
	}
	return n_primos;
}

long long int somaArray(int* array){
	int i;
	long long int total = 0;
	for (i = 0; i < n_threads; ++i)
	{
		total += total_primos[i];
	}

	return total;
}

void imprimeArray(int * array){
	int i;
	for (i = 0; i < n_threads; ++i)
	{
		printf("Total primos[%d]: [%d]\n",i,array[i] );
	}
}
int main(int argc, char *argv[]) {
	
	pthread_t *tid_sistema;
	int *tid;
	int i;
	long long int total_primos_sequencial;
	double ini, fim, inicioS, fimS;

	if(argc != 3){
		printf("--ERRO: Número de argumentos errados. Uso ./lab5.exe n_primo n_threads\n"); exit(-1);
	}

	N = atoi(argv[1]);
	n_threads = atoi(argv[2]);

  	GET_TIME(ini);

  	//--inicilaiza o mutex (lock de exclusao mutua)
  	pthread_mutex_init(&mutex, NULL);

  	// Aloca espaço para o vetor total de Números Primos
  	total_primos = (int *) malloc(sizeof(int) * n_threads);
    if(total_primos==NULL) {
        printf("--ERRO: malloc()\n"); exit(-1);
    }

  	// Aloca espaço para o vetor de Threads do Sistema.
    tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * n_threads);
    if(tid_sistema==NULL) {
        printf("--ERRO: malloc()\n"); exit(-1);
    }


	for(i=0; i<n_threads; i++) {
		tid = (int *) malloc(sizeof(int));
        if(tid == NULL) {
            printf("--ERRO: malloc()\n");
            exit(-1);
        }
        *tid = i;
		if(pthread_create(&tid_sistema[i], NULL, contraPrimosConcorrente, (void *) tid) ){
			printf("--ERRO: pthread_create()\n"); exit(-1);
		}
	}

	//--espera todas as threads terminarem
    for (i=0; i<n_threads; i++) {
        if (pthread_join(tid_sistema[i], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1);
        }
    }
  	GET_TIME(fim);
	
	pthread_mutex_destroy(&mutex);
	GET_TIME(inicioS);
	total_primos_sequencial = contaPrimos();
	GET_TIME(fimS);
	printf("Versão Sequencial : Total de primos:[%lld]\n", total_primos_sequencial);
	printf("Versão Concorrente: Total de primos:[%lld]\n",somaArray(total_primos));
	printf("Tempo Sequencial = %lf\n", fimS-inicioS);
	printf("Tempo Concorrente = %lf\n", fim-ini);
	free(tid);
	free(tid_sistema);
	free(total_primos);
  	pthread_exit(NULL);
}
