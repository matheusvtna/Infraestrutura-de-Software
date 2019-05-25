#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NumThreads 100
#define MAX 1000000

//Variável compartilhada
int contador = 0;

//Mutex para o contador
pthread_mutex_t mutex_cont = PTHREAD_MUTEX_INITIALIZER;

void *func(void *threadid){

    //ID da thread    
    int tid = *((int*)threadid);

    //Indica se o contador chegou ao objetivo
    int flag = 0;

    while(!flag){
        //Travamos a região crítica
        pthread_mutex_lock(&mutex_cont);
        
        //Verifica se o contador ainda não atingiu a marca
        if(contador < MAX){
            contador++;
            
            //Se ao incrementar, o contador ficar igual a 1, essa thread deve ser a "vencedora" e setamos a flag para 1
            if( contador == MAX ){
                printf("Oi, sou a thread %d. Cheguei em 1m primeiro ;D\n", tid);
                flag = 1;
            }

        }
        //Se outra thread atingiu a marca de 1000000
        else{
            flag = 1;
        }

        //Liberamos a região crítica
        pthread_mutex_unlock(&mutex_cont);
    }

}

int main (){
    //Auxiliares
    int i;
    int rc;  
    
    //Inicialização das threads 
    pthread_t threads[NumThreads];
    int *ids[NumThreads];

    //Criação das threads
    for(i = 0 ; i < NumThreads ; i++){
        ids[i] = (int*) malloc(sizeof(int)); 
        *ids[i] = i;
        rc = pthread_create(&threads[i], NULL, func, (void *) ids[i]);
    }

    //União das threads
    for( i = 0 ; i < NumThreads ; i++){
        pthread_join(threads[i], NULL);    
    }

    //Libera o vetor de ID's
    for( i = 0 ;  i < NumThreads ; i++){
        free(ids[i]);
    }

    //Verifica o valor final do contador
    printf("Valor final do contador: %d\n", contador);
    pthread_exit(NULL);
}
