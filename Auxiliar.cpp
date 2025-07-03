#define NOMINMAX
#include "Auxiliar.h"
#include "Produto.h"
#include "Cliente.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <sys/stat.h>
#include <windows.h>
#include <locale>
#include <codecvt>
#include <regex>

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
    string input;
    bool primeiraTentativa = true;
    while (true) {
        if (!primeiraTentativa) {
            cout << "\033[31mEntrada invalida. Digite um numero inteiro maior que 0.\033[0m\n";
        }
        cout << mensagem;
        getline(cin, input);
        istringstream iss(input);
        if (iss >> valor && valor > 0) {
            char extra;
            if (!(iss >> extra)) {
                return valor;
            }
        }
        primeiraTentativa = false;
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
            if (!(iss >> extra)) {
                return valor;
            }
        }
        cout << "\033[31mEntrada inválida. Digite um número válido (>= 0).\033[0m\n";
    }
}

string toLower(const string& str) {
    string resultado = str;
    for (char& c : resultado) {
        c = tolower(static_cast<unsigned char>(c));
    }
    return resultado;
}

string toUpper(const string& str) {
    string resultado = str;
    for (char& c : resultado) {
        c = toupper(static_cast<unsigned char>(c));
    }
    return resultado;
}

void limparBuffer() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void desenharLinhaHorizontal(const string& inicio, const string& fim, size_t largura) {
    const string margem = "    ";
    string linha_str;
    for (size_t i = 0; i < largura; ++i) {
        linha_str += "-";
    }
    cout << BOLD << margem << inicio << linha_str << fim << END_COLOR << endl;
}

void desenharLinhaHorizontalVenda(const string& inicio, const string& fim, size_t largura) {
    const string margem = "    ";
    string linha_str;
    for (size_t i = 0; i < largura; ++i) {
        linha_str += "-";
    }
    cout << BOLD << margem << inicio << linha_str << fim << END_COLOR << endl;
}

string repetir(const string& s, size_t n) {
    string r;
    for (size_t i = 0; i < n; ++i) r += s;
    return r;
}

void desenharCaixaTitulo(const string& titulo, size_t largura) {
    const string margem = "    ";
    string bordaTop = margem + "+" + repetir("=", largura) + "+";
    string bordaBottom = margem + "+" + repetir("=", largura) + "+";
    size_t espacoEsq = (largura - titulo.length()) / 2;
    size_t espacoDir = largura - titulo.length() - espacoEsq;
    cout << CYAN << "\n" << bordaTop << "\n";
    cout << margem << "|" << repetir(" ", espacoEsq) << BOLD << titulo << RESET << CYAN << repetir(" ", espacoDir) << "|\n";
    cout << bordaBottom << "\n" << RESET;
}

// Função auxiliar para remover pontuação e acentos de uma string
string removerPontuacao(const string& str) {
    string resultado;
    for (char c : str) {
        if (isalnum(static_cast<unsigned char>(c)) || isspace(static_cast<unsigned char>(c))) {
            resultado += c;
        }
    }
    return resultado;
}

