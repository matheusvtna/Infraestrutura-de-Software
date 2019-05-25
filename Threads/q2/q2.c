#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#define N 1000

//Matriz A (NxN) dos coeficientes
double A[N][N];

//Matriz b (Nx1) dos resultados
double b[N];

//Matriz x (Nx1) das incógnitas numa iteração k 
double x[N];

//Matriz x (Nx1) das incógnitas numa iteração k - 1
double X[N];

//Ordem da matriz
int ordem;

//Número máximo de iterações
int P;

//Número de threads
int nt;

//Threads
pthread_t *threads;

//Barreira
pthread_barrier_t barreira1, barreira2; 

//Função que resolve o sistema de equações lineares a partir do modo de Jacobi
void* jacobi(void* threadID){

    //i-ésima incógnita
    int ind = *( (int *) threadID);
    
    //Índices
    int i, j, m, k;

    //Auxiliar
    double aux;

    //Método de Jacobi
    for( k = 0 ; k < P ; k++ ){ 

        //Espera as threads atingirem a primeira barreira
        pthread_barrier_wait(&barreira1);       

        for( i = ind ; i < ordem ; i+=nt ){
            
            aux = 0;

            for( j = 0 ; j < ordem ; j++ ){

                if( j != i ){
                    aux += A[i][j]*X[j]/A[i][i];
                }  
            }

            x[i] = (b[i]/A[i][i]) - aux; 
            
        }

        //Espera as threads atingirem a segunda barreira
        pthread_barrier_wait(&barreira2);   

        for( m = 0 ;  m < ordem ; m++ ){
            X[m] = x[m];
        }

    }

}


int main(){
    //Índices
    int i, j;

    //Vetor de ID's
    int *ids[N]; 

    //Leitura da ordem da matriz
    puts("Digite a ordem da matriz:");
    scanf("%d", &ordem);

    //Leitura da matriz de coeficientes
    puts("Digite os coeficientes do sistema de equações lineares:");
    for( i = 0 ;  i < ordem ; i++ ){
        for( j = 0 ; j < ordem ; j++ ){
            scanf("%lf", &A[i][j]);
        }
    }

    //Leitura dos resultados 
    puts("Digite os termos independentes:"); 
    for( i = 0 ; i < ordem ; i++ ){
        scanf("%lf", &b[i]);
    }

    //Inicialmente, setamos o valores da i incógnitas para 1
    for( i = 0 ; i < ordem  ; i++ ){
        X[i] = 0;
    }

    //Leitura de P
    puts("Digite o número de iterações desejadas para o refinamento:");
    scanf("%d", &P);

    //Leitura de nt
    puts("Digite o número de threads desejadas:");
    scanf("%d", &nt);

    //Inicialização da barreira
    pthread_barrier_init(&barreira1, NULL, nt);
    pthread_barrier_init(&barreira2, NULL, nt);

    //Alocação, criação e união das threads 
    threads = (pthread_t *) malloc(nt*sizeof(pthread_t));
    for( i = 0 ; i < nt ; i++ ){
        ids[i] =(int *) malloc(sizeof(int)); 
        *ids[i] = i;
        pthread_create(&threads[i], NULL, jacobi, (void *)ids[i]);
    }

    for( i = 0 ; i < nt ; i++ ){
        pthread_join(threads[i], NULL);
    }

    //Destruição das barreiras
    pthread_barrier_destroy(&barreira1);
    pthread_barrier_destroy(&barreira2);

    //Liberação do ponteiro das threads
    free(threads);

    //Printa o resultado
    for( i = 0 ; i < ordem ; i++ ){
        printf("x[%d] = %lf\n", i, x[i]);
    }

    pthread_exit(NULL);
    
}