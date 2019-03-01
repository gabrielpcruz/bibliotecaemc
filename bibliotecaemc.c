/*                                            
*	AUTHOR: GABRIEL PEREIRA DA CRUZ --- E-MAIL:gabriel.cruz116@gmail.com 	   --- WHATSAPP:(61)9 9583-0718
*	CURSO: SISTEMAS DE INFORMAÇÃO   --- IES: UNIVERSIDADE CATÓLICA DE BRASÍLIA --- UCB
*	SEMESTRE: 4° SEMESTRE 1/2017    --- DATA DA ÚLTIMA MODIFICAÇÃO:17/05/2017
*
*	OBJETIVO: CONSTRUIR UM SOFTWARE DE GESTÃO DE UMA BIBLIOTECA
*
*	FUNÇÕES: CADASTRAR.: LIVROS - LEITORES - | LISTAR.: LIVROS - LEITORES | REALIZAR E CONSULTAR EMPRÉSTIMOS
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> //Biblioteca para configurar os caracteres latinos

/* Ajuste das funçôes específicas aos OS */
#ifdef __unix__ 
#define LIMPA_TELA system("clear")

#elif defined (_WIN32) || defined (WIN32) 
#define LIMPA_TELA system("cls")
#endif
#define PAUSE system("pause")

#define TS 30

//Estrutras De Leitores
typedef struct Leitor{
	char nome[TS];
	int matricula;
	char deletado;
}Leitor;

//Estrutras De Livros
typedef struct Livro {
	char nome[TS];
	int codLivro;
	char deletado;
}Livro;

//Estrutras De Empréstimos
typedef struct Emprestimo{
	int codigo;
	int codigoLivro;
	int matricula;
	char success;
	int adress[2];
}Emprestimo;

//Estrutras Para Contar 
typedef struct Cont{
	int mat;
	int codLiv;
	int codEmp;
	int situacao;
	int flag;
}Cont;

//Declaração de Protótipos das Funções
//====================================================================================================================//

// Desenhar Cabeçalho Do Sistema
void desenhaCabecalho(void);

//Desenha Subcabeçalhos do Sistema
void imprimeSubCabecalho(int sessao);

//Cadastrar Leitor
struct Leitor recolheDadosLeitor(int matricula);
struct Leitor cadastraLeitor( FILE** listaDeleitores,  struct Leitor dadosLeitor);

//Cadastrar Livro
struct Livro recolheDadosLivro(int codLivro);
struct Livro cadastraLivro( FILE** listaDelivros,  struct Livro dadosLivro);

//Realizar Emprestimo
struct Emprestimo recolheDadosEmprestimo(int codEmp);
void retornaNome(int matricula, int codLivro, int);
struct Emprestimo realizarEmprestimo(FILE** , struct Emprestimo dadosEmprestimo);

//Listar Livros
int listarLivros();

//Listar Leitores
int listarLeitores(int op);

//Abre/Fecha Arquivo
int abreArquivo(FILE** arquivo, char* nomeDoArquivo, char* modo);
void fechaArquivo(FILE** arquivo);

//Realizar Consulta
void realizarConsulta(int indice, FILE** listaAtual);

//Realizar Alteração
void realizarAlteracao(int indice, FILE** listaAtual);

//Realizar Alteração
void excluiRestaura(int indice, FILE** listaAtual, int op);

//Limpa Lixeira
void limpaLixeira(void);

//getsize
int get_size(const char* file_name);

//Atualiza Contadores
int atualizaContadores(int, int, int, int);

//====================================================================================================================//

