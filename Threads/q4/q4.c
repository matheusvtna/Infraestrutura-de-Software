#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define MAX 10000

//Vetor de votos por candidato
int *contador;

//Total de votos
int long long cont=0;

//Total de votos válidos
int long long conts0=0;

//Número de arquivos
int N;
int NRO;

//Vetor que indica se um arquivo já foi aberto
bool vetor[MAX];

//Mutex para os candidatos
pthread_mutex_t *mutex;

//Mutex para os arquivos
pthread_mutex_t *mutexArquivos;//vai proteger a seção crítica

//Vetor das threads
pthread_t *thread;

//FUnção que lê os arquivos e computa os votos
void *opread(void * threadID);


int main(){
    int c;              //Número de candidatos    
    int n;              //Número de arquivos
    int nro;            //Número de threads
  
    int i;              //Auxiliar
  
    int maior=0;        //Quantidade máxima de votos
    int candidato=0;    //Candidato mais votado
    
    float var1;         
    float var2;
    float var3;
    double var4;
  
    //Lê 'n'
    printf("Digite o numero de arquivos:\n");
    scanf("%d", &n);
  
    //Define N = número de arquivos
    N=n;
  
    //Lê 'nro'
    printf("Digite o numero de threads:\n");
    scanf("%d", &nro);

    NRO=nro;

    //Lê o número de candidatos
    printf("Digite o numero de candidatos\n");
    scanf("%d", &c);
  
    //Aloca o vetor de mutex para os arquivos 
    mutexArquivos = (pthread_mutex_t*) malloc (n* sizeof(pthread_mutex_t));

    //Aloca o vetor de mutex para os candidatos
    mutex = (pthread_mutex_t*) malloc ((c+1)* sizeof(pthread_mutex_t)); 

    //Aloca o vetor de threads
    thread = (pthread_t *) malloc(nro*sizeof(pthread_t));
  
    //Aloca o vetor de votos por candidato
    contador = (int*) malloc ((c+1)* sizeof(int));

    //Inicializa os votos em zero
    for( i = 0 ; i <= c ; i++ ){
        contador[i]=0;
    }

    //Seta o vetor inicialmente pare 'FALSE' (nenhum arquivo foi aberto)
    for( i = 0 ; i < n ; i++ ){
        vetor[i] = false;
    }
    
    //Inicializa os mutex para os candidatos
    for( i = 0; i <= c ; i++ ){
        pthread_mutex_init (&mutex[i], NULL );//inicializa o mutex para DEFAULT 
    }

    //Inicializa os mutex para os arquivos
    for( i = 0; i <= n ; i++ ){
        pthread_mutex_init (&mutexArquivos[i], NULL );//inicializa o mutex para DEFAULT 
    }

    //Vetor de ID's
    int *id[MAX];
    
    //Criação das threads
    for( i = 0 ; i < nro ; i++ ){
        id[i] = (int *)malloc (sizeof(int));
        *id[i]=i;
        pthread_create(&thread[i], NULL, opread, (void *) id[i]);
    }
  
    //União das threads
    for( i = 0 ; i < nro ; i++ ){
        pthread_join(thread[i], NULL); 
    }
  
    //Computa os votos
    for( i = 0 ; i <= c ; i++ ){
        cont += contador[i];
        
        if( i != 0 ){
            conts0 += contador[i];
        }
    }
  
    printf("Total de votos : %lld\n", cont);

    //Define o vencedor
    for( i = 1 ; i <= c; i++ ){

        var1=contador[i];
        var2=conts0;

        var3=(var1)/var2;

        var3=var3*100;

        printf("Candidato %d recebeu %.2lf por cento \n",i,var3);
        
        if( contador[i] > maior ){
            maior = contador[i];
            candidato = i;
        }
    }
  
    printf("%d\n", candidato);
  
    pthread_exit(NULL);

}

void *opread(void *threadID){
    int id = *( (int *) threadID);
    int i;
    FILE *fp;
    int x;
    int d;
    char buffer[50];
  
    //Abre os N arquivos
    for( i = id ; i < N ; i+=NRO ){

        sprintf(buffer,"%d.in",i+1);

        fp = fopen ( buffer, "r");

        //Enquanto existirem votos a serem computados, lemos o candidato votado
        while(fscanf(fp,"%d", &d)!=EOF){
            //Travamos o mutex para este candidato    
            pthread_mutex_lock(&mutex[d]);

            //Incrementamos sua contagem de votos 
            contador[d]++;
                    
            //Liberamos o mutex para este candidato
            pthread_mutex_unlock(&mutex[d]);  
        }

        //Fechamos o arquivo ao fim dos votos  
        fclose(fp);
        
    }
    
    pthread_exit(NULL);
}