// É como se fosse o reverso do que foi pedido na atividade 1.
// Agora temos que criar uma nova variavel de condição pra sinalizar uma "fila" de escritores que querem escrever, e só quando esta 
// fila for zerado, é que um leitor pode ler. É um starvation dos leitores agora. (Antes era dos escritores).

/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Aluno: Douglas Quintanilha Barbosa Ferreira */
/* DRE: 112012763 */
/* Laboratório: 9 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N 2 // LEITORES 
#define M 2 // ESCRITORES

pthread_mutex_t mutex;
pthread_cond_t cond_leitor;
pthread_cond_t cond_escritor;

int escritores = 0;
int leitores = 0;
int querEscrever = 0;

typedef struct a
{
	int contador;
	int idThread;
}Arquivo;

Arquivo arquivo;

void inicializa_leitor(){
	pthread_mutex_lock(&mutex);
	while(escritores > 0 || querEscrever > 0){
		pthread_cond_wait(&cond_leitor, &mutex);
	}
	leitores++;
	pthread_mutex_unlock(&mutex);
}

void finaliza_leitor(){
	pthread_mutex_lock(&mutex);
	leitores --;
	if(leitores == 0 || querEscrever > 0)
		pthread_cond_signal(&cond_escritor);
	pthread_mutex_unlock(&mutex);
}
void inicializa_escritor(){
	pthread_mutex_lock(&mutex);
	querEscrever ++;
	while(escritores > 0){
		pthread_cond_wait(&cond_escritor,&mutex);
	}
	escritores ++;
	pthread_mutex_unlock(&mutex);
}

void finaliza_escritor(){
	pthread_mutex_lock(&mutex);
	escritores --;
	querEscrever --;
	pthread_cond_signal(&cond_escritor);
	if(querEscrever == 0){
		pthread_cond_broadcast(&cond_leitor);
	}
	pthread_mutex_unlock(&mutex);
}

void * leitor(void * arg){
	int* p = (int *) arg;
	int pid =  (int) *p;
	Arquivo arquivoLocal;
	
	while(1){
		printf("Thread Leitora[%d]\n", pid);
		inicializa_leitor();	
		//Lê
		arquivoLocal.contador = arquivo.contador;
		arquivoLocal.idThread = arquivo.idThread;
		printf("Leu o contador: [%d] que foi gravado pela Thread: [%d] \n",arquivoLocal.contador,arquivoLocal.idThread );
		finaliza_leitor();
		sleep(1);
	}
	pthread_exit(NULL);
}

void* escritor(void *arg){
	int * p = (int *) arg;
	int pid = (int) *p;
	while(1){
		printf("Thread Escritora[%d]\n", pid);
		inicializa_escritor();
		// Escreve
		pthread_mutex_lock(&mutex);
		arquivo.contador ++;
		arquivo.idThread = pid;
		pthread_mutex_unlock(&mutex);

		finaliza_escritor();
		sleep(1);
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	int i,j;
	int* pid;
    pthread_t threads[N+M];
	
	// Inicializa Threads Escritoras
    for ( i = 0; i < M; ++i)
    {
    	pid = (int*) malloc(sizeof(int));
    	*pid = i;
		pthread_create(&threads[i],NULL, escritor,(void *) pid);
    }

    // Inicializa Threads Leitoras
    for ( j = M; j < N + M; ++j)
    {
    	pid = (int*) malloc(sizeof(int));
    	*pid = j;
    	pthread_create(&threads[j],NULL, leitor,(void *) pid);
    }


    for (i = 0; i < N + M; ++i)
    {
        pthread_join(threads[i],NULL);
    }

	pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_leitor);
	pthread_cond_destroy(&cond_escritor);
	free(pid);

   return 0;
}

