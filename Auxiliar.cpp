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
#include <string>


using namespace std;

const std::string MARGEM = "    ";
const char* RESET = "\033[0m";
const char* RED = "\033[31m";
const char* GREEN = "\033[38;5;22m";
const char* YELLOW = "\033[38;5;3m";
const char* CYAN = "\033[36m";
const char* BOLD = "\033[1m";
const char* FG_BLACK = "\033[30m";
const std::string FUNDO_BRANCO = "\033[47m";
const std::string TEXTO_PRETO = "\033[30m";
#define BG_GRAY "\033[100m"
#define FG_BLUE "\033[34m"
#define FG_BRIGHT_WHITE "\033[97m"

int lernumero(const string& mensagem) {
    int valor;
    string input;
    bool primeiraTentativa = true;
    while (true) {
        if (!primeiraTentativa) {
            cout << BG_GRAY << RED << MARGEM << "Entrada invalida. Digite um numero inteiro maior que 0." << RESET << endl;
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
                cout << BG_GRAY << RED << MARGEM << "Entrada invalida. Digite um numero valido (>= 0)." << RESET << endl;
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
    string linha_str;
    for (size_t i = 0; i < largura; ++i) {
        linha_str += "-";
    }
    cout << BOLD << MARGEM << inicio << linha_str << fim << RESET << endl;
}

void desenharLinhaHorizontalVenda(const string& inicio, const string& fim, size_t largura) {
    string linha_str;
    for (size_t i = 0; i < largura; ++i) {
        linha_str += "-";
    }
    cout << BOLD << MARGEM << inicio << linha_str << fim << RESET << endl;
}

string repetir(const string& s, size_t n) {
    string r;
    for (size_t i = 0; i < n; ++i) r += s;
    return r;
}

void desenharCaixaTitulo(const string& titulo, size_t largura) {
    string bordaTop = MARGEM + "+" + repetir("=", largura) + "+";
    string bordaBottom = MARGEM + "+" + repetir("=", largura) + "+";
    size_t espacoEsq = (largura - titulo.length()) / 2;
    size_t espacoDir = largura - titulo.length() - espacoEsq;
    cout << CYAN << "\n" << bordaTop << "\n";
    cout << MARGEM << "|" << repetir(" ", espacoEsq) << BOLD << titulo << RESET << CYAN << repetir(" ", espacoDir) << "|\n";
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
    size_t largura = max<size_t>(titulo.length(), 40);
    for (const auto& op : opcoes) {
        string linha = "X - " + op;
        if (linha.length() > largura) largura = linha.length();
    }
    largura += 8;
    int opcao = -1;
    bool primeiraTentativa = true;
    do {
        system("cls");
        preencherTela(BG_BLUE, FG_BRIGHT_WHITE);
        desenharNALATA();
        // Topo
        cout << BG_BLUE << FG_GRAY_MEDIUM << "\n" << MARGEM << "+" << string(largura, '-') << "+" << RESET << "\n";
        // Título centralizado
        size_t espacoEsq = (largura - titulo.length()) / 2;
        size_t espacoDir = largura - titulo.length() - espacoEsq;
        cout << BG_BLUE << FG_GRAY_MEDIUM << MARGEM << "|" << string(espacoEsq, ' ') << titulo << string(espacoDir, ' ') << "|" << RESET << "\n";
        cout << BG_BLUE << FG_GRAY_MEDIUM << MARGEM << "+" << string(largura, '-') << "+" << RESET << "\n";
        // Opções
        for (size_t i = 0; i < opcoes.size(); ++i) {
            string texto = to_string(i + 1) + " - " + opcoes[i];
            size_t espaco = largura - texto.length();
            cout << BG_BLUE << FG_GRAY_MEDIUM << MARGEM << "| " << texto << string(espaco - 1, ' ') << "|" << RESET << "\n";
        }
        cout << BG_BLUE << FG_GRAY_MEDIUM << MARGEM << "+" << string(largura, '-') << "+" << RESET << "\n";
        cout << BG_BLUE << FG_GRAY_MEDIUM << "\n" << MARGEM << "Escolha uma opcao: " << BG_BLUE << FG_GRAY_MEDIUM;
        string input;
        getline(cin, input);
        cout << RESET;
        istringstream iss(input);
        if ((iss >> opcao) && opcao > 0 && opcao <= (int)opcoes.size()) {
            char extra;
            if (!(iss >> extra)) {
                return opcao;
            }
        }
        if (!primeiraTentativa) {
            cout << BG_BLUE << RED << MARGEM << "Opcao invalida. Digite um numero entre 1 e " << opcoes.size() << "." << RESET << endl;
        }
        primeiraTentativa = false;
    } while (true);
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
            cout << MARGEM << "\033[31mTelefone invalido. Digite exatamente 9 numeros:\033[0m\n";
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
    std::string input;
    while (true) {
        std::cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << mensagem << " (s/n): " << RESET;
        std::cout << BG_GRAY << FG_BLUE;
        std::getline(std::cin, input);
        std::cout << RESET;
        if (input.length() == 1 && (input[0] == 's' || input[0] == 'S')) {
            return true;
        } else if (input.length() == 1 && (input[0] == 'n' || input[0] == 'N')) {
            return false;
        } else {
            std::cout << BG_GRAY << RED << MARGEM << "Entrada invalida. Digite 's' para sim ou 'n' para nao." << RESET << std::endl;
        }
    }
}

bool confirmarAcao(const std::string& mensagem) {
    std::string input;
    while (true) {
        std::cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << mensagem << " (s/n): " << RESET;
        std::cout << BG_GRAY << FG_BLUE;
        std::getline(std::cin, input);
        std::cout << RESET;
        if (input.length() == 1 && (input[0] == 's' || input[0] == 'S')) {
            return true;
        } else if (input.length() == 1 && (input[0] == 'n' || input[0] == 'N')) {
            return false;
        } else {
            std::cout << BG_GRAY << RED << MARGEM << "Entrada invalida. Digite 's' para sim ou 'n' para nao." << RESET << std::endl;
        }
    }
}

Produto* encontrarProdutoPorNome(const std::string& nome, std::vector<Produto>& produtos) {
    std::string nomePadrao = normalizarNomeProduto(nome);
    for (auto& p : produtos) {
        if (normalizarNomeProduto(p.getNome()) == nomePadrao) return &p;
    }
    return nullptr;
}

Cliente* encontrarClientePorNome(const std::string& nome, std::vector<Cliente>& clientes) {
    std::string nomePadrao = normalizarNomeProduto(nome);
    for (auto& c : clientes) {
        if (normalizarNomeProduto(c.getNome()) == nomePadrao) return &c;
    }
    return nullptr;
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

void preencherTela(const std::string& bgColor, const std::string& fgColor, int linhas, int colunas) {
    for (int i = 0; i < linhas; ++i)
        std::cout << bgColor << fgColor << std::string(colunas, ' ') << RESET << std::endl;
    std::cout << "\033[" << linhas << "A";
    std::cout << bgColor << fgColor;
}

string normalizarNomeProduto(const string& nome) {
    return removerPontuacao(toUpper(trim(nome)));
}

string alinhar(const string& texto, int largura) {
    if ((int)texto.size() >= largura) return texto.substr(0, largura);
    return texto + string(largura - texto.size(), ' ');
}

void imprimirCabecalhoDetalhadoTabela(const std::string& margem, int wProd, int wQtd, int wUnit, int wTotal, int wCusto, int wLucro) {
    std::cout << BG_GRAY << FG_BLUE << margem
        << "|" << centroExato("NOME DO PRODUTO", wProd)
        << "|" << centroExato("QTD", wQtd)
        << "|" << centroExato("PRECO UNIT.", wUnit)
        << "|" << centroExato("TOTAL C/IVA", wTotal)
        << "|" << centroExato("CUSTO", wCusto)
        << "|" << centroExato("LUCRO", wLucro)
        << "|" << RESET << std::endl;
    std::cout << BG_GRAY << FG_BLUE << margem
        << std::string(wProd, '-') << "+" << std::string(wQtd, '-') << "+" << std::string(wUnit, '-') << "+"
        << std::string(wTotal, '-') << "+" << std::string(wCusto, '-') << "+" << std::string(wLucro, '-') << "+"
        << RESET << std::endl;
}
// Centraliza texto e garante exatamente o tamanho da largura
string centroExato(const string& texto, int largura) {
    string t = texto;
    if ((int)t.length() > largura) t = t.substr(0, largura);
    int espacos = largura - (int)t.length();
    int esq = espacos / 2;
    int dir = espacos - esq;
    return string(esq, ' ') + t + string(dir, ' ');
}

int lerIDPositivo(const std::string& mensagem, bool suprimirErro) {
    int valor;
    std::string input;
    while (true) {
        std::cout << BG_GRAY << FG_BLUE << mensagem << std::flush;
        std::getline(std::cin, input);
        std::cout << RESET;
        std::istringstream iss(input);
        if ((iss >> valor) && valor > 0) {
            char extra;
            if (!(iss >> extra)) {
                return valor;
            }
        }
        if (!suprimirErro) {
            std::cout << std::endl;
            std::cout << BG_BLUE << RED << MARGEM << "ID invalido. Digite um numero inteiro positivo." << RESET << std::endl;
        }
    }
}

void desenharNALATA() {
    const int largura_terminal = 80; // ajuste conforme necessário
    const char* FG_DARK_RED = "\033[38;5;208m";
    //const char* BG_BLUE = "\033[44m";
    string banner = R"(
    )           (                                
 ( /(   (       )\ )    (       *   )    (      
 )\())  )\     (()/(    )\    ` )  /(    )\     
((_)\((((_)(    /(_))((((_)(   ( )(_))((((_)(   
 _((_) 5c _ )\  (_))   )\ _ )\ (_(_())  )\ _ )\  
| \| |(_)_\(_) | |    (_)_\(_)|_   _|  (_)_\(_) 
| .` | / _ \   | |__   / _ \    | |     / _ \   
|_|\_|/_/ \_\  |____| /_/ \_\   |_|    /_/ \_\  
                                                
)";
    istringstream iss(banner);
    string linha;
    while (getline(iss, linha)) {
        int espacos = (largura_terminal - (int)linha.length());
        if (espacos < 0) espacos = 0;
        cout << BG_BLUE << FG_DARK_RED << string(espacos / 2, ' ') << linha << RESET << endl;
    }
}
