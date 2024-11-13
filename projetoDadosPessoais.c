#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct Registro {
	unsigned long long cpf;
	char nome[100];
	char dataNasc[11];
	char endereco[200];
	char telefone[15];
} Registro;

typedef struct Estrutura {
	Registro dado;
	int altura;
	struct Estrutura *esq;
	struct Estrutura *dir;
} Estrutura;

int calculaAltura(Estrutura *no){
	int alturaDir = 0;
	int alturaEsq = 0;
	
	if(!no)
		return -1;
		
	if(no->esq)
		alturaEsq = calculaAltura(no->esq) + 1;
		
	if(no->dir) 
		alturaDir = calculaAltura(no->dir) + 1;
		
	return (alturaEsq > alturaDir)?(alturaEsq):(alturaDir);
}

Estrutura *rotacaoDir(Estrutura *antigaRaiz) {
	Estrutura *novaRaiz = antigaRaiz->esq;
	Estrutura *aux = novaRaiz->dir;
	novaRaiz->dir = antigaRaiz;
	antigaRaiz->esq = aux;
	
	return novaRaiz;
}

Estrutura *rotacaoEsq(Estrutura *antigaRaiz) {
	Estrutura *novaRaiz = antigaRaiz->dir;
	Estrutura *aux = novaRaiz->esq;
	novaRaiz->esq = antigaRaiz;
	antigaRaiz->esq = aux;
	
	return novaRaiz;
}

Estrutura *rotacaoDuplaDir(Estrutura *antigaRaiz) {
	antigaRaiz->esq = rotacaoEsq(antigaRaiz->esq);
	
	return rotacaoDir(antigaRaiz);
}

Estrutura *rotacaoDuplaEsq(Estrutura *antigaRaiz) {
	antigaRaiz->dir = rotacaoDir(antigaRaiz->dir);
	
	return rotacaoEsq(antigaRaiz);
}

int fatorBalanc(Estrutura *no) {
	
	return no ? calculaAltura(no->esq) - calculaAltura(no->dir) : 0;
}

Estrutura *inserir(Estrutura *raiz, Registro dado) {
	if(!raiz) {
		raiz = (Estrutura *)malloc(sizeof(Estrutura));
		raiz->dado = dado;
		raiz->altura = 0;
		raiz->esq = NULL;
		raiz->dir = NULL;
	} else if(dado.cpf < raiz->dado.cpf) {
		raiz->esq = inserir(raiz->esq, dado);
		
		if(fatorBalanc(raiz) == 2) {
			if(dado.cpf < raiz->esq->dado.cpf) 
				raiz = rotacaoDir(raiz);
			else
				raiz = rotacaoDuplaDir(raiz);
		}
	} else if(dado.cpf > raiz->dado.cpf) {
		raiz->dir = inserir(raiz->dir, dado);
		if(fatorBalanc(raiz) == -2) {
			if(dado.cpf > raiz->dir->dado.cpf)
				raiz = rotacaoEsq(raiz);
			else
				raiz = rotacaoDuplaDir(raiz);
		}
	} else
		printf("ERRO! CPF já existe!");
	
	raiz->altura = calculaAltura(raiz);
	
	return raiz;
}

Estrutura *busca(Estrutura *raiz, unsigned long long cpf) {
	if(!raiz)
		return NULL;
	if(cpf < raiz->dado.cpf)
		return busca(raiz->esq, cpf);
	else if(cpf > raiz->dado.cpf)
		return busca(raiz->dir, cpf);
	else
		return raiz;
}

Estrutura *menor(Estrutura *no) {
	while (no->esq)
		no = no->esq;
	
	return no;
}

