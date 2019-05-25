#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define N 10000000

//Vetor analisado
int vetor[N];

//Pair <int, int> 
typedef struct{
    int inicio;
    int fim;
} pii;


//Ordena os sub-vetores
void mergeSort(int inicio, int fim, int pivo){

    //Vetor auxiliar, responsável por armazenar os números ordenados
	int aux[fim-inicio+1]; 

    //Auxiliares
	int i, j, k, n, lim;

	lim = pivo+1; 
	n = inicio; 
	k = 0; 

	//Ordena enquanto houver elementos em ambos os lados
	while( n <= pivo && lim <= fim ){
    
		if( vetor[n] <= vetor[lim] ){
			aux[k] = vetor[n];
            k++; 
			n++; 
		}
		else{
			aux[k] = vetor[lim];
			k++;  
			lim++; 
		}
	}

	//Caso não exista mais elementos na segunda parte do vetor, repassamos todos os elementos da primeira
	while( n <= pivo ){
		aux[k] = vetor[n];
		k++;
		n++;
	}

	//O mesmo ocorre na situacao inversa
	while( lim <= fim ){
		aux[k] = vetor[lim];
		k++;
		lim++;
	}

	//Precisamos repassar o vetor auxiliar para o vetor original
	for( i = 0 ; i < k ; i++ ){
		vetor[inicio] = aux[i];  
		inicio++;
	}
}

//Quebra o vetor em duas partes
void* merge(void* par){

    //Conversão do par
    pii A = *( (pii*)par );

    //Threads usadas
    pthread_t t1, t2;

    if( A.fim > A.inicio){
        //Pivo de quebra do vetor
        int pivo = (A.fim + A.inicio)/2;
        
        //Partes esquerda (B) e direita (C) do vetor
        pii B, C;

        B.inicio = A.inicio;
        B.fim = pivo;

        C.inicio = pivo + 1;
        C.fim = A.fim;

        //Criação das threads para ambas as partes do sub-vetor
        pthread_create(&t1, NULL, merge, (void *) &B);
        pthread_create(&t2, NULL, merge, (void *) &C);

        //Encerra as threads 
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        
        //MergeSort
        mergeSort(A.inicio, A.fim, pivo);
    }

}

//Função "master" para a ordenação
void sort(int tam){

    //Thread master
    pthread_t thread;

    //Par inicial (0, tam -1)
    pii A;
    A.inicio = 0;
    A.fim = tam - 1;

    //Início do algoritmo
    pthread_create(&thread, NULL, merge, (void *) &A);

    //Aguarda a thread retornar
    pthread_join(thread, NULL);

}

//Printa o vetor ordenado
void result(int tam){

    //Auxiliar
    int i;

    puts("Vetor Ordenado:");

    //Percorre todo o vetor, printando o seu conteúdo
    for( i = 0 ; i < tam ; i++ ){
        printf("%d ", vetor[i]);
    }

    //Quebra de linha
    puts("");

}

int main(){
    //Tamanho do vetor
    int tam;

    //Auxiliar
    int i;

    puts("Digite quantos valores você deseja por no vetor:");
    scanf("%d", &tam);

    puts("Digite os valores desejados:");

    //Leitura dos valores
    for( i = 0 ; i < tam ; i++ ){
        scanf("%d", &vetor[i]);
    }

    //Chamada da função "master"
    sort(tam);

    //Printa o resultado da ordenação
    result(tam);

    return 0;
}