int main(void){
	
	
	FILE *listaDeLeitores, *listaDeLivros, *listaDeEmprestimos, *cont;
	struct Cont contador;
	int situacao;
	
	int codEmp = 0, matricula = 0, codLivro = 0, flag = 0,  op , opcao;
	
	situacao = abreArquivo(&cont, "cont/cont.dat", "r+b");
	
	if(situacao == 1){
		fread(&contador, sizeof(struct Cont), 1, cont);
		codEmp = contador.codEmp;
		matricula = contador.mat;
		codLivro = contador.codLiv;
		flag = contador.flag;
		fechaArquivo(&cont);
	}
	
	if(situacao == 0){
		abreArquivo(&cont, "cont/cont.dat", "wb");
		codEmp=1; 
		matricula=1; 
		codLivro=1;
		flag = 1;
		contador.codEmp = codEmp;
		contador.mat = matricula;
		contador.codLiv = codLivro;
		contador.flag = 0;
		fwrite(&contador, sizeof(struct Cont), 1, cont);
		fechaArquivo(&cont);
	}
	
	if(flag == 0){
		codEmp++;
		matricula++;
		codLivro++;
	}
	
	
	//Declaração de variáveis Personalizadas
	struct Leitor 	  leitorAtual	 ;
	struct Livro  	  livroAtual	 ;
	struct Emprestimo emprestimoAtual;
	
	//Configuração de Caracteres para 'UTF-8' (Permite o usuo de acentos no programa)
	setlocale(LC_ALL, "");
	
	//Início do Ciclo DoWhile do Programa
	do{	
		LIMPA_TELA;//limpa a Tela
		desenhaCabecalho();//Chama a função que desenha o cabeçalho
		fflush(stdin);
		printf("1-CADASTRAR LEITOR    3-LISTAR LEITORES     5-REALIZAR EMPRESTIMO     7-ALTERA   9-SAIR\n");
		printf("2-CADASTRAR LIVRO     4-LISTAR LIVROS       6-REALIZAR CONSULTA       8-EXCLUI   10-LIXEIRA\n\nOPCAO:");
		scanf("%i",&op);
		
		
		if(op==9)break;//Analisa se o usuário deseja finalizar o programa.
		
		else{// Caso não deseje, dá procedimento ao programa
		
			switch(op){
				case 1://Cadastra Leitor
					
					LIMPA_TELA;//limpa a Tela
					
					leitorAtual = cadastraLeitor(&listaDeLeitores, recolheDadosLeitor(matricula));
					
					if(leitorAtual.matricula == 0){
						printf(" TEMOS UM NÚMERO MUITO GRANDE DE LEITORES!\nPOR FAVOR, VOLTE MÊS QUE VEM!\n\n");
					}else{
						desenhaCabecalho();//Chama a função que desenha o cabeçalho
						imprimeSubCabecalho(1);
						matricula++;//Incrementa o numero da matricula
						printf(" LEITOR CADASTRADO COM SUCESSO!\n\n");
						imprimeSubCabecalho(3);
						printf(" %-30s %9.09i\n\n\n",leitorAtual.nome, leitorAtual.matricula);
						system("pause"); 
					}
										
				break;
					
				case 2://Cadastra Livro
					LIMPA_TELA;
					
					livroAtual = cadastraLivro(&listaDeLivros, recolheDadosLivro(codLivro));
					
					if(livroAtual.codLivro == 0){
						printf(" ESTOQUE MUITO CHEIO!\nPOR FAVOR, ESPERE AUMENTAR-MOS NOSSA ESTRUTURA!");
						PAUSE;
					}else{
						desenhaCabecalho();//Chama a função que desenha o cabeçalho
						imprimeSubCabecalho(4);
						codLivro++;
						printf(" LIVRO CADASTRADO COM SUCESSO!\n\n");
						imprimeSubCabecalho(6);
						printf(" %-30s %9.09i\n\n\n", livroAtual.nome, livroAtual.codLivro);
						PAUSE;	
					} 
					
				break;
					
				case 3://Lista Leitores
					LIMPA_TELA;
					desenhaCabecalho();
						if(listarLeitores(1) == 0){
							imprimeSubCabecalho(12);
							PAUSE;					
						}else{
							PAUSE;
						}
				break;
			
				case 4://Lista Livros
						LIMPA_TELA;
						desenhaCabecalho();
						if(listarLivros(1) == 0){
							imprimeSubCabecalho(13);
							PAUSE;					
						}else{
							PAUSE;
						}					
				break;
				
				case 5://Realiza Empréstimo
					LIMPA_TELA;
					if(abreArquivo(&listaDeLeitores, "leitores/listaDeLeitores.dat", "rb") == 0 || abreArquivo(&listaDeLivros, "livros/listaDeLivros.dat", "rb") == 0){
						desenhaCabecalho();//Chama a função que desenha o cabeçalho
						printf("---ERRO: IMPOSSÍVEL REALIZAR EMPRÉSTIMO---\n\n\n");
						printf(" O ERRO PODE SER CAUSADO POR:\n\n");
						printf("   1)FALTA DE ALUNOS CONSTANDO NA BASE DE DADOS\n\n");
						printf("   2)FALTA DE LIVROS CONSTANDO NA BASE DE DADOS\n\n");
						printf(" OBS: CONSERTE O ERRO, E TENTE NOVAMENTE!\n");
						printf("\n\n");
						PAUSE;				
					}else{
						fechaArquivo(&listaDeLeitores);
						fechaArquivo(&listaDeLivros);
						emprestimoAtual = realizarEmprestimo(&listaDeEmprestimos, recolheDadosEmprestimo(codEmp));
						
						if(emprestimoAtual.success == '1' || emprestimoAtual.success == '2'){
							LIMPA_TELA;
							desenhaCabecalho();//Chama a função que desenha o cabeçalho
							imprimeSubCabecalho(7);
							printf("---ERRO: IMPOSSÍVEL REALIZAR EMPRÉSTIMO---\n\n\n");
							if(emprestimoAtual.success == '1')
								printf(" O LIVRO INFORMADO, NÃO CONSTA MAIS NA BASE DE DADOS");
							if(emprestimoAtual.success == '2')
								printf(" O LEITOR INFORMADO, NÃO CONSTA MAIS NA BASE DE DADOS");
							printf("\n\n");
							PAUSE;
							fflush(stdin);
							emprestimoAtual.success = '*';
						}else{
							LIMPA_TELA;
							codEmp++;
							desenhaCabecalho();//Chama a função que desenha o cabeçalho
							imprimeSubCabecalho(7);
							printf(" EMPRÉSTIMO REALIZADO COM SUCESSO!\n\n");
							PAUSE;	
						}							
					}
				break;
				
				case 6://Realizar Consulta
					do{
						LIMPA_TELA;
						desenhaCabecalho();
						imprimeSubCabecalho(9);
						printf("1-CONSULTAR LIVRO    2-CONSULTAR LEITOR    3-CONSULTAR EMPRESTIMO    4-CANCELAR OPERAÇÃO\n\nOPCAO:");
						fflush(stdin);
						scanf("%i", &opcao);
						if(opcao == 4) break;
						switch(opcao){
							case 1:
								if(abreArquivo(&listaDeLivros, "livros/listaDeLivros.dat", "rb") == 1){
									realizarConsulta(opcao, &listaDeLivros);
									PAUSE;
								}else{
									printf("erro");
									PAUSE;
								}
							break;
								
							case 2:
								if(abreArquivo(&listaDeLeitores, "leitores/listaDeLeitores.dat", "rb") == 1){
									realizarConsulta(opcao, &listaDeLeitores);
									PAUSE;
								}else{
									printf("erro");
									PAUSE;
								}	
							break;
							
							case 3:
								if(abreArquivo(&listaDeEmprestimos, "emprestimos/listaDeEmprestimos.dat", "rb") == 1){
									realizarConsulta(opcao, &listaDeEmprestimos);
									PAUSE;
								}else{
									printf("erro");
									PAUSE;
								}					
							break;
							
							case 4:
								
							break;
							
							default:
								LIMPA_TELA;
								desenhaCabecalho();
								imprimeSubCabecalho(9);
								printf("OPÇÃO INVÁLIDA. TENTE NOVAMENTE\n\n");//CASO O O USUÁRIO DIGITE UM NUMERO DIFERENTES DOS PRÉ-DEFINIDOS.
								PAUSE;
						
						}
					}while(opcao != 4);
					

				break;	
					
				case 7://Realizar Alteração
					do{
						LIMPA_TELA;
						desenhaCabecalho();
						printf("1-ALTERAR LIVRO    2-ALTERAR LEITOR    3-CANCELAR OPERAÇÃO\n\nOPCAO:");
						fflush(stdin);
						scanf("%i", &opcao);
						if(opcao == 3)break;
						switch(opcao){
							case 1:
								if(abreArquivo(&listaDeLivros, "livros/listaDeLivros.dat", "r+b") == 1){
									realizarAlteracao(1, &listaDeLivros);
									PAUSE;
								}else{
									printf("erro");
									PAUSE;
								}
							break;
								
							case 2:
								if(abreArquivo(&listaDeLeitores, "leitores/listaDeLeitores.dat", "r+b") == 1){
									realizarAlteracao(2, &listaDeLeitores);
									PAUSE;
								}else{
									printf("erro");
									PAUSE;
								}	
							break;
							
							case 3:
													
							break;
							
							default:
								LIMPA_TELA;
								desenhaCabecalho();
								imprimeSubCabecalho(10);
								printf("OPÇÃO INVÁLIDA. TENTE NOVAMENTE\n\n");//CASO O O USUÁRIO DIGITE UM NUMERO DIFERENTES DOS PRÉ-DEFINIDOS.
								PAUSE;
						}
					}while(opcao != 3);							
				break;	
				
				case 8:
					do{
						LIMPA_TELA;
						desenhaCabecalho();
						imprimeSubCabecalho(11);
						printf("1-EXCLUIR LIVRO    2-EXCLUIR LEITOR    3-CANCELAR OPERAÇÃO\n\nOPCAO:");
						fflush(stdin);
						scanf("%i", &opcao);
						switch(opcao){
							case 1:
								if(abreArquivo(&listaDeLivros, "livros/listaDeLivros.dat", "r+b") == 1){
									excluiRestaura(1, &listaDeLivros, 0);
									PAUSE;
								}else{
									printf("erro");
									PAUSE;
								}
							break;
								
							case 2:
								if(abreArquivo(&listaDeLeitores, "leitores/listaDeLeitores.dat", "r+b") == 1){
									excluiRestaura(2, &listaDeLeitores, 0);
									PAUSE;
								}else{
									printf("erro");
									PAUSE;
								}	
							break;
							
							case 3:
													
							break;
							
							default:
								LIMPA_TELA;
								desenhaCabecalho();
								imprimeSubCabecalho(11);
								printf("OPÇÃO INVÁLIDA. TENTE NOVAMENTE\n\n");//CASO O O USUÁRIO DIGITE UM NUMERO DIFERENTES DOS PRÉ-DEFINIDOS.
								PAUSE;
							
						}
					}while(opcao != 3);
				break;	
				
				case 10:
					do{
						LIMPA_TELA;
						desenhaCabecalho();
						imprimeSubCabecalho(14);
						printf("1-RESTAURAR LIVRO    2-RESTAURAR LEITOR    3-LIMPAR LIXEIRA    4-CANCELAR OPERAÇÃO\n\nOPCAO:");
						fflush(stdin);
						scanf("%i", &opcao);
						switch(opcao){
							case 1:
								if(abreArquivo(&listaDeLivros, "livros/listaDeLivros.dat", "r+b") == 1){
									excluiRestaura(1, &listaDeLivros, 1);
									PAUSE;
								}else{
									printf("erro");
									PAUSE;
								}
							break;
								
							case 2:
								if(abreArquivo(&listaDeLeitores, "leitores/listaDeLeitores.dat", "r+b") == 1){
									excluiRestaura(2, &listaDeLeitores, 1);
									PAUSE;
								}else{
									printf("erro");
									PAUSE;
								}	
							break;
							
							case 3:
								LIMPA_TELA;
								desenhaCabecalho();
								imprimeSubCabecalho(11);
								printf("Limpando Lixeira...\n\n");
								limpaLixeira();
								PAUSE;
							break;
							
							case 4:
								
							break;
							
							default:
								LIMPA_TELA;
								desenhaCabecalho();
								imprimeSubCabecalho(11);
								printf("OPÇÃO INVÁLIDA. TENTE NOVAMENTE\n\n");//CASO O O USUÁRIO DIGITE UM NUMERO DIFERENTES DOS PRÉ-DEFINIDOS.
								PAUSE;
							
						}
					}while(opcao != 4);
				break;	
				
				default:
					LIMPA_TELA;
					printf("OPÇÃO INVÁLIDA. TENTE NOVAMENTE\n");//CASO O O USUÁRIO DIGITE UM NUMERO DIFERENTES DOS PRÉ-DEFINIDOS.
					PAUSE;
				break;
			}	
		}
	}while(op!=9);	
	
	
	situacao = abreArquivo(&cont, "cont/cont.dat", "wb");
	fechaArquivo(&cont);
	if(situacao == 0){
		printf("Falha na numeração!");
		exit(1);
	}
	
	if(situacao == 1){
		flag = 1;
		situacao = atualizaContadores(codEmp, matricula , codLivro, flag);
		if(situacao == 0){
			printf("Falha grave no Sistema");
		}
	}
	
	return 0;
}

