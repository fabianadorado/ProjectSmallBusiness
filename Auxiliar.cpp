#define NOMINMAX
#include "Auxiliar.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <sys/stat.h>
#include <windows.h>

// Cores ANSI
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define BOLD    "\033[1m"
#define YELLOW  "\033[33m"
#define RESET   "\033[0m"
#define END_COLOR "\033[0m"

using namespace std;

int lernumero(const string& mensagem) {
    int valor;
    while (true) {
        cout << mensagem;
        if (cin >> valor && valor > 0) {
            cin.ignore(1000, '\n');
            return valor;
        }
        cout << "\033[31mEntrada inválida. Digite um número inteiro maior que 0.\033[0m\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

double lerFloatPositivo(const string& mensagem) {
    double valor;
    while (true) {
        cout << mensagem;
        string input;
        getline(cin, input);
        replace(input.begin(), input.end(), ',', '.');
        istringstream iss(input);
        if (iss >> valor && valor >= 0.0f) {
            char extra;
            if (iss >> extra) {
                cout << "\033[31mEntrada inválida. Digite apenas números.\033[0m\n";
            } else {
                return valor;
            }
        } else {
            cout << "\033[31mEntrada inválida. Digite um número válido (>= 0).\033[0m\n";
        }
        cin.clear();
    }
}

string toLower(const string& str) {
    string resultado = str;
    for (char& c : resultado) {
        c = tolower(static_cast<unsigned char>(c));
    }
    return resultado;
}

void limparBuffer() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void desenharLinhaHorizontal(const string& inicio, const string& fim, size_t largura) {
    string linha_str;
    for (size_t i = 0; i < largura; ++i) {
        linha_str += "-";
    }
    cout << BOLD << inicio << linha_str << fim << END_COLOR << endl;
}

void desenharLinhaHorizontalVenda(const string& inicio, const string& fim, size_t largura) {
    string linha_str;
    for (size_t i = 0; i < largura; ++i) {
        linha_str += "-";
    }
    cout << BOLD << inicio << linha_str << fim << END_COLOR << endl;
}

string repetir(const string& s, size_t n) {
    string r;
    for (size_t i = 0; i < n; ++i) r += s;
    return r;
}

void desenharCaixaTitulo(const string& titulo, size_t largura) {
    string bordaTop = "+" + repetir("=", largura) + "+";
    string bordaBottom = "+" + repetir("=", largura) + "+";
    size_t espacoEsq = (largura - titulo.length()) / 2;
    size_t espacoDir = largura - titulo.length() - espacoEsq;
    cout << CYAN << "\n" << bordaTop << "\n";
    cout << "|" << repetir(" ", espacoEsq) << BOLD << titulo << RESET << CYAN << repetir(" ", espacoDir) << "|\n";
    cout << bordaBottom << "\n" << RESET;
}

int mostrarMenu(const string& titulo, const vector<string>& opcoes) {
    size_t largura = titulo.length();
    for (const auto& op : opcoes) {
        string linha = "X - " + op;
        if (linha.length() > largura) largura = linha.length();
    }
    largura += 4;
    string bordaTop = "+" + repetir("=", largura) + "+";
    string bordaMeio = "+" + repetir("=", largura) + "+";
    string bordaBottom = "+" + repetir("=", largura) + "+";
    size_t espacoEsq = (largura - titulo.length()) / 2;
    size_t espacoDir = largura - titulo.length() - espacoEsq;
    cout << CYAN << bordaTop << "\n";
    cout << "|" << repetir(" ", espacoEsq) << BOLD << titulo << RESET << CYAN << repetir(" ", espacoDir) << "|\n";
    cout << bordaMeio << "\n" << CYAN;
    for (size_t i = 0; i < opcoes.size(); ++i) {
        string texto = to_string(i + 1) + " - " + opcoes[i];
        cout << "| " << texto << repetir(" ", largura - texto.length() - 1) << "|\n";
    }
    cout << CYAN << bordaBottom << "\n" << RESET;
    cout << YELLOW << "Escolha uma opção: " << RESET;
    int opcao;
    cin >> opcao;
    return opcao;
}

bool arquivoExiste(const string& nomeArquivo) {
    struct stat buffer;
    return (stat(nomeArquivo.c_str(), &buffer) == 0);
}

void criarDiretorioSeNaoExistir(const string& nomeDiretorio) {
    if (!arquivoExiste(nomeDiretorio)) {
        #ifdef _WIN32
        system(("mkdir " + nomeDiretorio).c_str());
        #else
        system(("mkdir -p " + nomeDiretorio).c_str());
        #endif
    }
}