#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include "Produto.h"
#include "Cliente.h"

using namespace std;

extern const std::string MARGEM;
extern const std::string FUNDO_BRANCO;
extern const std::string TEXTO_PRETO;

extern const char* RESET;
extern const char* RED;
extern const char* GREEN;
extern const char* YELLOW;
extern const char* CYAN;
extern const char* BOLD;
extern const char* FG_BLACK;

int lernumero(const string& mensagem);
double lerFloatPositivo(const string& mensagem);
string toLower(const string& str);
string toUpper(const string& str);

#define BG_BLUE "\033[44m"
#define FG_BRIGHT_GRAY "\033[37m"
#define BG_GRAY "\033[100m"
#define FG_BLUE "\033[34m"
#define FG_GRAY_MEDIUM "\033[38;5;245m"
#define FG_RED "\033[31m"
#define FG_BLACK "\033[30m"
#define FUNDO_BRANCO "\033[47m"
#define TEXTO_PRETO "\033[30m"

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

bool confirmarAcao(const std::string& mensagem);

bool desejaContinuar(const std::string& mensagem = "Deseja realizar outra operacao?");

Produto* encontrarProdutoPorNome(const std::string& nome, std::vector<Produto>& produtos);
Cliente* encontrarClientePorNome(const std::string& nome, std::vector<Cliente>& clientes);

std::string trim(const std::string& str);

void preencherTela(const std::string& bgColor, const std::string& fgColor, int linhas = 40, int colunas = 120);

string normalizarNomeProduto(const string& nome);

string alinhar(const string& texto, int largura);

void imprimirCabecalhoDetalhadoTabela(const std::string& margem, int wProd, int wQtd, int wUnit, int wTotal, int wCusto, int wLucro);

string centroExato(const string& texto, int largura);

string normalizarDecimal(const string& input);

void desenharNALATA();

int lerIDPositivo(const std::string& mensagem);