int atualizaContadores(int codEmp, int matricula, int codLivro, int flag){
	
	struct Cont dadosAtualizados;
	FILE *arquivo;
	
	dadosAtualizados.codEmp = codEmp;
	dadosAtualizados.mat = matricula;
	dadosAtualizados.codLiv = codLivro;
	dadosAtualizados.flag = flag;
	
	arquivo = fopen("cont/cont.dat", "wb");
	
	if (arquivo == NULL) {
		return 0;
	}
	
	fwrite(&dadosAtualizados, sizeof(struct Cont), 1, arquivo);
	
	fclose(arquivo);
	
	return 1;
}

void desenhaCabecalho(void){// FUNÇÃO PARA DESENHAR O CABEÇALHO DO SISTEMA.
	printf("--------------------------------------------------------------------------------------------------\n");
	printf("|                                     SISTEMA DA BIBLIOTECA                                      |\n");
	printf("--------------------------------------------------------------------------------------------------\n\n");
}

struct Leitor recolheDadosLeitor(int matricula){

	struct Leitor leitor;
	desenhaCabecalho();//Chama a função que desenha o cabeçalho

	imprimeSubCabecalho(1);
	printf(" DIGITE O NOME DO LEITOR:");
	fflush(stdin);//Limpa o buffer do teclado
	fgets(leitor.nome,TS,stdin);//FAZ A LEITURA DE UMA STRING DIGITADA PELO USUÁRIO.
	leitor.nome[strlen(leitor.nome) - 1] = '\0';
	leitor.matricula = matricula;
	LIMPA_TELA;	
	return leitor;
}

