#pragma once
#include <string>
#include <vector>
#include <iomanip>
#include <sstream> 
#include <random>
#include <chrono>
#include "Cliente.h"

using namespace std;

struct ItemVenda {
    int numeroLinha;
    string nomeProduto;
    int quantidade;
    double precoUnitario;
    double precoSemIVA;
    double iva;
    double totalComIVA;
    double precoCusto;

    ItemVenda(int linha, const string& nomeProduto, int quantidade, double precoUnit, double precoCusto);
};

class Venda {
private:
    static mt19937 gerador;  // Gerador Mersenne Twister
    static uniform_int_distribution<int> distribuidor;  // Distribuicao

    int idCliente;
    string numeroFatura;
    vector<ItemVenda> itens;
    double valorEntregue;
    double troco;
    bool gratis = false;
    string nomeCliente;

public:
    Venda();
    //explicit Venda(int idCliente);  // explicit previne conversoes implicitas
    Venda(int idCliente);

    void adicionarItem(const string& nomeProduto, int quantidade, double precoUnit, double precoCusto);
    void finalizarVenda(double valorEntregue);
    void imprimirTalao() const;

    string getNumeroFatura() const;

    int getIdCliente() const;
    double getValorTotal() const;
    double getTroco() const;
    double getTotalComIVA() const;

    bool isGratis() const
    {
        return gratis;
    }

    const vector<ItemVenda>& getItens() const
    {
        return itens;
    }

    void removerItemPorLinha(int numeroLinha);
    void cancelarVenda();

    void setNomeCliente(const string& nome) { nomeCliente = nome; }
    string getNomeCliente() const { return nomeCliente; }

};
