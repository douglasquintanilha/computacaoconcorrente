/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Aluno: Douglas Quintanilha Barbosa Ferreira */
/* DRE: 112012763 */
/* Laboratório: 7 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  5
#define PASSOS 5

/* Variaveis globais */
int x = 0,contador=0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

void barreira(int nthreads){
    pthread_mutex_lock(&x_mutex);
    contador++;
    //printf("Contador: [%d]\n",contador );
    while(contador != nthreads){
        //printf("Dei Wait\n" );
        pthread_cond_wait(&x_cond,&x_mutex);        
        pthread_mutex_unlock(&x_mutex);
        return;
    }
    
    //printf("Chamei Broadcast\n" );
    contador = 0;
    pthread_cond_broadcast(&x_cond);
    pthread_mutex_unlock(&x_mutex);
    return;
}

void *A (void *arg) {
    int tid = * (int*)arg, i;
    int cont = 0, boba1, boba2;
    
    for (i=0; i < PASSOS; i++) {
        cont++;
        printf("Thread %d: cont=%d, passo=%d\n", tid, cont, i);
        //sincronizacao condicional
        barreira(NTHREADS);
        //printf("    Sai da barreira :[%d]\n",tid );
        /*faz alguma coisa inutil pra gastar tempo...*/
        boba1=1000; boba2=-1000; while (boba2 < boba1) boba2++;
    }
pthread_exit(NULL);
}


/* Funcao principal */
int main(int argc, char *argv[]) {
    int i;
    int * tid;
    pthread_t threads[NTHREADS];

    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init (&x_cond, NULL);

    /* Cria as threads */
    
    for ( i = 0; i < NTHREADS; ++i)
    {
        tid = (int *) malloc(sizeof(int));
        *tid = i;
        pthread_create(&threads[i], NULL, A,(void *) tid);
    }

    /* Espera todas as threads completarem */
    for (i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    pthread_exit (NULL);
}
