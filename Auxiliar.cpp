#define NOMINMAX
#include "Auxiliar.h"
#include <iostream>
#include <sstream>
#include <limits>
#include <cctype>
#include <iomanip>
#include <windows.h>
#include <algorithm>
#include <sys/stat.h>


// Cores ANSI
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define BOLD    "\033[1m"
#define YELLOW  "\033[33m"
#define RESET "\033[0m" 
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
        if (cin >> valor && valor >= 0.0f) {
            cin.ignore(1000, '\n');
            return valor;
        }
        cout << "\033[31mEntrada inválida. Digite um número válido (>= 0).\033[0m\n";
        cin.clear();
        cin.ignore(1000, '\n');
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
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void desenharLinhaHorizontal(const string& inicio, const string& fim, int largura)
{
    string linha_horizontal_char = "\u2500"; // Caractere de linha horizontal UTF-8 (U+2500)
    std::string linha_str;
    for (int i = 0; i < largura; ++i) 
    {
        linha_str += linha_horizontal_char;
    }
    cout << BOLD << inicio << linha_str << fim << END_COLOR << endl;
}

void desenharLinhaHorizontalVenda(const string& inicio, const string& fim, int largura) 
{
    string linha_char = "\u2500";
    string linha_str;
    for (int i = 0; i < largura; ++i) {
        linha_str += linha_char;
    }
    cout << BOLD << inicio << linha_str << fim << END_COLOR << endl;
}

string repetir(const string& s, int n) {
    string r;
    for (int i = 0; i < n; ++i) r += s;
    return r;
}

int mostrarMenu(const string& titulo, const vector<string>& opcoes) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Calcula largura máxima entre título e opções
    size_t largura = titulo.size();
    for (const auto& op : opcoes) {
        std::string linha = "X - " + op; // simulando "1 - Opção"
        if (linha.size() > largura) largura = linha.size();
    }
    largura += 4; // margem interna

    // Bordas
    string bordaTop = u8"╔" + repetir(u8"═", largura) + u8"╗";
    string bordaMeio = u8"╠" + repetir(u8"═", largura) + u8"╣";
    string bordaBottom = u8"╚" + repetir(u8"═", largura) + u8"╝";

    // Centraliza título
    int espacoEsq = (largura - titulo.size()) / 2;
    int espacoDir = largura - titulo.size() - espacoEsq;

    cout << CYAN << bordaTop << "\n";
    cout << u8"║" << repetir(" ", espacoEsq) << BOLD << titulo
        << RESET << CYAN << repetir(" ", espacoDir) << u8"║\n";
    cout << bordaMeio << "\n" << CYAN;

    // Exibe as opções com largura fixa
    for (size_t i = 0; i < opcoes.size(); ++i) {
        string texto = to_string(i + 1) + " - " + opcoes[i];
        cout << u8"║ " << texto
            << repetir(" ", largura - texto.size() - 1) << u8"║\n";
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