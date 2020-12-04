#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define N 8
using namespace std;

//distribui as 8 rainhas em pontos iniciais no algoritmo, do tabuleiro e do estado.
void aleatorio(int tabuleiro[][N],int* estado)
{
    //seed
	srand(time(0));
	//iteracao entre os indices da coluna
    for (int i = 0; i < N; i++) {
    	//obter uma linha aleatoria
        estado[i] = rand() % N;
        //colocar rainha no tabuleiro
        tabuleiro[estado[i]][i] = 1;
    }
}

//imprimir tabuleiro em um "tabuleiro" simulado atraves de uma matriz
void imprimir_tabuleiro(int tabuleiro[][N])
{

    for (int i = 0; i < N; i++) {
        cout << " ";
        for (int j = 0; j < N; j++) {
            cout << tabuleiro[i][j] << " ";
        }
        cout << "\n";
    }
}
//Imprime o array estado.
void imprimir_estado(int* estado)
{

    for (int i = 0; i < N; i++) {
        cout << " " << estado[i] << " ";
    }
    cout << endl;
}
//
bool comparar_estados(int* estado1, int* estado2)
{

    for (int i = 0; i < N; i++) {
        if (estado1[i] != estado2[i]) {
            return false;
        }
    }
    return true;
}
void preencher(int tabuleiro[][N], int valor)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            tabuleiro[i][j] = valor;
        }
    }
}