int mostrarMenu(const string& titulo, const vector<string>& opcoes) {
    const string margem = "    "; // 4 espaços
    size_t largura = titulo.length();
    for (const auto& op : opcoes) {
        string linha = "X - " + op;
        if (linha.length() > largura) largura = linha.length();
    }
    largura += 4;
    string bordaTop = margem + CYAN + BOLD "+" + repetir("=", largura) + "+" + RESET;
    string bordaMeio = margem + CYAN + BOLD "+" + repetir("-", largura) + "+" + RESET;
    string bordaBottom = margem + CYAN + BOLD "+" + repetir("=", largura) + "+" + RESET;
    size_t espacoEsq = (largura - titulo.length()) / 2;
    size_t espacoDir = largura - titulo.length() - espacoEsq;
    cout << bordaTop << "\n";
    cout << margem << CYAN << BOLD << "|" << repetir(" ", espacoEsq) << titulo << repetir(" ", espacoDir) << "|" << RESET << "\n";
    cout << bordaMeio << "\n";
    for (size_t i = 0; i < opcoes.size(); ++i) {
        string texto = to_string(i + 1) + " - " + opcoes[i];
        cout << margem << "| " << YELLOW << texto << RESET << repetir(" ", largura - texto.length() - 1) << "|" << "\n";
    }
    cout << bordaBottom << "\n";
    cout << margem << "Escolha uma opcao: ";
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

void imprimirLinhaInterna(const string& margem, int largura, const string& conteudo, const string& corFundo, const string& corTexto, const string& reset) {
    string conteudoAjustado = conteudo;
    if (conteudoAjustado.length() > static_cast<size_t>(largura - 2)) {
        conteudoAjustado = conteudoAjustado.substr(0, largura - 2);
    }
    int espacos = largura - 2 - conteudoAjustado.length();
    if (espacos < 0) espacos = 0;
    cout << margem << "|" << corFundo << corTexto << conteudoAjustado << string(espacos, ' ') << reset << "|" << endl;
}

void imprimirLinhaHorizontalBranca(const string& margem, int largura, const string& corFundo, const string& corTexto, const string& reset) {
    cout << margem << "|" << corFundo << corTexto << string(largura - 2, '-') << reset << "|" << endl;
}

void imprimirLinhaTabela(const vector<string>& colunas, const vector<int>& larguras) {
    cout << "|";
    for (size_t i = 0; i < colunas.size(); ++i) {
        cout << " " << left << setw(larguras[i]) << colunas[i] << " |";
    }
    cout << endl;
}

void imprimirTituloCentralizado(const string& titulo, int largura, const string& margem) {
    cout << margem << "+" << string(largura - 2, '=') << "+" << endl;
    int espaco = largura - 4 - static_cast<int>(titulo.length());
    int esq = espaco / 2;
    int dir = espaco - esq;
    cout << margem << "|" << string(esq + 1, ' ') << titulo << string(dir + 1, ' ') << "|" << endl;
    cout << margem << "+" << string(largura - 2, '=') << "+" << endl;
}

string lerTelefone(const string& mensagem) {
    regex telefoneRegex("^\\d{9}$"); // Exatamente 9 dígitos
    string telefone;
    while (true) {
        cout << mensagem;
        getline(cin, telefone);
        if (regex_match(telefone, telefoneRegex)) {
            return telefone;
        } else {
            cout << "\033[31mTelefone invalido. Digite exatamente 9 numeros.\033[0m\n";
        }
    }
}

string formatarTelefone(const string& telefone) {
    // Formata para xxxxx-xxxx
    if (telefone.length() == 9)
        return telefone.substr(0,5) + "-" + telefone.substr(5,4);
    return telefone;
}

// Função utilitária para centralizar e truncar texto em uma largura fixa
string centro(const string& texto, int largura) {
    string t = texto;
    if ((int)t.length() > largura) t = t.substr(0, largura);
    int espacos = largura - (int)t.length();
    int esq = espacos / 2;
    int dir = espacos - esq;
    return string(esq, ' ') + t + string(dir, ' ');
}

bool desejaContinuar(const std::string& mensagem) {
    char opcao;
    while (true) {
        std::cout << mensagem << " (s/n): ";
        std::cin >> opcao;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (opcao == 's' || opcao == 'S') {
            return true;
        } else if (opcao == 'n' || opcao == 'N') {
            return false;
        } else {
            std::cout << "Entrada invalida. Digite 's' para sim ou 'n' para nao." << std::endl;
        }
    }
}

bool confirmarAcao(const std::string& mensagem) {
    char opcao;
    while (true) {
        std::cout << mensagem << " (s/n): ";
        std::cin >> opcao;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (opcao == 's' || opcao == 'S') {
            return true;
        } else if (opcao == 'n' || opcao == 'N') {
            return false;
        } else {
            std::cout << "Entrada invalida. Digite 's' para sim ou 'n' para nao." << std::endl;
        }
    }
}

Produto* encontrarProdutoPorNome(const std::string& nome, std::vector<Produto>& produtos) {
    std::string nomePadrao = toUpper(trim(nome));
    for (auto& p : produtos) {
        if (toUpper(trim(p.getNome())) == nomePadrao) return &p;
    }
    return nullptr;
}

Cliente* encontrarClientePorNome(const std::string& nome, std::vector<Cliente>& clientes) {
    std::string nomePadrao = toUpper(trim(nome));
    for (auto& c : clientes) {
        if (toUpper(trim(c.getNome())) == nomePadrao) return &c;
    }
    return nullptr;
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}