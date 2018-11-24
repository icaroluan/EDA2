#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

# define TAMANHO_ARQUIVO 11
# define VAZIO -1

FILE *file_p;

typedef struct {
	int chave;
	int idade;
	char nome[21];
}Dados_registro;

Dados_registro dados_registro_tmp;
Dados_registro dados_registro_auxiliar;
int cont_resgistro=0, insert=0;

#define TAMANHO_DADOS sizeof(Dados_registro) 

int indice;

void iniciar_arquivo(){

	file_p=fopen("arquivo.bin", "rb");
	if(file_p){
		fclose(file_p);
	}else{
		Dados_registro dados_registro_tmp;
		file_p=fopen("arquivo.bin", "wb");
		dados_registro_tmp.chave=VAZIO;
		for(int i=0;i<TAMANHO_ARQUIVO;i++){
			fwrite(&dados_registro_tmp, TAMANHO_DADOS, 1, file_p);
		}
		fclose(file_p);
	}
}


int hash(int registro_chave){
	int h1 = registro_chave%TAMANHO_ARQUIVO;
	return h1;
}

int hash_duplo(int registro_chave){
	int h2 = registro_chave/TAMANHO_ARQUIVO;
	h2 = h2%TAMANHO_ARQUIVO;
	if(h2==0){
		h2=1;
	}
	return h2;
}

bool consulta_registro(int registro_chave) {
	Dados_registro dados_registro_auxiliar;
	int h=hash(dados_registro_tmp.chave);
	int h_d=hash_duplo(dados_registro_tmp.chave);
	for(int i=0;i<TAMANHO_ARQUIVO;i++){
		int valor=(((h_d*i)+h)%TAMANHO_ARQUIVO);
		fseek(file_p, TAMANHO_DADOS * valor, SEEK_SET);
		fread(&dados_registro_auxiliar, TAMANHO_DADOS, 1, file_p);
		if(dados_registro_auxiliar.chave== registro_chave){
      return true;
		}
	}
	return false;
}

void inseri_registro(){
	Dados_registro dados_registro_tmp, dados_registro_auxiliar;
	file_p=fopen("arquivo.bin","r+b");
	scanf("%d ", &dados_registro_tmp.chave);
	scanf(" %[a-z A-Z]s ", dados_registro_tmp.nome);
	scanf("%d ", &dados_registro_tmp.idade);

	int h=hash(dados_registro_tmp.chave);
	int h_d=hash_duplo(dados_registro_tmp.chave);
	if(consulta_registro(dados_registro_tmp.chave)){
		printf("chave ja existente: %d\n", dados_registro_tmp.chave);
	}else{
		//insert++;
		for(int i=0; i< TAMANHO_ARQUIVO; i++){
			int valor=(((h_d*i)+h)%TAMANHO_ARQUIVO);		
			fseek(file_p, TAMANHO_DADOS * valor, SEEK_SET);
			fread(&dados_registro_auxiliar, TAMANHO_DADOS, 1, file_p);
			if(dados_registro_auxiliar.chave==VAZIO){
				fseek(file_p, TAMANHO_DADOS * valor, SEEK_SET);
				fwrite(&dados_registro_tmp, TAMANHO_DADOS, 1, file_p);
				fclose(file_p);
				return;
			}
		}
	}
	fclose(file_p);
	return;
}


void consulta(){
	Dados_registro dados_registro_tmp;
	scanf("%d", &indice);
	file_p=fopen("arquivo.bin", "rb");
	
	int h=hash(indice);
	int h_d=hash_duplo(indice);

	for(int i=0; i<TAMANHO_ARQUIVO;i++){
		int valor=(((h_d*i)+h)%TAMANHO_ARQUIVO);
		fseek(file_p, TAMANHO_DADOS * valor, SEEK_SET);
		fread(&dados_registro_tmp, TAMANHO_DADOS, 1, file_p);
		if(dados_registro_tmp.chave == indice){
			printf("chave: %d\n", dados_registro_tmp.chave);
			printf("%s\n",dados_registro_tmp.nome);
			printf("%d\n", dados_registro_tmp.idade);
			fclose(file_p);
			return;
		}
	}
	printf("chave nao encontrada: %d\n", indice);
	fclose(file_p);
	return;
}

