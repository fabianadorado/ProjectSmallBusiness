#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

int lernumero(const string& mensagem);
double lerFloatPositivo(const string& mensagem);
string toLower(const string& str);
string toUpper(const string& str);

#define END_COLOR  "\033[0m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define BOLD    "\033[1m"
#define YELLOW  "\033[33m"

void limparBuffer();
void desenharLinhaHorizontal(const string& inicio, const string& fim, size_t largura);
void desenharLinhaHorizontalVenda(const string& inicio, const string& fim, size_t largura);
string repetir(const string& s, size_t n);
void desenharCaixaTitulo(const string& titulo, size_t largura);
int mostrarMenu(const string& titulo, const vector<string>& opcoes);
bool arquivoExiste(const string& nomeArquivo);
void criarDiretorioSeNaoExistir(const string& nomeDiretorio);
void imprimirLinhaInterna(const string& margem, int largura, const string& conteudo, const string& corFundo, const string& corTexto, const string& reset);
void imprimirLinhaHorizontalBranca(const string& margem, int largura, const string& corFundo, const string& corTexto, const string& reset);

// Função auxiliar para imprimir uma linha de tabela com colunas de larguras fixas
void imprimirLinhaTabela(const vector<string>& colunas, const vector<int>& larguras);

// Função auxiliar para centralizar e imprimir um título com bordas
void imprimirTituloCentralizado(const string& titulo, int largura, const string& margem = "    ");

string lerTelefone(const string& mensagem);
string formatarTelefone(const string& telefone);

string centro(const string& texto, int largura);