struct Leitor cadastraLeitor(FILE** listaDeLeitores, struct Leitor dadosLeitor){
	
	*listaDeLeitores = fopen("leitores/ListaDeLeitores.dat", "ab");
	
	if (*listaDeLeitores == NULL) {
		dadosLeitor.matricula = 0;
		return dadosLeitor;
	}
	
	dadosLeitor.deletado = ' '; // NAO DELETADO
	
	fwrite(&dadosLeitor, sizeof(struct Leitor), 1, *listaDeLeitores);
	
	fclose(*listaDeLeitores);
	
	return dadosLeitor;

}

struct Livro recolheDadosLivro(int codLivro){
	
	struct Livro dadosLivro;
	fflush(stdin);//Limpa o buffer do teclado
	desenhaCabecalho();//Chama a função que desenha o cabeçalho

	imprimeSubCabecalho(4);
	printf(" DIGITE O NOME DO LIVRO:");
	fflush(stdin);//Limpa o buffer do teclado
	fgets(dadosLivro.nome,TS,stdin);//FAZ A LEITURA DE UMA STRING DIGITADA PELO USUÁRIO.
	dadosLivro.nome[strlen(dadosLivro.nome) - 1] = '\0';
	dadosLivro.codLivro = codLivro;
	LIMPA_TELA;	
	return dadosLivro;
}

struct Livro cadastraLivro (FILE** listaDeLivros,  struct Livro dadosLivro){
	
	*listaDeLivros = fopen("livros/ListaDeLivros.dat", "ab");
	
	if (*listaDeLivros == NULL) {
		dadosLivro.codLivro = 0;
		return dadosLivro;
	}
	
	dadosLivro.deletado = ' '; // NAO DELETADO
	
	fwrite(&dadosLivro, sizeof(struct Livro), 1, *listaDeLivros);
	
	fclose(*listaDeLivros);
	