void imprimir_registro(){
	Dados_registro dados_registro_tmp;
	file_p=fopen("arquivo.bin", "rb");
	for(int i=0;i<TAMANHO_ARQUIVO;i++){
		fread(&dados_registro_tmp, TAMANHO_DADOS, 1, file_p);
		//fread(&dados_registro_tmp, 1, sizeof(dados_registro_tmp), file_p);
		if(dados_registro_tmp.chave > VAZIO){
			printf("%d: %d %s %d\n", i, dados_registro_tmp.chave, dados_registro_tmp.nome, dados_registro_tmp.idade);
		}else{
			printf("%d: vazio\n", i);
		}
	}
	fclose(file_p);
}

void remover_registro(){
	Dados_registro dados_registro_tmp;
	int indice;
	scanf("%d", &indice);
	file_p=fopen("arquivo.bin", "r+b");
	
	int h=hash(indice);
	int h_d=hash_duplo(indice);

	for(int i=0; i<TAMANHO_ARQUIVO;i++){
		int valor=(((h_d*i)+h)%TAMANHO_ARQUIVO);
		fseek(file_p, TAMANHO_DADOS * valor, SEEK_SET);
		fread(&dados_registro_tmp, TAMANHO_DADOS, 1, file_p);
		if(dados_registro_tmp.chave == indice){
			dados_registro_tmp.chave=VAZIO;
			fseek(file_p, TAMANHO_DADOS * valor, SEEK_SET);
			fwrite(&dados_registro_tmp, TAMANHO_DADOS, 1, file_p);
			fclose(file_p);
			return;
		}
	}
	printf("chave nao encontrada: %d\n", indice);
	fclose(file_p);
	return;
}

void media_registro(){
  Dados_registro dados_registro_tmp;
  Dados_registro confere;
  int chave=0;
  file_p=fopen("arquivo.bin", "r+b");
  
  for(int i=0; i<TAMANHO_ARQUIVO;i++){
		fseek(file_p, TAMANHO_DADOS * i, SEEK_SET);
		fread(&dados_registro_tmp, TAMANHO_DADOS, 1, file_p);
		if(dados_registro_tmp.chave != VAZIO){
		  insert++;
      int contador_parcial=0;
      chave=dados_registro_tmp.chave;
      int h=hash(chave);
	    int h_d=hash_duplo(chave);   

      for(int j=0; j<TAMANHO_ARQUIVO;++j){
        contador_parcial++;
        int valor=(((h_d*j)+h)%TAMANHO_ARQUIVO);
        fseek(file_p, TAMANHO_DADOS * valor, SEEK_SET);
        fread(&confere, TAMANHO_DADOS, 1, file_p);
        if(confere.chave == chave){
          cont_resgistro=contador_parcial+cont_resgistro;
          break;
        }
      }
    }
	}
  
	//printf("cont_resgistro --> %d\n", cont_resgistro);
	//printf("Insert --> %d\n", insert);
	float media = (float)cont_resgistro/ (float)insert;
	printf("%.1f\n", media);
	fclose(file_p);
}

int main(){
	char entrada[1];
	iniciar_arquivo();
	while(scanf("%c", &entrada[0])){
		if(entrada[0] == 'i'){
			inseri_registro();
		}
		if(entrada[0] == 'c'){
			consulta();
		}
		if(entrada[0] == 'r'){
			remover_registro();
		}
		if(entrada[0] == 'p'){
			imprimir_registro();
		}
		if(entrada[0] == 'm'){
			media_registro();
		}
		if(entrada[0] == 'e'){
			return 0;
		}
	}
return 0;
}