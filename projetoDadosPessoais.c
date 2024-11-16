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

int calculaAltura(Estrutura *no) {
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
	antigaRaiz->dir = aux;
	
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

int validarCPF(unsigned long long cpf) {
	int digitos = 0;
	
	while(cpf) {
		cpf /= 10;
		digitos++;
	}
	
	return digitos == 11;
}

Estrutura *inserir(Estrutura *raiz, Registro dado) {
	if(!raiz) {
		raiz = (Estrutura *)malloc(sizeof(Estrutura));
		raiz->dado = dado;
		raiz->altura = 0;
		raiz->esq = NULL;
		raiz->dir = NULL;
	}else if(dado.cpf < raiz->dado.cpf) {
		raiz->esq = inserir(raiz->esq, dado);
		
		if(fatorBalanc(raiz) == 2) {
			if(dado.cpf < raiz->esq->dado.cpf) 
				raiz = rotacaoDir(raiz);
			else
				raiz = rotacaoDuplaDir(raiz);
		}
	}else if(dado.cpf > raiz->dado.cpf) {
		raiz->dir = inserir(raiz->dir, dado);
		if(fatorBalanc(raiz) == -2) {
			if(dado.cpf > raiz->dir->dado.cpf)
				raiz = rotacaoEsq(raiz);
			else
				raiz = rotacaoDuplaDir(raiz);
		}
	}else {
		printf("ERRO! O CPF %llu já existe!\n", dado.cpf);
	}
	
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
		printf("CPF %llu não encontrado!\n", cpf);
	
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
		printf("CPF %llu não encontrado!\n", cpf);
	}
}

void read(Estrutura *raiz) {
	if(raiz) {
		read(raiz->esq);
		printf("CPF: %llu\nNome: %s\nData de Nascimento: %s\nEndereço: %s\nTelefone: %s\n",
				raiz->dado.cpf, raiz->dado.nome, raiz->dado.dataNasc,
				raiz->dado.endereco, raiz->dado.telefone);
		read(raiz->dir);
	}
}

int lerNumero() {
	int num;
	
	while (scanf("%d", &num) != 1) {
		printf("\nOpção Inválida! Digite um número: \n");
		
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
				Registro novoRegistro;
				
				printf("Digite o CPF: ");
				scanf("%llu", &novoRegistro.cpf);
				if(!validarCPF(novoRegistro.cpf)) {
					printf("CPF inválido!\n");
					break;
				}
				printf("Digite o nome: ");
				scanf(" %[^\n]", novoRegistro.nome);
				printf("Digite a data de nascimento (dd/mm/yyyy): ");
				scanf(" %[^\n]", novoRegistro.dataNasc);
				printf("Digite o endereço: ");
				scanf(" %[^\n]", novoRegistro.endereco);
				printf("Digite o telefone: ");
				scanf(" %[^\n]", novoRegistro.telefone);
				
				raiz = inserir(raiz, novoRegistro);
				break;
			}
			case 2:{
				unsigned long long cpf;
				
				printf("Digite o CPF para buscar: ");
				while (getchar() != '\n');
				scanf("&llu", &cpf);
				
				Estrutura *no = busca(raiz, cpf);
				
				if(no) {
					printf("\nRegistro encontrado:\n");
			        printf("CPF: %llu\n", no->dado.cpf);
			        printf("Nome: %s\n", no->dado.nome);
			        printf("Data de Nascimento: %s\n", no->dado.dataNasc);
			        printf("Endereço: %s\n", no->dado.endereco);
			        printf("Telefone: %s\n", no->dado.telefone);
				}else {
					printf("\nCPF %llu não encontrado!\n", cpf);
				}
				break;
			}
			case 3:{
				unsigned long long cpf;
				
				printf("Digite o CPF para deletar: ");
				scanf("%llu", &cpf);
				
				raiz = remover(raiz, cpf);
				break;
			}
			case 4:{
				unsigned long long cpf;
				
				printf("Digite o CPF para atualizar: ");
				scanf("%llu", &cpf);
				
				update(raiz, cpf);
				break;
			}
			case 5:{
				printf("Registros cadastrados:\n");
				
				read(raiz);
				break;
			}
			case 0:{
				printf("Saindo...\n");
				break;
			}
			default:{
				printf("Opção inválida!\n");
				break;
			}
		}
		
		if(op!=0) {
			printf("\nDigite '1' para voltar ao menu ou '0' para encerrar o programa.\n ");
			op = lerNumero();
		}
		
	} while(op != 0);
	
	return 0;
}