	return dadosLivro;
	
}

int abreArquivo(FILE** arquivo, char* nomeDoArquivo, char* modo){
	*arquivo = fopen(nomeDoArquivo, modo);
	if(*arquivo == NULL)
		return 0;
	else
		return 1;
}

void fechaArquivo(FILE** arquivo){
	fclose(*arquivo);
}

void retornaNome(int matricula, int codLivro, int op){

	fflush(stdin);//Limpa o buffer do teclado
	
	FILE* listaDeLeitores, *listaDeLivros;
	
	//Declaração de ponteiros para auxiliar na tarefa
	struct Livro  nomeLivroAtual;
	struct Leitor nomeLeitorAtual;
	
	switch(op){
		case 1:
			abreArquivo(&listaDeLivros, "livros/listaDeLivros.dat", "rb");
			while(fread(&nomeLivroAtual, sizeof(struct Livro), 1, listaDeLivros)){
				if(nomeLivroAtual.codLivro == codLivro){
					if(nomeLivroAtual.deletado == ' ')//Verifica se está deletado
						printf("%30s", nomeLivroAtual.nome);
				}
			}
			fechaArquivo(&listaDeLivros);
			
		break;
		
		case 2:
			abreArquivo(&listaDeLeitores, "leitores/listaDeLeitores.dat", "rb");
			while(fread(&nomeLeitorAtual, sizeof(struct Leitor), 1, listaDeLeitores)){
				if(nomeLeitorAtual.matricula == matricula){
					if(nomeLeitorAtual.deletado == ' ')//Verifica se está deletado
						printf("%30s", nomeLeitorAtual.nome);
				}
			}
			fechaArquivo(&listaDeLeitores);
			
		break;
	}
}

struct Emprestimo recolheDadosEmprestimo(int codEmp){

	struct Emprestimo dadosEmprestimo;
	fflush(stdin);//Limpa o buffer do teclado.
	desenhaCabecalho();//Chama a função que desenha o cabeçalho
	
	imprimeSubCabecalho(7);
	printf(" DIGITE SUA MATRÍCULA: ");
	scanf("%i",&dadosEmprestimo.matricula);	
	
	if(dadosEmprestimo.success == '2')
		return dadosEmprestimo;
	
	LIMPA_TELA;
	desenhaCabecalho();
	listarLivros(1);
	imprimeSubCabecalho(7);
	printf(" DIGITE O CODIGO DO LIVRO: ");
	scanf("%i",&dadosEmprestimo.codigoLivro);
	
	if(dadosEmprestimo.success == '1')
		return dadosEmprestimo;
	
	dadosEmprestimo.codigo = codEmp;
	
	return dadosEmprestimo;
}

struct Emprestimo realizarEmprestimo(FILE** listaDeEmprestimos, struct Emprestimo dadosEmprestimo){
	
	if(dadosEmprestimo.success == '1' || dadosEmprestimo.success == '2')
		return dadosEmprestimo;
	else{
		*listaDeEmprestimos = fopen("emprestimos/listaDeEmprestimos.dat", "ab");
		
		if (*listaDeEmprestimos == NULL) {
			dadosEmprestimo.codigo = 0;
			return dadosEmprestimo;
		}
		
		fwrite(&dadosEmprestimo, sizeof(struct Emprestimo), 1, *listaDeEmprestimos);
		
		fclose(*listaDeEmprestimos);
		
		return dadosEmprestimo;
	}	
}

int listarLivros(int op){//Função para listar os livros

	struct Livro livroAtual;
	FILE *arquivo;
	
	arquivo = fopen("livros/listaDeLivros.dat", "rb");
	
	if ((arquivo == NULL) || (get_size("livros/listaDeLivros.dat") == 0)) {
		return 0;
	}
	//desenhaCabecalho();
	imprimeSubCabecalho(5);
	imprimeSubCabecalho(6);
	
	if(op == 1){
		while (fread(&livroAtual, sizeof(struct Livro), 1, arquivo)){
			if(livroAtual.deletado== ' '){//Verifica se está deletado
				printf(" %-30s %9.09i\n", livroAtual.nome, livroAtual.codLivro);
			}
		}
	}
	
	if(op == 2){
		while (fread(&livroAtual, sizeof(struct Livro), 1, arquivo)){
			if(livroAtual.deletado== '*'){//Verifica se está deletado
				printf(" %-29s* %9.09i\n", livroAtual.nome, livroAtual.codLivro);
			}
		}
	}
	
	printf("\n");
	fclose(arquivo);
	
	return 1;

}

int listarLeitores(int op){//Função para listar os livros

	struct Leitor leitorAtual;
	FILE *arquivo;
	
	arquivo = fopen("leitores/listaDeLeitores.dat", "rb");
	
	if (arquivo == NULL || (get_size("leitores/listaDeLeitores.dat") == 0)) {
		return 0;
	}
	//desenhaCabecalho();
	imprimeSubCabecalho(2);
	imprimeSubCabecalho(3);
	if(op == 1){
		while (fread(&leitorAtual, sizeof(struct Leitor), 1, arquivo)){
			if(leitorAtual.deletado== ' '){//Verifica se está deletado
				printf(" %-30s %9.09i\n", leitorAtual.nome, leitorAtual.matricula);
			}
		}
	}
	
	if(op == 2){
		while (fread(&leitorAtual, sizeof(struct Leitor), 1, arquivo)){
			if(leitorAtual.deletado== '*'){//Verifica se está deletado
				printf(" %-29s* %9.09i\n", leitorAtual.nome, leitorAtual.matricula);
			}
		}
	}
	
	printf("\n");
	fclose(arquivo);
	
	return 1;

}

