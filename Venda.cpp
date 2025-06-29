
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
    double total = getValorTotal();
    if (valorEntregue < total)
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
    const string VERDE = "\033[32m";

    // Obter data atual
    time_t agora = time(0);
    tm tempoLocal;
    localtime_s(&tempoLocal, &agora);
    string data = to_string(tempoLocal.tm_mday) + "/" +
        to_string(tempoLocal.tm_mon + 1) + "/" +
        to_string(tempoLocal.tm_year + 1900);


    // Configurações de layout
    const int LARGURA = 60;
    auto linhaComBordas = [&](const string& conteudo) {
        return NEGRITO + "\u2502 " + conteudo +
            string(LARGURA - 2 - conteudo.length(), ' ') + " \u2502" + END_COLOR;
        };

    // Início do talão
    cout << FUNDO_BRANCO << TEXTO_PRETO;
    desenharLinhaHorizontalVenda("\u250C", "\u2510", LARGURA);

    // Cabeçalho
    cout << linhaComBordas("TALÃO DE COMPRA") << endl;
    desenharLinhaHorizontalVenda("\u251C", "\u2524", LARGURA);

    // Informações da venda
    cout << linhaComBordas("Data: " + data) << endl;
    cout << linhaComBordas("Fatura Nº: " + numeroFatura) << endl;
    cout << linhaComBordas("Cliente ID: " + to_string(idCliente)) << endl;
    desenharLinhaHorizontalVenda("\u251C", "\u2524", LARGURA);

    // Cabeçalho dos itens
    cout << linhaComBordas("ITENS COMPRADOS") << endl;
    desenharLinhaHorizontalVenda("\u251C", "\u2524", LARGURA);

    // Lista de itens
    for (const auto& item : itens) {
        ostringstream oss;
        oss << item.numeroLinha << ". " << left << setw(20) << item.nomeProduto
            << right << setw(3) << item.quantidade << " x "
            << fixed << setprecision(2) << (item.precoSemIVA / item.quantidade)
            << " = " << item.totalComIVA << " €";

        cout << linhaComBordas(oss.str()) << endl;
    }

    // Totais
    desenharLinhaHorizontalVenda("\u251C", "\u2524", LARGURA);

    double total = getValorTotal();
    bool gratis = false;
    if (gratis) 
    {
        cout << linhaComBordas(VERDE + "COMPRA GRATUITA!" + END_COLOR) << endl;
    }
    else {
        double semIVA = total / 1.23;
        double iva = total - semIVA;

        auto formatar = [](string label, double valor) 
            {
            ostringstream oss;
            oss << right << setw(10) << fixed << setprecision(2) << valor << " €";
            return label + oss.str();
            };

        cout << linhaComBordas(formatar("Subtotal: ", semIVA)) << endl;
        cout << linhaComBordas(formatar("IVA (23%): ", iva)) << endl;
        cout << linhaComBordas(formatar("TOTAL: ", total)) << endl;
        desenharLinhaHorizontalVenda("\u251C", "\u2524", LARGURA);
        cout << linhaComBordas(formatar("Valor pago: ", valorEntregue)) << endl;
        cout << linhaComBordas(formatar("Troco: ", troco)) << endl;
    }

    // Rodapé
    desenharLinhaHorizontalVenda("\u2514", "\u2518", LARGURA);
    cout << linhaComBordas("Obrigado pela sua preferência!") << endl;
    desenharLinhaHorizontalVenda("\u2500", "\u2500", LARGURA);

    // END_COLOR cores
    cout << END_COLOR;
}