/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Aluno: Douglas Quintanilha Barbosa Ferreira */
/* DRE: 112012763 */
/* Laboratório: 8 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NTHREADS  2
#define BUFFER_SIZE  5
#define ITERACOES 25


/* Variaveis globais */
pthread_mutex_t mutex;
pthread_cond_t x_cond_produtor;
pthread_cond_t x_cond_consumidor;

long buffer[BUFFER_SIZE];
int in,out,count;

void imprime_buffer(long* buffer){
    int i;
    for (i = 0; i < BUFFER_SIZE; ++i)
    {
        printf("BUFFER: [%li]\n", buffer[i]);
    }
    printf("\n");
    return;
}

int ehPrimo(long unsigned int n) {
    int i;
    if(n<=1) 
        return 0;
    if(n==2)
        return 1;
    if(n%2==0)
        return 0;
    for(i=3; i< sqrt(n)+1; i+=2) {
        if(n%i==0) 
            return 0;
    }
    return 1;
}

long fibonacci(int n)
{   
   if ( n == 0 )
      return 0;
   else if ( n == 1 )
      return 1;
   else
      return ( fibonacci(n-1) + fibonacci(n-2) );
} 

void insere (long n){
    pthread_mutex_lock(&mutex);
    while(count == BUFFER_SIZE){
        pthread_cond_wait(&x_cond_produtor,&mutex);
    }   
    buffer[in] = n;
    in = (in +1) % BUFFER_SIZE;
    count++;
    pthread_cond_signal(&x_cond_consumidor);
    pthread_mutex_unlock(&mutex);
}

long remover(){
    long n;
    pthread_mutex_lock(&mutex);
    while(count == 0){
        pthread_cond_wait(&x_cond_consumidor,&mutex);
    }   
    n = buffer[out];
    buffer[out] = 0;
    out = (out +1) % BUFFER_SIZE;
    count--;
    pthread_cond_signal(&x_cond_produtor);
    pthread_mutex_unlock(&mutex);
    return n;
}

void* produtor(void * arg){
    long fib=0;
    int n=0;
    for (n = 0; n < ITERACOES ; ++n)
    {
        fib = fibonacci(n);
        insere(fib);
        imprime_buffer(buffer);
    }

    pthread_exit(NULL);

}

void* consumidor(void * arg){
    long item,i;
    for ( i = 0; i < ITERACOES; ++i)
    {
        ehPrimo(item);
        item = remover();
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    int i;
    pthread_t threads[NTHREADS];

    pthread_create(&threads[0],NULL, produtor,NULL);
    pthread_create(&threads[1],NULL, consumidor,NULL);

    for (i = 0; i < NTHREADS; ++i)
    {
        pthread_join(threads[i],NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&x_cond_produtor);
    pthread_cond_destroy(&x_cond_consumidor);

    return 0;
}