void realizarConsulta(int indice, FILE** listaAtual){
	
	//Cria um ponteiros para auxiliar na tarefa
	struct Emprestimo emprestimoAtual;
	struct Livro      livroAtual;
	struct Leitor     leitorAtual;
	
	//Cria um auxiliar de busca
	int procurado, achei;
	
	LIMPA_TELA;
	switch(indice){
		case 1:
			desenhaCabecalho();
			imprimeSubCabecalho(9);
			printf(" DIGITE O CODIGO DO LIVRO: ");
			scanf("%i",&procurado);
			while(fread(&livroAtual, sizeof(struct Livro), 1, *listaAtual)){
				if(livroAtual.codLivro == procurado && livroAtual.deletado == ' '){
					if(achei == 0)imprimeSubCabecalho(6);
					printf(" %-30s %9.09i\n", livroAtual.nome, livroAtual.codLivro);
					achei = 1;
				}	
			}
			
			if(!achei)
				printf("\n\n CÓDIGO NÃO ENCONTRADO!\n\n");
			else
				printf("\n\n");
				
			fechaArquivo(&(*listaAtual));
			
		break;
		
		case 2:
			desenhaCabecalho();
			imprimeSubCabecalho(9);
			printf(" DIGITE A MATRÍCULA: ");
			scanf("%i",&procurado);
			while(fread(&leitorAtual, sizeof(struct Leitor), 1, *listaAtual)){
				if(leitorAtual.matricula == procurado && leitorAtual.deletado == ' '){
					if(achei == 0)imprimeSubCabecalho(3);
					printf(" %-30s %9.09i\n", leitorAtual.nome, leitorAtual.matricula);
					achei = 1;
				}	
			}
			
			if(!achei)
				printf("\n\n CÓDIGO NÃO ENCONTRADO!\n\n");
			else
				printf("\n\n");
					
			fechaArquivo(&(*listaAtual));
			
		break;
		
		case 3:
			desenhaCabecalho();
			imprimeSubCabecalho(9);
			printf(" DIGITE SUA MATRÍCULA: ");
			scanf("%i",&procurado);
			while(fread(&emprestimoAtual, sizeof(struct Emprestimo), 1, *listaAtual) ){
				if(emprestimoAtual.matricula == procurado ){
					if(achei == 0){
						printf("\n-------------------------------CONSULTAR EMPRESTIMO-----------------------------------\n");
						imprimeSubCabecalho(8);
					}
					printf(" %3.03i ", emprestimoAtual.codigo);
					retornaNome(emprestimoAtual.matricula,  emprestimoAtual.codigoLivro, 2);
					printf(" %9.09i ", emprestimoAtual.matricula);
					retornaNome(emprestimoAtual.matricula,  emprestimoAtual.codigoLivro, 1);
					printf(" %9.09i \n", emprestimoAtual.codigoLivro);
					achei = 1;
				}
			}
			
			if(!achei)
				printf("\n\n CÓDIGO NÃO ENCONTRADO!\n\n");
			else
				printf("\n\n");
					
			fechaArquivo(&(*listaAtual));
			
		break;
	}
}

