/*
*Universidade Federal do Piauí
*Curso: Bacharelado em Ciência da Computação
*Disciplina: Sistemas Operacionais
*Pedro Felipe
*Objetivo:  
    Implemente o problema dos filósofos comilões, utilizando threads para criar os 
    participantes do problema, mutex e variáveis de condição para garantir a exclusão mútua. Será 
    avaliado principalmente se a implementação comprova que há momentos de concorrência(Ex: dois 
    filósofos comendo simultaneamente) mesmo com a implementação de exclusão mútua.
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define FORKS 5 /*numero de garfos*/
#define SLEEP_THINK 10 /*tempo do filosofo pensando em segundos*/
#define SLEEP_EAT 5 /*tempo do filoso comendo em segundos */

pthread_mutex_t fork_mutex[FORKS]; /*um semaforo por garfo*/

void take_fork(int *filo) /*Funcao pega garfo | filo: o numero do filosofo*/
{  
    int left = *(filo); // indice do garfo a esqueda
    int rigth = (*(filo) + 1)  % FORKS; // indice do garfo a direita

    pthread_mutex_lock(&(fork_mutex[left]));  // bloqueia o garfo da esqueda
    pthread_mutex_lock(&(fork_mutex[rigth])); // bloqueia o garfo da direita

    // printf(" esqueda: %d ; direita: %d", left, rigth);
    printf("filosofo %d pegou os garfos\n", *filo);
}

void release_fork(int *filo)/*Funcao devolve garfo | filo: o numero do filosofo*/
{ // solta o garfo
    int left = *(filo); // indice do garfo a esqueda
    int rigth = (*(filo) + 1) % (FORKS); /// indice do garfo a esqueda
    // printf(" esqueda: %d ; direita: %d", left, rigth);
    pthread_mutex_unlock(&(fork_mutex[left]));  // bloqueia o garfo da esqueda
    pthread_mutex_unlock(&(fork_mutex[rigth])); // bloqueia o garfo da direita

    printf("filosofo %d soltou os garfos\n", *filo);
}

void think(int filo)
{
    printf("filosofo %d esta pensando\n", filo);
    sleep(SLEEP_THINK);
}

void eat(int filo)
{
    take_fork(&filo); /*pega dois garfos ou bloqueia */
    printf("filosofo %d esta comendo\n", filo);
    sleep(SLEEP_EAT); /*come*/
    release_fork(&filo); /*devolve os dois garfos a mesa*/
}

void *philosopher_life(void *index) /*index : o numero do filosofo*/
{
    int filo = (long)index;
    while (1)
    {
        think(filo); /*o filosofo esta pensando */
        eat(filo); /*o filosofo esta comendo */
    }
}

int main(int arg, char *args[])
{
    pthread_t philosophers[FORKS];  /*uma thread para cada filósofo*/
    int rc; /*recebe código de error da criação de uma thread*/

    /*inicializa os semáforos de cada filósofo*/
    for (long i = 0; i < FORKS; i++) pthread_mutex_init(&fork_mutex[i], 0); 

    printf("\nDigite 0 para se quiser encerrar o programa...\n\n\n");
    for (long i = 0; i < FORKS; i++)
    {
        /*cria thread de cada filosofo*/
        rc = pthread_create(&philosophers[i], NULL, philosopher_life, (void *)i); 
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            return -1;
        }
    }

    char flag[100];
    while (1)
    { /*repete até que seja digitado 0*/
        fgets(flag, 100, stdin);
        if (strcmp(flag, "0\n") == 0) break;
    };

    /*destrói todos os semáforos*/
    for (int i = 0; i < FORKS; i++) pthread_mutex_destroy(&fork_mutex[i]);

    return 0;
}
