#include <bits/stdc++.h>
#include <pthread.h>
#define N 1000

using namespace std;

//Bitmap
string bitmap[N];

//Estrutura passada como parametro da funcao
typedef struct{
    int id;
    string resposta;
    int line_begin;
    int line_end;
    int col_begin;
    int col_end;
} dados;

//Funcao que quebra o bitmap
void* bitmapFunc (void * estrutura){

    //Bitmap analisado
    dados set = *( (dados *) estrutura); 

    dados *info = (dados *) estrutura; 

    //Threads usadas nas divisões do bitmap
    pthread_t thread[4];

    //Booleanos que indicam presenca de 0's 1's 
    bool um = false;
    bool zero = false;

    //Percorre a matriz
    for( int i = set.line_begin ; i < set.line_end ; i++ ){

       for(int j = set.col_begin ; j < set.col_end ; j++ ){

           //Indica presenca de 0 
           if( bitmap[i][j] == '0' ){
               zero = true;
           }
           //Indica presenca de 1
           else{
               um = true;
           }

       }
    }

    //Se o bitmap esta dentro dos limites corretos
    if( set.col_end > set.col_begin && set.line_end > set.line_begin ){

        //Se nao existe '1' no bitmap
        if( !um ){
            info->resposta = '0';

        }
        //Se nao existe '0' no bitmap
        else if( !zero ){
            info->resposta = '1';

        }
        //Se existe '1' e '0' no bitmap
        else{
            //Printamos "D" indicando a divis�o e quebramos o bitmap 
            //printf("D");

            //Sub-matrizes do bitmap
            dados subs[4];

            //Inicialmente, todas as sub-matrizes tem as mesmas informacoes da matriz total
            for( int i = 0 ;  i < 4 ; i++ ){
                subs[i] = set;
                subs[i].id = set.id+1;
            }

            //Linha e coluna de quebra do bitmap
            int midLine, midCol;

            //Calcula a posicao do meio da linha
            if( ( set.line_begin + set.line_end )%2 == 0 ){
                midLine = ( set.line_begin + set.line_end )/2;
            }
            else{
                midLine = (set.line_begin + set.line_end)/2 + 1;
            }

            //Calcula a posicao do meio da coluna
            if( ( set.col_begin + set.col_end )%2 == 0 ){
                midCol = ( set.col_begin + set.col_end )/2;
            }
            else{
                midCol = ( set.col_begin + set.col_end )/2 + 1;
            }

            //Mudamos as coordenadas certas e chamamos recursivamente o Bitmap
            subs[0].col_end = midCol;
            subs[0].line_end = midLine;
            pthread_create (&thread[0], NULL, bitmapFunc, (void*) &subs[0]);

            subs[1].col_begin = midCol;
            subs[1].line_end = midLine;
            pthread_create (&thread[1], NULL, bitmapFunc, (void*) &subs[1]);

            subs[2].col_end = midCol;
            subs[2].line_begin = midLine;
            pthread_create (&thread[2], NULL, bitmapFunc, (void*) &subs[2]);

            subs[3].col_begin = midCol;
            subs[3].line_begin = midLine;
            pthread_create (&thread[3], NULL, bitmapFunc, (void*) &subs[3]);

            //Esperamos as 4 threads
            pthread_join(thread[0], NULL);
            pthread_join(thread[1], NULL);
            pthread_join(thread[2], NULL);
            pthread_join(thread[3], NULL);

            //'D' denota uma quebra no bitmap
            info->resposta = 'D';
            
            //Indice
            int i;

            //Concatenamos as respostas dos quadrantes do bitmap	
            for( i = 0 ; i < 4 ; i++ ){
                info->resposta += subs[i].resposta;
            }
        }

        //Se a thread em questão for a inicial, printamos a resposta e é o fim da funcao
        if( info->id == 0 ){
            cout << info->resposta;
        }

    }

} 


int main(){
    //Informacoes do bitmap inicial
    dados info;

    //Thread inicial para o bitmap inteiro
    pthread_t thread;

    //Indices
    int i, j;
    
    //Numero de linhas e colunas
    int linhas, colunas;

    //Lemos o numero de linhas e colunas
    scanf("%d %d", &linhas, &colunas);

    //Lemos o bitmap
    for( int i = 0 ; i < linhas ; i++ ){

        cin >> bitmap[i];

    }

    //Bitmap inicial
    info.line_begin = 0;
    info.line_end = linhas;
    info.col_begin = 0;
    info.col_end = colunas;
    info.id = 0;

    //Criamos a thread principal com o bitmap inteiro
    pthread_create(&thread, NULL, bitmapFunc, (void *) &info);
    
    //Esperamos o retorno da thread principal
    pthread_join(thread, NULL);

    //Quebra de linha ao fim do caso
    puts("");
 
    //Encerra o programa
    pthread_exit(NULL);
}