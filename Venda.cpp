// -*-coding: utf - 8 - *-
// Venda.cpp
#include "Venda.h"
#include <iomanip>   // para usar manipuladores de entrada e saida para controlar a formatacao dos dados
#include <ctime>     // para adicionar horas e datas (talao)
#include <sstream>   // para poder usar setprecision em strings || deixa-me usar manipulador de strings
#include <iostream>
#include <limits>
#include <random>     // Para geração de números aleatórios
#include <chrono>     // Para geração de sementes aleatórias
#include "Auxiliar.h"

#define BG_GRAY "\033[100m"
#define FUNDO_BRANCO "\033[47m"
#define TEXTO_PRETO "\033[30m"
#define RESET "\033[0m"

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
        throw invalid_argument("Preco unitario nao pode ser negativo");
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
        throw invalid_argument("Valor entregue e inferior ao valor total da compra!");
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

void Venda::imprimirTalao(const std::string& margemCentral) const {
    const int LARGURA = 60;
    // Função utilitária para imprimir qualquer linha do talão com fundo branco e texto preto, e margem cinza
    auto printLinhaTalao = [&](const std::string& texto) {
        // Imprime a margem com fundo cinza
        std::cout << BG_GRAY << FG_BLACK << margemCentral << RESET;
        // Imprime o talão com fundo branco
        std::cout << FUNDO_BRANCO << TEXTO_PRETO << texto << RESET << std::endl;
    };
    auto linhaBranca = [&](char c) {
        std::string faixaBranca(LARGURA, c);
        printLinhaTalao(faixaBranca);
    };
    auto linhaTexto = [&](const std::string& texto, bool centralizar = false) {
        std::string linha = texto;
        if (centralizar) linha = centro(linha, LARGURA);
        if (linha.length() < LARGURA)
            linha += std::string(LARGURA - linha.length(), ' ');
        else if (linha.length() > LARGURA)
            linha = linha.substr(0, LARGURA);
        printLinhaTalao(linha);
    };
    // Obter data atual
    time_t agora = time(0);
    tm tempoLocal = {};
    localtime_s(&tempoLocal, &agora);
    string data = to_string(tempoLocal.tm_mday) + "/" +
        to_string(tempoLocal.tm_mon + 1) + "/" +
        to_string(tempoLocal.tm_year + 1900);

    // Montar o talão no vetor
    linhaBranca('=');
    linhaTexto("TALAO DE COMPRA", true);
    linhaBranca('=');
    // Dados centralizados
    linhaTexto(centro("Data: " + data, LARGURA - 2), true);
    linhaTexto(centro("Fatura: " + numeroFatura, LARGURA - 2), true);
    linhaTexto(centro("Cliente ID: " + to_string(idCliente), LARGURA - 2), true);
    string nomeCliente = getNomeCliente();
    if (!nomeCliente.empty()) {
        linhaTexto(centro("Bem-vindo!", LARGURA - 2), true);
        linhaTexto(centro("Nome: " + nomeCliente, LARGURA - 2), true);
        ostringstream ossPontos;
        ossPontos << "Monsters points: " << getMonstersPoints();
        linhaTexto(centro(ossPontos.str(), LARGURA - 2), true);
    }
    linhaBranca('-');
    linhaTexto(centro("ITENS COMPRADOS", LARGURA - 2), true);
    linhaBranca('-');
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
        linha = " " + linha + " ";
        if (linha.length() > static_cast<size_t>(LARGURA - 2))
            linha = linha.substr(0, LARGURA - 2);
        else if (linha.length() < static_cast<size_t>(LARGURA - 2))
            linha += std::string(LARGURA - 2 - linha.length(), ' ');
        linhaTexto(linha, false);
    }
    linhaBranca('-');
    double total = getValorTotal();
    double semIVA = total / 1.23;
    double iva = total - semIVA;
    auto addLinhaTotal = [&](const string& label, double valor) {
        ostringstream oss;
        oss << left << setw(2) << "";
        oss << left << setw(20) << label;
        oss << left << setw(3) << "";
        oss << left << setw(4) << "";
        oss << left << setw(6) << "";
        oss << left << setw(3) << "";
        oss << right << setw(7) << fixed << setprecision(2) << valor;
        string linha = oss.str();
        if (linha.length() < LARGURA - 2)
            linha += std::string(LARGURA - 2 - linha.length(), ' ');
        linhaTexto(linha, false);
    };
    addLinhaTotal("Subtotal:", semIVA);
    addLinhaTotal("IVA (23%):", iva);
    addLinhaTotal("TOTAL:", total);
    linhaBranca('-');
    addLinhaTotal("Valor pago:", valorEntregue);
    addLinhaTotal("Troco:", troco);
    linhaBranca('-');
    string dataNasc = getDataNascimentoCliente();
    bool parabens = false;
    if (dataNasc.length() == 10) {
        int diaNasc = stoi(dataNasc.substr(0,2));
        int mesNasc = stoi(dataNasc.substr(3,2));
        if (diaNasc == tempoLocal.tm_mday && mesNasc == (tempoLocal.tm_mon + 1)) {
            parabens = true;
        }
    }
    if (parabens) {
        linhaTexto(centro("Parabens, hoje e seu dia que dia mais feliz!", LARGURA - 2), true);
    }
    linhaTexto(centro("Obrigado pela sua preferencia!", LARGURA - 2), true);
    linhaBranca('=');
}

void Venda::removerItemPorLinha(int numeroLinha) {
    if (itens.empty()) {
        std::cout << MARGEM << "Nao ha itens para remover.\n";
        return;
    }
    auto it = std::find_if(itens.begin(), itens.end(), [numeroLinha](const ItemVenda& item) {
        return item.numeroLinha == numeroLinha;
    });
    if (it == itens.end()) {
        std::cout << MARGEM << "Item nao encontrado.\n";
        return;
    }
    if (!confirmarAcao("Tem certeza que deseja remover o item da linha " + std::to_string(numeroLinha) + "?")) {
        std::cout << MARGEM << "Operacao cancelada.\n";
        return;
    }
    itens.erase(it);
    // Atualiza os numeros das linhas restantes
    for (size_t i = 0; i < itens.size(); ++i) {
        itens[i].numeroLinha = i + 1;
    }
    std::cout << MARGEM << "Item removido.\n";
}

void Venda::cancelarVenda() {
    if (itens.empty()) {
        std::cout << MARGEM << "Nao ha venda em andamento para cancelar.\n";
        return;
    }
    if (!confirmarAcao("Tem certeza que deseja cancelar toda a venda?")) {
        std::cout << MARGEM << "Operacao cancelada.\n";
        return;
    }
    itens.clear();
    valorEntregue = 0.0;
    troco = 0.0;
    std::cout << MARGEM << "Venda cancelada.\n";
}