Estrutura *remover(Estrutura *raiz, unsigned long long cpf) {
	if(!raiz) {
		printf("CPF não escontrado!");
	
		return NULL;
	}
	
	if(cpf < raiz->dado.cpf)
		raiz->esq = remover(raiz->esq, cpf);
	else if(cpf > raiz->dado.cpf)
		raiz->dir = remover(raiz->dir, cpf);
	else {
		if (!raiz->esq || !raiz->dir) {
			Estrutura *aux = raiz->esq ? raiz->esq : raiz->dir;
			if(!aux) {
				aux = raiz;
				raiz = NULL;
			} else {
				*raiz = *aux;
			}
			
			free(aux);
		} else {
			Estrutura *aux = menor(raiz->dir);
			raiz->dado = aux->dado;
			raiz->dir = remover(raiz->dir, aux->dado.cpf);
		}
	}
	
	if(!raiz)
		return raiz;
		
	raiz->altura = calculaAltura(raiz);
	
	int balance = fatorBalanc(raiz);
	if(balance > 1 && fatorBalanc(raiz->esq) >= 0)
		return rotacaoDir(raiz);
	if(balance > 1 && fatorBalanc(raiz->esq) < 0)
		return rotacaoDuplaDir(raiz);
	if(balance < -1 && fatorBalanc(raiz->dir) <= 0)
		return rotacaoEsq(raiz);
	if(balance < -1 && fatorBalanc(raiz->dir) > 0)
		return rotacaoDuplaEsq(raiz);
	
	return raiz;
}

void update(Estrutura *raiz, unsigned long long cpf) {
	Estrutura *no = busca(raiz, cpf);
	
	if(no) {
		printf("Digite o novo nome: ");
		scanf(" %[^\n]", no->dado.nome);
		printf("Digite a nova data de nascimento (dd/mm/yyyy): ");
		scanf(" %[^\n]", no->dado.dataNasc);
		printf("Digite o novo endereço: ");
		scanf(" %[^\n]", no->dado.endereco);
		printf("Digite o novo telefone: ");
		scanf(" %[^\n]", no->dado.telefone);
	} else {
		printf("CPF não encontrado!\n");
	}
}

void read(Estrutura *raiz) {
	if (raiz) {
		read(raiz->esq);
		printf("CPF: %llu\n", raiz->dado.cpf);
		printf("Nome: %s\n", raiz->dado.nome);
		printf("Data de Nascimento: %s\n", raiz->dado.dataNasc);
		printf("Endereço: %s\n", raiz->dado.endereco);
		printf("Telefone: %s\n\n", raiz->dado.telefone);
		read(raiz->dir);
	}
}

int lerNumero() {
	int num;
	
	while (scanf("%d", &num) != 1) {
		printf("\n Opção Inválida! Digite um número: \n");
		
		while(getchar() != '\n');
	}
	
	return num;
}

int main(void) {
	setlocale(LC_ALL,"portuguese");
	
	Estrutura *raiz = NULL;
	int op;
		
	do {
		system("cls");
		printf("\n -------------------- MENU -------------------- \n");
		printf("1 - INSERIR REGISTRO\n");
		printf("2 - BUSCAR REGISTRO\n");
		printf("3 - DELETAR REGISTRO\n");
		printf("4 - ATUALIZAR REGISTRO\n");
		printf("5 - LISTAR REGISTROS\n");
		printf("0 - SAIR\n\n");
		printf("Digite a opção desejada: ");
		op = lerNumero();
		
		switch(op) {
			case 1:{
				printf("Inserir registro\n");
				break;
			}
			case 2:{
				printf("Buscar registro\n");
				break;
			}
			case 3:{
				printf("Deletar registro\n");
				break;
			}
			case 4:{
				printf("Atualizar registro\n");
				break;
			}
			case 5:{
				printf("Listar registros\n");
				break;
			}
			case 0:{
				printf("Sair\n");
				break;
			}
			default:{
				printf("Inválido!\n");
				break;
			}
		}
		
		if(op!=0) {
			printf("\n Digite '1' para voltar ao menu ou '0' para encerrar o programa.\n ");
			op = lerNumero();
		}
		
	} while(op != 0);
	
	return 0;
}
