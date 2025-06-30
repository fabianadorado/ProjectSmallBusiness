// -*-coding: utf - 8 - *-
// Venda.cpp
#include "Venda.h"
#include <iomanip>   // para usar manipuladores de entrada e saida para controlar a formatação dos dados
#include <ctime>     // para adicionar horas e datas (talao)
#include <sstream>   // para poder usar setprecision em strings || deixa-me usar manipulador de strings
#include <iostream>
#include <limits>
#include <random>     // Para geração de números aleatórios
#include <chrono>     // Para geração de sementes aleatórias
#include "Auxiliar.h"

#define END_COLOR "\033[0m"  // Define a cor de texto para padrão
#define RED "\033[31m"   // Define a cor de texto para vermelho
#define GREEN "\033[32m" // Define a cor de texto para verde


ItemVenda::ItemVenda(int linha, const string& nomeProduto, int quantidade, double precoUnit, double precoCusto)
    :   numeroLinha(linha),
        nomeProduto(nomeProduto),
        quantidade(quantidade),
        precoUnitario(precoUnit),
        precoCusto(precoCusto),
        precoSemIVA(precoUnit* quantidade),
        iva(precoSemIVA * 0.23), // 23% IVA
        totalComIVA(precoSemIVA + iva)
{

}

// Inicialização do gerador aleatório
mt19937 Venda::gerador(std::random_device{}());
uniform_int_distribution<int> Venda::distribuidor(10000, 99999);

// Construtores da Venda
Venda::Venda()
    :   idCliente(0),
        valorEntregue(0.0),
        troco(0.0)
{
    ostringstream criarNFatura;
    criarNFatura << distribuidor(gerador);
    numeroFatura = criarNFatura.str();
}

Venda::Venda(int idCliente)
    :   idCliente(idCliente),
        valorEntregue(0.0),
        troco(0.0)
{
    ostringstream criarNFatura;
    criarNFatura << distribuidor(gerador);
    numeroFatura = criarNFatura.str();
}

// Métodos da Venda
void Venda::adicionarItem(const string& nomeProduto, int quantidade, double precoUnit, double precoCusto)
{
    // Validações essenciais
   

    if (quantidade <= 0)
    {
        throw invalid_argument("Quantidade deve ser maior que zero");
    }

    if (precoUnit < 0)
    {
        throw invalid_argument("Preço unitário não pode ser negativo");
    }
    int linha = itens.size() + 1;
    ItemVenda item(linha, nomeProduto, quantidade, precoUnit, precoCusto);
    itens.push_back(item);
}

void Venda::finalizarVenda(double valorEntregue)
{
    auto arred2 = [](double v) { return std::round(v * 100.0) / 100.0; };
    double total = getValorTotal();
    if (arred2(valorEntregue) < arred2(total))
    {
        throw invalid_argument("Valor entregue é inferior ao valor total da compra!");
    }
    this->valorEntregue = valorEntregue;

    troco = valorEntregue - total;
    if (troco < 0)
    {
        troco = 0;
    }
}
double Venda::getValorTotal() const
{
    double total = 0.0;
    for (const auto& item : itens)
    {
        total += item.totalComIVA;
    }
    return total;
}

double Venda::getTotalComIVA() const {
    double total = 0.0;
    for (const auto& item : itens) {
        total += item.totalComIVA;
    }
    return total;
}

double Venda::getTroco() const
{
    return troco;
}

string Venda::getNumeroFatura() const
{
    return numeroFatura;
}

int Venda::getIdCliente() const
{
    return idCliente;
}

