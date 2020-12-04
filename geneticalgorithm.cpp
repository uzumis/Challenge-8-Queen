//biblioteca
#include <iostream>
#include <time.h>
#include <cstdlib>
using namespace std;
//definicoes do professor
#define tam_pop 20
#define tam_genes 8
#define grau_elite 60/100
#define chance_mutacao 5/100
#define chance_cruzamento 80/100

//gerar solucao: aloca espaco na memoria pra definir uma solucao possivel.
int *gerarsolucao(){
	static int *soluc;
	unsigned int genes, j, ponto1, ponto2, aux;
	soluc = (int*) calloc(tam_genes, sizeof(int));
	
	for(genes=0; genes < tam_genes; genes++)
	{
		soluc[genes] = genes;
	}
	
	for(j=0; j<tam_genes-1; j++)
	{
		ponto1 = rand() % tam_genes;
		ponto2 = rand() % tam_genes;
			
		aux = soluc[ponto1];
		soluc[ponto1] = soluc[ponto2];
		soluc[ponto2] = aux;
	}

	return soluc;
	 
}	
//definir aptos: verifica usando o tamanho dos genes, nos casos as 8 rainhas e verifica por eles quem sera apto.
int aptidao(int *individuo)
{
	int i, j, score = (tam_genes * (tam_genes - 2)) + 1;
	
	for(i=0; i<tam_genes-1; i++)
	{
		for(j=0; j<tam_genes; j++)
		{
			if(individuo[i] == individuo[j] && i != j)
			{
				score -= 1;
			}
			
			if((i+individuo[i] == j+individuo[j] && i != j) || (individuo[i] - i == individuo[j] - j && i != j))
			{
				score -= 1;
			}
			
			if(individuo[i] == individuo[i+1]+1)
			{
				score -= 1;
			}
		}
		
		
	}
	return score;
}
//executar o cruzamento: recombinacao de cromossomos
void crossOver(int i1, int i2, int **populacao, int *filho){
	
	int pos = rand() % tam_genes;
	int i;
	
	for(i=0; i<=pos; i++){
		filho[i] = populacao[i1][i];
	}
	
	for(i=pos+1; i<tam_genes; i++){
		filho[i] = populacao[i2][i];
	}
}
//configurar mutacao: definir atraves da chance de mutacao para assegurar uma mutacao nova.
void mutacao(int *individuo)
{
	int ponto = rand() % tam_genes;
	int ponto2, temp;
	do{
		ponto2 = rand() % tam_genes;
	}
	while(ponto == ponto2);
	
	temp = individuo[ponto];
	individuo[ponto] = individuo[ponto2];
	individuo[ponto2] = temp;
	
	
}
//definir a selecao: selecao atraves da aptidao
int selecao(int ** populacao){

	int soma = 0;
	int i;
	int selec;
	for(i=0; i<tam_pop; i++)
		soma += aptidao(populacao[i]);
	int n = rand() % soma;
	soma = 0;
	
	for(i=0; i<tam_pop;i++){
		soma += aptidao(populacao[i]);
		if (soma > n){
			selec = i;
			break;
		} 
	}
	return selec;
}
//obter melhor: assegurar mais apto.
int obter_melhor(int **populacao)
{
	int i, scoreMelhor = aptidao(populacao[0]), indiceMelhor = 0;
	
	for(i=1; i<tam_pop; i++)
	{
		int score = aptidao(populacao[i]);
		if(score > scoreMelhor)
		{
			indiceMelhor = i;
			scoreMelhor = score;
			
		}
	}
	return indiceMelhor;
}
//rank da populacao: auto-explicativo.
void m_populacao(int **populacao, int **elite)
{
	int i, j;
	int *aux;
	int **populacao_ordenada;
	populacao_ordenada = (int**) calloc(tam_pop, sizeof(int*));
	
	for(i=0; i<tam_pop;i++)
	{
		populacao_ordenada[i] = (int*) calloc(tam_genes, sizeof(int));
	}
	
	for(i=0; i<tam_pop;i++)
	{
		for(j=0; j<tam_genes;j++)
		{
			populacao_ordenada[i][j] = populacao[i][j];
		}
	}

	for(i=0; i<tam_pop-1; i++)
	{
		for(j=0; j<tam_pop-1-i; j++)
		{
			if(aptidao(populacao_ordenada[j]) < aptidao(populacao_ordenada[j+1]))
			{
				aux = populacao_ordenada[j+1];
				populacao_ordenada[j+1] = populacao_ordenada[j];
				populacao_ordenada[j] = aux;
			}
		}
	}
	
	for(i=0; i<((double) (grau_elite*tam_pop)); i++)
	{
		for(j=0; j<tam_genes; j++)
		{
			elite[i][j] = populacao_ordenada[i][j];
		}
	}
	
}
//aptidao media: formular a media dos mais aptos.
double mediaaptidao(int **populacao){
	int i, totalFit = 0;
	
	for(i=0; i<tam_pop;i++){
	    totalFit += aptidao(populacao[i]);
	}
	return (double) (totalFit/tam_pop);
	
}
//execucao do algoritmo genetico
int main(){
	time_t t;
	srand(time(&t));
	
	int ** populacao, *filho, **elite, *individuo; // Matrizes ponteiros
	int i; //variaveis auxiliares
	int tam_elite = (int) (grau_elite * tam_pop); // tamanho da elite
	populacao = (int**) malloc(tam_pop * sizeof(int*)); // alocar matriz populacao
	filho = (int*) malloc(tam_genes * sizeof(int)); // alocar matriz filho
	elite = (int**) malloc(tam_elite * sizeof(int*)); // elite
	individuo = (int*) malloc(tam_genes * sizeof(int)); // individuo
	for(i=0; i<tam_pop;i++)
		populacao[i] = (int*) malloc(tam_genes * sizeof(int));
	
	for(i=0 ;i<tam_elite; i++){
		elite[i] = (int*) malloc(tam_genes * sizeof(int));
	}

	
	
	for(i=0; i<tam_pop; i++){
		populacao[i] = gerarsolucao();
	}
	m_populacao(populacao, elite);
	int geracao;	
	for(geracao = 0; ; geracao++){
		
		for(i=0; i<tam_pop;i++)
			populacao[i] = gerarsolucao();
		
		
		for(i=0; i<tam_elite; i++){
			if(aptidao(populacao[i]) < aptidao(elite[i])){
				populacao[i] = elite[i];
			}
		}			
		
		double prob = rand();
		int cruzamentos = (int) (tam_pop/3);
		
		for(i=0; i<cruzamentos; i++){
			int i1 = selecao(populacao);
			int i2 = selecao(populacao);
			crossOver(i1, i2, populacao, filho);
			prob = rand() ;
			
			if(prob < chance_mutacao)
				mutacao(filho);
			
			if(aptidao(filho) > aptidao(populacao[i1]))
				populacao[i1] = filho;
		}
		
		cout<<"\n \nGeracao "<<geracao+1;
		cout<<"\nMedia de Aptidao: "<<mediaaptidao(populacao);
		cout<<"\n\nMais aptas (Numeros correspondem a linha do tabuleiro de xadrez (de 0 a 7): ";
		for(i=0; i<tam_genes; i++)
		cout<<populacao[obter_melhor(populacao)][i]<<" ";
		cout<<" \n";
		cout<<"\nPontuacao: "<<aptidao(populacao[obter_melhor(populacao)])<<"\n\n--------------------------------";
		m_populacao(populacao, elite);
		if(aptidao(populacao[obter_melhor(populacao)]) == ((tam_genes * (tam_genes - 2)) + 1)) break;
		
	}


	free(populacao);
	free(filho);
	free(elite);
	free(individuo);
	
	
	return 0;
}                                                        