#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

/*============================================
============= CRIACAO DA STRUCT ==============
============================================*/

typedef struct parametro{
	double **matriz;
	int num_l;
	int num_c;
	int posicao_atual;
	int t;
} Parametro;

/*=============================================
=============== PROTOTIPO DAS FUNCOES =========
=============================================*/
void *rotaciona_matriz (void *arg);



/*=============================================
============== VARIAVEIS GLOBAIS ==============
=============================================*/

double **matriz_inv;

/*==============================================
============== FUNCAO MAIN =====================
==============================================*/

int main (int argc, char *argv[]){
	Parametro param[16];

	int i,j;

	int num_l = atoi(argv[1]);
	int num_c = atoi(argv[2]);                         //INICIALIZANDO VARIAVEIS
	int num_t = atoi(argv[3]);
	FILE *mat = fopen(argv[4], "r");
	FILE *mat_rot = fopen(argv[5], "w");

	if((mat == NULL) || (mat_rot == NULL)){
		printf("Erro na abertura do arquivo\n");
		exit(1);
	}



	pthread_t v_t[num_t];

	double **matriz_copia = (double**) malloc (sizeof(double*)*(num_l)); //ALOCANDO MATRIZ COPIA DINAMICAMENTE
	for(i = 0; i < num_l; i++){
		matriz_copia[i] = (double*) malloc(sizeof(double)*(num_c));
	}

	matriz_inv = (double**) malloc (sizeof(double*)*(num_c));	//ALOCANDO MATRIZ INVERTIDA DINAMICAMENTE
	for(i = 0; i < num_c; i++){
		matriz_inv[i] = (double*) malloc(sizeof(double)*(num_l));
	}


/*========================================================================================
=================== LEITURA DO ARQUIVO DE ENTRADA ========================================
========================================================================================*/

	for(i = 0; i < num_l; i++){
		for(j = 0; j < num_c; j++){
			fscanf(mat, "%lf", &matriz_copia[i][j]);
		}
	}


	clock_t tempo_inicial; //VARIAVEIS PARA O CALCULO DO CLOCK
	clock_t tempo_final;
	tempo_inicial = clock();



/*=========================================================================================
================== CRIACAO DAS THREADS E SWITCH PARA CADA CASO ============================
=========================================================================================*/
	 for(i = 0; i < num_t; i++){
		param[i].num_l = num_l;
		param[i].num_c = num_c;
		param[i].t = num_t;
		param[i].matriz = matriz_copia;
		switch(param[i].t){
			case 2:    //EM CASO DE 2 THREADS
				param[i].posicao_atual = i;
				pthread_create(&v_t[i], NULL, rotaciona_matriz, (void *) &param[i]);
				break;

			case 4:	   //EM CASO DE 4 THREADS
				param[i].posicao_atual = i;
				pthread_create(&v_t[i], NULL, rotaciona_matriz, (void *) &param[i]);
				break;

			case 8:	     //EM CASO DE 8 THREADS
				param[i].posicao_atual = i;
				pthread_create(&v_t[i], NULL, rotaciona_matriz, (void *) &param[i]);
				break;

			case 16:    //EM CASO DE 16 THREADS
				param[i].posicao_atual = i;
				pthread_create(&v_t[i], NULL, rotaciona_matriz, (void *) &param[i]);
				break;

			default:      //EM CASO DE NENHUMA DAS ANTERIORES
				printf("Valor invalido\n");
				break;
			}

	}

/*===========================================================================================
============== JOIN PARA DUAS THREADS NAO COMECAR AO MESMO TEMPO     ========================
===========================================================================================*/

	for(i = 0; i < num_t ; i++){
		pthread_join(v_t[i], NULL);
	}




/*==========================================================================================
================== CALCULO DO TEMPO DE EXECUCAO DAS THREADS EM MS ==========================
==========================================================================================*/
	tempo_final = clock();

	float tempo_total = (tempo_final - tempo_inicial)*1000.0/CLOCKS_PER_SEC;
	printf("\n %f \n", tempo_total);
	printf("\nPrograma executado com sucesso!\n");



/*==========================================================================================
=================            GRAVACAO NO ARQUIVO DE SAIDA     ==============================
==========================================================================================*/

	for(j = 0; j < num_c; j++){
		for(i = 0; i < num_l; i++){
			fprintf(mat_rot, "%2.f", matriz_inv[j][i]);
		}
		fprintf(mat_rot, "\n");
	}


	fclose(mat);
	fclose(mat_rot);

	return 0;
}

/*===========================================================================================
======================= FUNCAO QUE ROTACIONA A MATRIZ =======================================
===========================================================================================*/

void *rotaciona_matriz(void * arg){
	Parametro *st;
	st = (Parametro *) arg;
	int linha_atual;
	int coluna_atual;
	int posicao_ini = st -> posicao_atual;

	while((posicao_ini) < (st -> num_l * st -> num_c))
	{
		linha_atual = (posicao_ini)/(st -> num_c);
		coluna_atual = (posicao_ini)%(st -> num_c);
		if(st -> num_c != st -> num_l){
			matriz_inv[coluna_atual][linha_atual] = st -> matriz[(st -> num_l) - linha_atual - 1][coluna_atual];
		} //matriz nao quadratica
		else{
			matriz_inv[linha_atual][coluna_atual] = st -> matriz[(st -> num_c) - coluna_atual - 1][linha_atual];
		} //matriz quadratica
		posicao_ini = (posicao_ini) + (st -> t);
	}
}