void realizarAlteracao(int indice, FILE** listaAtual){
	
	//Cria um ponteiros para auxiliar na tarefa
	struct Livro      livroAtual;
	struct Leitor     leitorAtual;
	
	//Cria um auxiliar de busca
	int procurado, achei;

	LIMPA_TELA;
	switch(indice){
		case 1:
			desenhaCabecalho();
			printf("ALTERAÇÃO DE LIVRO\n");
			printf("-------------------\n\n");
			printf(" DIGITE O CODIGO DO LIVRO: ");
			scanf("%i",&procurado);
			LIMPA_TELA;
			while(fread(&livroAtual, sizeof(struct Livro), 1, *listaAtual)){
				if(livroAtual.codLivro == procurado && livroAtual.deletado == ' '){
					desenhaCabecalho();//Chama a função que desenha o cabeçalho
					printf("ALTERAÇÃO DE LIVRO\n");
					printf("-------------------\n\n");
					achei = 1;
					imprimeSubCabecalho(3);
					printf(" %-30s %9.09i\n\n", livroAtual.nome, livroAtual.codLivro);
					printf(" DIGITE O NOVO NOME DO LIVRO:");
					fflush(stdin);//Limpa o buffer do teclado
					fgets(livroAtual.nome,TS,stdin);//FAZ A LEITURA DE UMA STRING DIGITADA PELO USUÁRIO.
					livroAtual.nome[strlen(livroAtual.nome) - 1] = '\0';
					
					fseek(*listaAtual, ((int)(sizeof(livroAtual) * (-1))), SEEK_CUR);
					fwrite(&livroAtual, sizeof(struct Livro), 1, *listaAtual);
					fseek(*listaAtual, 0, SEEK_CUR);
					
				}	
			}
			printf("\n\n");
			
			if(!achei)
				printf("CÓDIGO NÃO ENCONTRADO!");
				
			fclose(*listaAtual);
			
		break;
		
		case 2:
			desenhaCabecalho();
			printf("ALTERAÇÃO DE LEITOR\n");
			printf("------------------\n\n");
			printf(" DIGITE A MATRÍCULA DO LEITOR: ");
			scanf("%i",&procurado);
			LIMPA_TELA;
			while(fread(&leitorAtual, sizeof(struct Leitor), 1, *listaAtual)){
				if(leitorAtual.matricula == procurado && leitorAtual.deletado == ' '){
					desenhaCabecalho();//Chama a função que desenha o cabeçalho
					printf("ALTERAÇÃO DE LEITOR\n");
					printf("------------------\n\n");
					achei = 1;
					imprimeSubCabecalho(3);
					printf(" %-30s %9.09i\n\n", leitorAtual.nome, leitorAtual.matricula);
					printf(" DIGITE O NOVO NOME DO LEITOR:");
					fflush(stdin);//Limpa o buffer do teclado
					fgets(leitorAtual.nome,TS,stdin);//FAZ A LEITURA DE UMA STRING DIGITADA PELO USUÁRIO.
					leitorAtual.nome[strlen(leitorAtual.nome) - 1] = '\0';
						
					fseek(*listaAtual, ((int)(sizeof(leitorAtual) * -1)), SEEK_CUR);//TA DANDO WARNING(QUERO SABER POQUE)
					fwrite(&leitorAtual, sizeof(struct Leitor), 1, *listaAtual);
					fseek(*listaAtual, 0, SEEK_CUR);
				}	
			}
			printf("\n\n");	
			
			if(!achei)
				printf("CÓDIGO NÃO ENCONTRADO!");
				
			fclose(*listaAtual);
			
		break;

	}
}

void excluiRestaura(int indice, FILE** listaAtual, int op){
	
	//Cria um ponteiros para auxiliar na tarefa
	struct Livro      livroAtual;
	struct Leitor     leitorAtual;
	
	//Cria um auxiliar de busca
	int procurado, achei;
	
	LIMPA_TELA;
	switch(indice){
		case 1:
			desenhaCabecalho();
			if(op==0){
				printf("EXCLUSÃO DE LIVRO\n");
				printf("-----------------\n\n");
				listarLivros(1);
			}
			if(op==1){
				printf("RESTAURARAÇÃO DE LIVRO\n");
				printf("----------------------\n\n");
				listarLivros(2);
			}
			printf(" DIGITE O CODIGO DO LIVRO: ");
			scanf("%i",&procurado);
			LIMPA_TELA;
			while(fread(&livroAtual, sizeof(struct Livro), 1, *listaAtual)){
				if(livroAtual.codLivro == procurado){
					desenhaCabecalho();//Chama a função que desenha o cabeçalho
					if(op==0)
					printf("EXCLUSÃO DE LIVRO\n");
					if(op==1)
					printf("RESTAURARAÇÃO DE LIVRO\n");
					printf("-----------------------\n\n");
					achei = 1;
					
					if(livroAtual.deletado == ' ' && op == 0)
					livroAtual.deletado = '*';
					if(livroAtual.deletado == '*' && op == 1)
					livroAtual.deletado = ' ';
					
					fseek(*listaAtual, ((int)(sizeof(livroAtual) * (-1))), SEEK_CUR);//TA DANDO WARNING(QUERO SABER POQUE)
					fwrite(&livroAtual, sizeof(struct Livro), 1, *listaAtual);
					fseek(*listaAtual, 0, SEEK_CUR);
				}	
			}
			
			if(!achei)
				printf("CÓDIGO NÃO ENCONTRADO!");
			else
				printf(" LIVRO MOVIDO PARA LIXEIRA!\n É POSSÍVEL RESTAURÁ-LO DEPOIS.\n\n");
				
			fclose(*listaAtual);
			
		break;
		
		case 2:
			desenhaCabecalho();
			if(op==0){
				printf("EXCLUSÃO DE LEITORES\n");
				printf("--------------------\n\n");
				listarLeitores(1);
			}
			if(op==1){
				printf("RESTAURARAÇÃO DE LEITORES\n");
				printf("-------------------------\n\n");
				listarLeitores(2);
			}
			printf(" DIGITE A MATRÍCULA DO LEITOR: ");
			scanf("%i",&procurado);
			LIMPA_TELA;
			while(fread(&leitorAtual, sizeof(struct Leitor), 1, *listaAtual)){
				if(leitorAtual.matricula == procurado){
					desenhaCabecalho();//Chama a função que desenha o cabeçalho
					if(op==0)
					printf("EXCLUSÃO DE LEITOR\n");
					if(op==1)
					printf("RESTAURARAÇÃO DE LEITOR\n");
					printf("-----------------------\n\n");
					
					achei = 1;
					
					if(leitorAtual.deletado == ' ' && op == 0)
					leitorAtual.deletado = '*';
					if(leitorAtual.deletado == '*' && op == 1)
					leitorAtual.deletado = ' ';
					
					fseek(*listaAtual, ((int)(sizeof(leitorAtual) * -1)), SEEK_CUR);//TA DANDO WARNING(QUERO SABER POQUE)
					fwrite(&leitorAtual, sizeof(struct Leitor), 1, *listaAtual);
					fseek(*listaAtual, 0, SEEK_CUR);
				}	
			}
			
			if(!achei)
				printf("CÓDIGO NÃO ENCONTRADO!");
			else	
				printf(" LEITOR MOVIDO PARA LIXEIRA!\n É POSSÍVEL RESTAURÁ-LO DEPOIS.\n\n");
				
			fclose(*listaAtual);
			
		break;

	}
}

