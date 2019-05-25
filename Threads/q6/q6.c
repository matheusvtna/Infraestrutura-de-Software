#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#define NUM_PRODUTORES 15
#define NUM_CONSUMIDORES 10
#define TAM_BUFFER 10

//Mutex e variáveis de condição
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;


//Estrutura de um elemento de fila bloqueante
typedef struct elem{
  	int value;
  	struct elem *prox;
}Elem;

//Estrutura de uma fila bloqueante
typedef struct blockingQueue{    
    unsigned int sizeBuffer, statusBuffer;
    Elem *head, *last;
}BlockingQueue;

//criando novo bloco queue
BlockingQueue *newBlockingQueue (unsigned int SizeBuffer){

    //Alocamos a fila bloqueante
    BlockingQueue *Q =  (BlockingQueue *) malloc (sizeof(BlockingQueue));

    //Definimos seus valores iniciais
    Q->sizeBuffer = SizeBuffer;
    Q->statusBuffer = 0;

    //Setamos os ponteiros de head e last para NULL
    Q->head = NULL;
    Q->last = NULL;

    return Q;
}

//Fila bloqueante global
BlockingQueue *Q;

//Função que coloca os elementos na fila
void putBlockingQueue (int newValue, int id){

  	//Trava o mutex para o fila
    pthread_mutex_lock(&mutex);
    
    //Elemento a ser inserido
	Elem *put;
	    
    //Alocamos o novo nó da lista
    put = (Elem *) malloc (sizeof(Elem));
    put->value = newValue;
    put->prox = NULL;

    //Indica se o elemento foi produzido
    bool can = false;

    //Se a fila esta cheia
	if(Q->statusBuffer == Q->sizeBuffer ){
        //Printa a mensagem indicando que produtor irá dormir
        printf("Produtor   %2d tentou inserir elemento de uma fila cheia\n", id);

        //Avisa os consumidores que tem elementos a serem consumidos	
        pthread_cond_wait(&empty, &mutex);        
	}
    //Se a fila está vazia
    else if ( Q->statusBuffer == 0 ){
            //'head' e 'last' apontam para o mesmo nó
            Q->head = put;
            Q->last = put;
            
            //Incrementamos a quantidade de nós da lista
            Q->statusBuffer++;
            
            //Se a fila estava vazia, nao esta mais: acorda os consumidores (tem o que consumir)
		    pthread_cond_broadcast(&fill);

            can = true;

    }	
    //Se a fila tem espaço para inserção
    else{					
		//Inserindo um novo valor na lista
		Q->last->prox = put;    //O último elemento da fila antiga aponta para o novo ultimo elemento          
		Q->last = put;          //'last' aponta para o ultimo elemento que foi inserido ('put')                       
        
        //Incrementamos a quantidade de nós da lista
        Q->statusBuffer++;

        can = true;

	}

    //Se conseguimos produzir algum elemento
    /*if(can){
        printf("Produtor %d produziu %d\n", id, newValue);
    }
    */

    //Liberamos o mutex para a fila bloqueante
  	pthread_mutex_unlock(&mutex);

}

//Função que retira os elementos da fila
int takeBlockingQueue (int id){

  	//Trava o mutex da fila
  	pthread_mutex_lock(&mutex);
  
    //Elemento a ser retirado
    Elem *cut;
    int valor;

    //Se existe elemento a ser retirado
    if( Q->statusBuffer >= 1 ){  
        //Iremos retirar o nó apontado por 'head'
        cut = Q->head;

        //O próximo nó apontado por 'head' será o 'cut->prox' (se houver 1 elemento, sera NULL)
        Q->head = cut->prox;
        
        //Se houver apenas um elemento na fila, 'last' apontam para NULL
        if(Q->statusBuffer == 1){
            Q->last = NULL;
        }

        //O valor retornado é 'cut->value'
        valor = cut->value;

        //Liberamos o 'cut'
        free(cut); 

        //Retiramos um elemento da fila
        Q->statusBuffer--;
    
        //Se a fila estava cheia, não está mais: acordam os produtores (tem espaço para produção)
        if( Q->statusBuffer == Q->sizeBuffer - 1 ){
            pthread_cond_broadcast(&empty);
        }

        //printf("Consumidor %d consumiu %d\n", id, valor);

    }
    //Caso não haja elementos a serem retirados
    else{
        //Printa a mensagem indicando que consumidor irá dormir
        printf("Consumidor %2d tentou retirar elemento de uma fila vazia\n", id);

        //Avisa aos produtores que pode produzir novos elementos 
        pthread_cond_wait(&fill, &mutex);            
    }

    //Liberamos o mutex da fila bloqueante
  	pthread_mutex_unlock(&mutex);
  
    //Retorna o valor retirado da fila
    return valor;

}

//Função produtor
void *producer(void *threadID){
    
    //ID da thread
    int id = *( (int *) threadID);

    //Loop de produção
    while( true ){
        int in = rand()%1000;
        putBlockingQueue(in, id);
    } 

}

//Função consumidor
void *consumer(void *threadID){

    //ID da thread
    int id = *( (int *) threadID);

    //Loop de consumo
    while( true ){
        int out = takeBlockingQueue(id);
    }

}

int main(){
    //Threads produtoras e consumidoras
  	pthread_t producer_thread[NUM_PRODUTORES];
  	pthread_t consumer_thread[NUM_CONSUMIDORES];

    //ID's produtores e consumidores
    int *ids_prod[NUM_PRODUTORES]; 
    int *ids_cons[NUM_CONSUMIDORES]; 
  	
    //Fila bloqueante
    Q = newBlockingQueue(TAM_BUFFER);
  	
    //Índice  
  	int i;

    //Criação das threads produtoras
  	for( i = 0 ; i < NUM_PRODUTORES ; i++ ){
        ids_prod[i] =(int *) malloc(sizeof(int)); 
		*ids_prod[i] = i; 
      	pthread_create(&producer_thread[i], NULL, producer, (void *) ids_prod[i]);
    } 
  
    //Criação das threads consumidoras  
  	for( i = 0 ; i < NUM_CONSUMIDORES ; i++ ){
		ids_cons[i] =(int *) malloc(sizeof(int)); 
		*ids_cons[i] = i;
      	pthread_create(&consumer_thread[i], NULL, consumer, (void *) ids_cons[i]);
    } 

	for( i = 0 ; i < NUM_PRODUTORES ; i++ ){
      	pthread_join(producer_thread[i], NULL);
    } 

	for( i = 0 ; i < NUM_CONSUMIDORES ; i++ ){
      	pthread_join(consumer_thread[i], NULL);
    }   

}