//configurar IA para movimento da rainha em cada coluna.
//checar se elas caem no mesma linha, 
//se encontrado é incrementado a variavel "atacando"
//isso assegura se as rainhas ao se movimentarem não apresentarem risco umas 
//as outras.
int calcular_objetivo(int tabuleiro[][N], int* estado)
{

    int attacking = 0;
    int linha, coluna;

    for (int i = 0; i < N; i++) {
//esquerda
        linha = estado[i], coluna = i - 1;
        while (coluna >= 0
               && tabuleiro[linha][coluna] != 1) {
            coluna--;
        }
        if (coluna >= 0
            && tabuleiro[linha][coluna] == 1) {
            attacking++;
        }

//direita
        linha = estado[i], coluna = i + 1;
        while (coluna < N
               && tabuleiro[linha][coluna] != 1) {
            coluna++;
        }
        if (coluna < N
            && tabuleiro[linha][coluna] == 1) {
            attacking++;
        }

//diagonal esquerda+cima
        linha = estado[i] - 1, coluna = i - 1;
        while (coluna >= 0 && linha >= 0
               && tabuleiro[linha][coluna] != 1) {
            coluna--;
            linha--;
        }
        if (coluna >= 0 && linha >= 0
            && tabuleiro[linha][coluna] == 1) {
            attacking++;
        }

//diagonal direita+baixo
        linha = estado[i] + 1, coluna = i + 1;
        while (coluna < N && linha < N
               && tabuleiro[linha][coluna] != 1) {
            coluna++;
            linha++;
        }
        if (coluna < N && linha < N
            && tabuleiro[linha][coluna] == 1) {
            attacking++;
        }

//diagonal esquerda+baixo
        linha = estado[i] + 1, coluna = i - 1;
        while (coluna >= 0 && linha < N
               && tabuleiro[linha][coluna] != 1) {
            coluna--;
            linha++;
        }
        if (coluna >= 0 && linha < N
            && tabuleiro[linha][coluna] == 1) {
            attacking++;
        }

//diagonal direita+cima
        linha = estado[i] - 1, coluna = i + 1;
        while (coluna < N && linha >= 0
               && tabuleiro[linha][coluna] != 1) {
            coluna++;
            linha--;
        }
        if (coluna < N && linha >= 0
            && tabuleiro[linha][coluna] == 1) {
            attacking++;
        }
    }

    // retornar pares.
    return (int)(attacking / 2);
}
//ao dar o atual estado, essa função gera um tabuleiro. 
void gerar_tabuleiro(int tabuleiro[][N], int* estado)
{

    preencher(tabuleiro, 0);
    for (int i = 0; i < N; i++) {
        tabuleiro[estado[i]][i] = 1;
    }
}
//função que copia as informações do estado 2 para o estado 1.
void copiar_estado(int* estado1, int* estado2)
{

    for (int i = 0; i < N; i++) {
        estado1[i] = estado2[i];
    }
}
//funcao seleciona o atual estado do vizinho com menor valor incrementado quanto aos movimentos da rainha.
void obter_vizinho(int tabuleiro[][N], int* estado)
{
	//declarando o tabuleiro e o estado considerado otimo.
    int tabuleiro_otimo[N][N];
    int estado_otimo[N];

    copiar_estado(estado_otimo,
              estado);
    gerar_tabuleiro(tabuleiro_otimo,
                  estado_otimo);

// inicializar o valor objetivo otimizado 
    int objetivo_otimo
        = calcular_objetivo(tabuleiro_otimo,
                             estado_otimo);
    
//declarando um tabuleiro temporario para execução de possiveis iterações
	int tabuleiro_vizinho[N][N];
    int estado_vizinho[N];

    copiar_estado(estado_vizinho,
              estado);
    gerar_tabuleiro(tabuleiro_vizinho,
                  estado_vizinho);
//Iniciar iterações. Essa iteração só para se o estado do tabuleiro temporário tiver valor 
//
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (j != estado[i]) {

                estado_vizinho[i] = j;
                tabuleiro_vizinho[estado_vizinho[i]][i]
                    = 1;
                tabuleiro_vizinho[estado[i]][i]
                    = 0;
                int temp
                    = calcular_objetivo(
                        tabuleiro_vizinho,
                        estado_vizinho);
                        
// se o temporário tiver melhor desempenho que o estado otimizado, o estado temporário se tornará o otimizado.
                if (temp <= objetivo_otimo) {
                    objetivo_otimo = temp;
                    copiar_estado(estado_otimo,
                              estado_vizinho);
                    gerar_tabuleiro(tabuleiro_otimo,
                                  estado_otimo);
                }
                
                //Retorna à configuração original para a proxima iteração.

                tabuleiro_vizinho[estado_vizinho[i]][i]
                    = 0;
                estado_vizinho[i] = estado[i];
                tabuleiro_vizinho[estado[i]][i] = 1;
            }
        }
    }

    copiar_estado(estado, estado_otimo);
    preencher(tabuleiro, 0);
    gerar_tabuleiro(tabuleiro, estado);
}
void hill_climbing(int tabuleiro[][N], int* estado)
{
	int iteracao;
    int tabuleiro_vizinho[N][N] = {};
    int estado_vizinho[N];

    copiar_estado(estado_vizinho, estado);
    gerar_tabuleiro(tabuleiro_vizinho,
                  estado_vizinho);

    do {


        copiar_estado(estado, estado_vizinho);
        gerar_tabuleiro(tabuleiro, estado);

 

        obter_vizinho(tabuleiro_vizinho,
                     estado_vizinho);

        if (comparar_estados(estado,
                          estado_vizinho)) {

            imprimir_tabuleiro(tabuleiro);
			iteracao++;
            break;
        }
        else if (calcular_objetivo(tabuleiro,estado)
                 == calcular_objetivo(
                        tabuleiro_vizinho,
                        estado_vizinho)) {
            estado_vizinho[rand() % N]
                = rand() % N;
            gerar_tabuleiro(tabuleiro_vizinho,
                          estado_vizinho);
        }
        iteracao++;
    } while (true);
    cout<<"Iteracoes possiveis: "<<iteracao;
}
int main()
{

    int estado[N] = {};
    int tabuleiro[N][N] = {};
    //Adicionando estado inicial
    //Configurando randomicamente o tabuleiro
    aleatorio(tabuleiro, estado);
    //Executar o hill climbing
    hill_climbing(tabuleiro, estado);

    return 0;
}
