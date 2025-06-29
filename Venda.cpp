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

    // Primeira borda (externa)
    cout << margem << "+" << string(LARGURA - 2, '-') << "+" << endl;
    {
        string titulo = "TALÃO DE COMPRA";
        int espaco = LARGURA - titulo.length();
        int esq = espaco / 2;
        int dir = espaco - esq;
        imprimirLinhaInterna(margem, LARGURA, string(esq, ' ') + titulo + string(dir, ' '), FUNDO_BRANCO, TEXTO_PRETO, RESET);
    }
    imprimirLinhaHorizontalBranca(margem, LARGURA, FUNDO_BRANCO, TEXTO_PRETO, RESET);

    // Informações da venda
    imprimirLinhaInterna(margem, LARGURA, "Data: " + data, FUNDO_BRANCO, TEXTO_PRETO, RESET);
    imprimirLinhaInterna(margem, LARGURA, "Fatura Nº: " + numeroFatura, FUNDO_BRANCO, TEXTO_PRETO, RESET);
    imprimirLinhaInterna(margem, LARGURA, "Cliente ID: " + to_string(idCliente), FUNDO_BRANCO, TEXTO_PRETO, RESET);
    imprimirLinhaHorizontalBranca(margem, LARGURA, FUNDO_BRANCO, TEXTO_PRETO, RESET);

    // Cabeçalho dos itens
    imprimirLinhaInterna(margem, LARGURA, "ITENS COMPRADOS", FUNDO_BRANCO, TEXTO_PRETO, RESET);
    imprimirLinhaHorizontalBranca(margem, LARGURA, FUNDO_BRANCO, TEXTO_PRETO, RESET);

    // Lista de itens (alinhamento fixo)
    for (const auto& item : itens) {
        ostringstream oss;
        string nome = item.nomeProduto;
        if (nome.length() > 20) nome = nome.substr(0, 20);
        oss << right << setw(2) << item.numeroLinha << ". ";
        oss << left << setw(20) << nome;
        oss << right << setw(3) << item.quantidade << " x ";
        oss << right << setw(6) << fixed << setprecision(2) << item.precoUnitario;
        oss << " = ";
        oss << right << setw(7) << fixed << setprecision(2) << item.precoSemIVA << " €";
        string linha = oss.str();
        if (linha.length() > static_cast<size_t>(LARGURA - 2))
            linha = linha.substr(0, LARGURA - 2);
        else if (linha.length() < static_cast<size_t>(LARGURA - 2))
            linha += string(LARGURA - 2 - linha.length(), ' ');
        imprimirLinhaInterna(margem, LARGURA, linha, FUNDO_BRANCO, TEXTO_PRETO, RESET);
    }
    imprimirLinhaHorizontalBranca(margem, LARGURA, FUNDO_BRANCO, TEXTO_PRETO, RESET);

    double total = getValorTotal();
    double semIVA = total / 1.23;
    double iva = total - semIVA;

    // Alinhar labels à esquerda e valores à direita, igual ao valor do item vendido, com afastamento automático do pipe direito
    auto formatarLinhaValor = [&](const string& label, double valor) {
        int posValor = 39; // posição onde começa o valor do item vendido
        int larguraInterna = LARGURA - 2;
        int espacosAntesPipe = 20; // número de espaços desejados antes do pipe direito

        ostringstream oss;
        string labelAjustado = label;
        if ((int)labelAjustado.length() > posValor) labelAjustado = labelAjustado.substr(0, posValor);
        oss << left << setw(posValor) << labelAjustado;

        // Valor formatado
        ostringstream valorStream;
        valorStream << right << fixed << setprecision(2) << valor << " €";
        string valorStr = valorStream.str();

        // Monta a linha automaticamente
        int espacos = larguraInterna - posValor - valorStr.length() - espacosAntesPipe;
        if (espacos < 0) espacos = 0;
        string conteudo = oss.str() + string(espacos, ' ') + valorStr + string(espacosAntesPipe, ' ');

        // Garante o tamanho exato
        if (conteudo.length() > (size_t)larguraInterna)
            conteudo = conteudo.substr(0, larguraInterna);
        else if (conteudo.length() < (size_t)larguraInterna)
            conteudo += string(larguraInterna - conteudo.length(), ' ');

        imprimirLinhaInterna(margem, LARGURA, conteudo, FUNDO_BRANCO, TEXTO_PRETO, RESET);
    };

    formatarLinhaValor("Subtotal:", semIVA);
    formatarLinhaValor("IVA (23%):", iva);
    formatarLinhaValor("TOTAL:", total);
    imprimirLinhaHorizontalBranca(margem, LARGURA, FUNDO_BRANCO, TEXTO_PRETO, RESET);
    formatarLinhaValor("Valor pago:", valorEntregue);
    formatarLinhaValor("Troco:", troco);
    imprimirLinhaHorizontalBranca(margem, LARGURA, FUNDO_BRANCO, TEXTO_PRETO, RESET);
    imprimirLinhaInterna(margem, LARGURA, "Obrigado pela sua preferência!", FUNDO_BRANCO, TEXTO_PRETO, RESET);
    // Última borda (externa)
    cout << margem << "+" << string(LARGURA - 2, '-') << "+" << endl;
}