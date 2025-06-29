#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

int lernumero(const string& mensagem);
double lerFloatPositivo(const string& mensagem);
string toLower(const string& str);

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
void imprimirLinhaInterna(const std::string& margem, int largura, const std::string& conteudo, const std::string& corFundo, const std::string& corTexto, const std::string& reset);
void imprimirLinhaHorizontalBranca(const std::string& margem, int largura, const std::string& corFundo, const std::string& corTexto, const std::string& reset);

// Função auxiliar para imprimir uma linha de tabela com colunas de larguras fixas
void imprimirLinhaTabela(const std::vector<std::string>& colunas, const std::vector<int>& larguras);

// Função auxiliar para centralizar e imprimir um título com bordas
void imprimirTituloCentralizado(const std::string& titulo, int largura, const std::string& margem = "    ");
