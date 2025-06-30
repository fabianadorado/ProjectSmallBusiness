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
    // Definição das larguras das colunas (igual à tabela do talão)
    const int wId = 3, wProd = 22, wQtd = 6, wPSemIVA = 13, wPComIVA = 13, wTotal = 13;

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

    // Dados centralizados
    linhaTexto(centro("Data: " + data, LARGURA - 2), true);
    linhaTexto(centro("Fatura: " + numeroFatura, LARGURA - 2), true);
    linhaTexto(centro("Cliente ID: " + to_string(idCliente), LARGURA - 2), true);
    // Adicionar nome do cliente se disponível
    string nomeCliente = getNomeCliente();
    if (!nomeCliente.empty()) {
        linhaTexto(centro("Bem-vindo!", LARGURA - 2), true);
        linhaTexto(centro("Nome: " + nomeCliente, LARGURA - 2), true);
    }
    linhaBranca('-');

    // Cabeçalho
    linhaTexto(centro("ITENS COMPRADOS", LARGURA - 2), true);
    linhaBranca('-');

    // Itens com margem interna
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
        // Adiciona margem interna
        linha = " " + linha + " ";
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

    // Função para alinhar totais igual à linha do item, sem pipes intermediários
    auto linhaTotalSemPipes = [&](const string& label, double valor) {
        ostringstream oss;
        oss << left << setw(2) << ""; // espaço do número
        oss << left << setw(20) << label;
        oss << left << setw(3) << ""; // espaço da quantidade
        oss << left << setw(4) << ""; // espaço do 'x'
        oss << left << setw(6) << ""; // espaço do preço unitário
        oss << left << setw(3) << ""; // espaço do '='
        oss << right << setw(7) << fixed << setprecision(2) << valor;
        string linha = oss.str();
        if (linha.length() < LARGURA - 2)
            linha += string(LARGURA - 2 - linha.length(), ' ');
        cout << margem << FUNDO_BRANCO << TEXTO_PRETO << linha << RESET << endl;
    };

    linhaTotalSemPipes("Subtotal:", semIVA);
    linhaTotalSemPipes("IVA (23%):", iva);
    linhaTotalSemPipes("TOTAL:", total);
    linhaBranca('-');
    linhaTotalSemPipes("Valor pago:", valorEntregue);
    linhaTotalSemPipes("Troco:", troco);
    linhaBranca('-');
    // Frase final centralizada
    linhaTexto(centro("Obrigado pela sua preferencia!", LARGURA - 2), true);
    linhaBranca('=');
}

void Venda::removerItemPorLinha(int numeroLinha) {
    if (itens.empty()) {
        std::cout << "Nao ha itens para remover.\n";
        return;
    }
    auto it = std::find_if(itens.begin(), itens.end(), [numeroLinha](const ItemVenda& item) {
        return item.numeroLinha == numeroLinha;
    });
    if (it == itens.end()) {
        std::cout << "Item nao encontrado.\n";
        return;
    }
    if (!confirmarAcao("Tem certeza que deseja remover o item da linha " + std::to_string(numeroLinha) + "?")) {
        std::cout << "Operacao cancelada.\n";
        return;
    }
    itens.erase(it);
    // Atualiza os numeros das linhas restantes
    for (size_t i = 0; i < itens.size(); ++i) {
        itens[i].numeroLinha = i + 1;
    }
    std::cout << "Item removido.\n";
}

void Venda::cancelarVenda() {
    if (itens.empty()) {
        std::cout << "Nao ha venda em andamento para cancelar.\n";
        return;
    }
    if (!confirmarAcao("Tem certeza que deseja cancelar toda a venda?")) {
        std::cout << "Operacao cancelada.\n";
        return;
    }
    itens.clear();
    valorEntregue = 0.0;
    troco = 0.0;
    std::cout << "Venda cancelada.\n";
}