void Venda::imprimirTalao() const {
    // Configurações de cores ANSI
    const string FUNDO_BRANCO = "\033[47m";
    const string TEXTO_PRETO = "\033[30m";
    const string NEGRITO = "\033[1m";
    const string RESET = "\033[0m";

    // Obter data atual
    time_t agora = time(0);
    tm tempoLocal;
    localtime_s(&tempoLocal, &agora);
    string data = to_string(tempoLocal.tm_mday) + "/" +
        to_string(tempoLocal.tm_mon + 1) + "/" +
        to_string(tempoLocal.tm_year + 1900);

    // Configurações de layout
    const int LARGURA = 60;
    const int larguraConsole = 100; // largura fixa para centralização
    int margemEsq = (larguraConsole > LARGURA) ? (larguraConsole - LARGURA) / 2 : 0;
    string margem(margemEsq, ' ');

    auto linhaBranca = [&](char c) {
        cout << margem << FUNDO_BRANCO << TEXTO_PRETO << string(LARGURA - 2, c) << RESET << endl;
    };
    auto linhaTexto = [&](const string& texto, bool centralizar = false) {
        string linha = texto;
        if (centralizar) linha = centro(linha, LARGURA - 2);
        if (linha.length() < LARGURA - 2)
            linha += string(LARGURA - 2 - linha.length(), ' ');
        else if (linha.length() > LARGURA - 2)
            linha = linha.substr(0, LARGURA - 2);
        cout << margem << FUNDO_BRANCO << TEXTO_PRETO << linha << RESET << endl;
    };

    // Título
    linhaBranca('=');
    linhaTexto("TALAO DE COMPRA", true);
    linhaBranca('=');

    // Dados
    linhaTexto("Data: " + data);
    linhaTexto("Fatura: " + numeroFatura);
    linhaTexto("Cliente ID: " + to_string(idCliente));
    linhaBranca('-');

    // Cabeçalho
    linhaTexto("ITENS COMPRADOS", true);
    linhaBranca('-');

    // Itens
    for (const auto& item : itens) {
        ostringstream oss;
        string nome = toUpper(item.nomeProduto);
        if (nome.length() > 20) nome = nome.substr(0, 20);
        oss << right << setw(2) << item.numeroLinha << ". ";
        oss << left << setw(20) << nome;
        oss << right << setw(3) << item.quantidade << " x ";
        oss << right << setw(6) << fixed << setprecision(2) << item.precoUnitario;
        oss << " = ";
        oss << right << setw(7) << fixed << setprecision(2) << item.precoSemIVA;
        string linha = oss.str();
        if (linha.length() > static_cast<size_t>(LARGURA - 2))
            linha = linha.substr(0, LARGURA - 2);
        else if (linha.length() < static_cast<size_t>(LARGURA - 2))
            linha += string(LARGURA - 2 - linha.length(), ' ');
        cout << margem << FUNDO_BRANCO << TEXTO_PRETO << linha << RESET << endl;
    }
    linhaBranca('-');

    double total = getValorTotal();
    double semIVA = total / 1.23;
    double iva = total - semIVA;

    auto linhaValor = [&](const string& label, double valor) {
        ostringstream oss;
        oss << left << setw(20) << label;
        ostringstream valorStream;
        valorStream << right << fixed << setprecision(2) << valor;
        string valorStr = valorStream.str();
        string conteudo = oss.str() + string(LARGURA - 2 - oss.str().length() - valorStr.length(), ' ') + valorStr;
        if (conteudo.length() > (size_t)LARGURA - 2)
            conteudo = conteudo.substr(0, LARGURA - 2);
        else if (conteudo.length() < (size_t)LARGURA - 2)
            conteudo += string(LARGURA - 2 - conteudo.length(), ' ');
        cout << margem << FUNDO_BRANCO << TEXTO_PRETO << conteudo << RESET << endl;
    };

    linhaValor("Subtotal:", semIVA);
    linhaValor("IVA (23%):", iva);
    linhaValor("TOTAL:", total);
    linhaBranca('-');
    linhaValor("Valor pago:", valorEntregue);
    linhaValor("Troco:", troco);
    linhaBranca('-');
    linhaTexto("Obrigado pela sua preferencia!");
    linhaBranca('=');
}