void limpaLixeira(void){
	
	struct Livro      livroAtual;
	struct Leitor     leitorAtual;
	
	FILE *leitor, *leitorAux, *livro, *livroAux;
	
	livro = fopen("livros/listaDeLivros.dat", "rb");
	livroAux = fopen("livros/listaDeLivros.edson", "wb");	
	
	leitor = fopen("leitores/listaDeLeitores.dat", "rb");
	leitorAux = fopen("leitores/listaDeLeitores.edson", "wb");	
	
	if(leitor == NULL){
		printf("ERRO DE ABERTURA DE ARQUIVO! Leitor\n");
		return;
	}
	if(leitorAux == NULL){
		printf("ERRO DE ABERTURA DE ARQUIVO! LeitorAux\n");
		return;
	}
	if(livro == NULL){
		printf("ERRO DE ABERTURA DE ARQUIVO! Livro\n");
		return;
	}
	
	if(leitorAux == NULL){
		printf("ERRO DE ABERTURA DE ARQUIVO! LivroAux\n");
		return;
	}
	
	while(fread(&livroAtual, sizeof(struct Livro), 1, livro))
		if(livroAtual.deletado == ' ')
			fwrite(&livroAtual, sizeof(struct Livro), 1, livroAux);
	
	while(fread(&leitorAtual, sizeof(struct Livro),1, leitor))
		if(leitorAtual.deletado == ' ')
			fwrite(&leitorAtual, sizeof(struct Leitor), 1, leitorAux);
			
	fclose(livro);
	fclose(livroAux);
	
	fclose(leitor);
	fclose(leitorAux);	
	
	remove("livros/listaDeLivros.dat");
	remove("leitores/listaDeLeitores.dat");
	
	rename("livros/listaDeLivros.edson", "livros/listaDeLivros.dat");
	rename("leitores/listaDeLeitores.edson", "leitores/listaDeLeitores.dat");
	
	
}

int get_size(const char* file_name){
    FILE *file = fopen(file_name, "r");

    if(file == NULL)
        return 0;

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fclose(file);

    return size;
}

void imprimeSubCabecalho(int sessao){
	switch(sessao){
		case 1:// Cadastro de Leitores
			printf("CADASTRO DE LEITOR\n");
			printf("------------------\n\n");
		break;
		
		case 2://listagem de Leitores
			printf("LISTA DE LEITORES\n");
			printf("-----------------\n\n");
		break;
		
		case 3://Apresentação de Leitores
			printf(" LEITOR                         MATRÍCULA\n");
			printf(" ------------------------------ ---------\n");
		break;
		
		case 4:// Cadastro de Livros
			printf("CADASTRO DE LIVRO\n");
			printf("-----------------\n\n");
		break;
		
		case 5://listagem de Livros
			printf("LISTA DE LIVROS\n");
			printf("---------------\n\n");
		break;
		
		case 6:///Apresentação de Livros
			printf(" LIVRO                          COD.LIVRO\n");
			printf(" ------------------------------ ---------\n");
		break;
		
		case 7://Realização de Empréstimo
			printf("EMPRÉSTIMO\n");
			printf("----------\n\n");
		break;
		
		case 8://Listagem de Empréstimo
			printf(" COD                         LEITOR MATRÍCULA LIVRO                          COD.LIVRO\n");
			printf(" --- ------------------------------ --------- ------------------------------ ---------\n");
		break;
		
		case 9://Realização de Consulta
			printf("CONSULTA\n");
			printf("----------\n\n");
		break;
		
		case 10://Realização de Alteração
			printf("ALTERAÇÃO\n");
			printf("----------\n\n");
		break;		
		
		case 11:
			printf("EXCLUSÃO\n");
			printf("--------\n\n");
		break;
		
		case 12://Erro listagem de Leitores
			printf("---ERRO: IMPOSSÍVEL LISTAR LEITORES---\n\n\n");
			printf(" NENHUM LEITOR CADASTRADO ATE O MOMENTO!\n\n");
			printf("\n\n");
		break;
		
		case 13://Erro listagem de Leitores
			printf("---ERRO: IMPOSSÍVEL LISTAR LIVROS---\n\n\n");
			printf(" NENHUM LIVRO CADASTRADO ATE O MOMENTO!\n\n");
			printf("\n\n");
		break;
		
		case 14:
			printf("LIXEIRA\n");
			printf("-------\n\n");
		break;
	}
}
