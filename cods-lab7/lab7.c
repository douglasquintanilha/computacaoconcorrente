/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Aluno: Douglas Quintanilha Barbosa Ferreira */
/* DRE: 112012763 /*
/* Laboratório: 7 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread A */
void *A (void *t) {
    int boba1, boba2;

    /* faz alguma coisa inutil pra gastar tempo... */
    boba1=10000; boba2=-10000; while (boba2 < boba1) boba2++;

    printf("tudo bem?\n");

    pthread_mutex_lock(&x_mutex);
    x++;
    pthread_cond_broadcast(&x_cond);
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

/* Thread B */
void *B (void *t) {
    int boba1, boba2;
    
    /* faz alguma coisa inutil pra gastar tempo... */
    boba1=10000; boba2=-10000; while (boba2 < boba1) boba2++;
    printf("hola!\n");

    pthread_mutex_lock(&x_mutex);
    x++;
    pthread_cond_broadcast(&x_cond);
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

/* Thread C */
void *C (void *t) {

    pthread_mutex_lock(&x_mutex);
    while(x != 2){
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    printf("ate mais tarde\n");
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

/* Thread D */
void *D (void *t) {

    pthread_mutex_lock(&x_mutex);
    while(x != 2){
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    printf("tchau\n");
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}


/* Funcao principal */
int main(int argc, char *argv[]) {
    int i;
    pthread_t threads[NTHREADS];

    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init (&x_cond, NULL);

    /* Cria as threads */
    pthread_create(&threads[2], NULL, C, NULL);
    pthread_create(&threads[3], NULL, D, NULL);
    pthread_create(&threads[0], NULL, A, NULL);
    pthread_create(&threads[1], NULL, B, NULL);

    /* Espera todas as threads completarem */
    for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
    }

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    pthread_exit (NULL);
}
