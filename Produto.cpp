#include "Produto.h"
#include <iostream>

using namespace std;

// Inicialização do ID estático
int Produto::proximoId = 1;

// Construtor padrão
Produto::Produto() {
    id = proximoId++;
    nome = "";
    quantidade = 0;
    precoCusto = 0.0;
}

// Construtor com parâmetros
Produto::Produto(int id, string nome, int quantidade, double precoCusto) {
    this->id = id;
    this->nome = nome;
    this->quantidade = quantidade;
    this->precoCusto = precoCusto;
    if (id >= proximoId) {
        proximoId = id + 1;
    }
}

// Getters
int Produto::getId() const {
    return id;
}

string Produto::getNome() const {
    return nome;
}

int Produto::getQuantidade() const {
    return quantidade;
}

double Produto::getPrecoCusto() const {
    return precoCusto;
}

// Preço de venda com margem (exemplo: 30% de lucro)
double Produto::getPrecoVenda() const {
    return precoCusto * 1.3;
}

// Preço de venda com IVA (23% aplicado sobre o preço de venda)
double Produto::getPrecoVendaComIVA() const {
    double precoVenda = getPrecoVenda();
    return precoVenda * 1.23;
}

// Estoque
void Produto::adicionarStock(int qtd) {
    quantidade += qtd;
}

void Produto::removerStock(int qtd) {
    if (quantidade >= qtd) {
        quantidade -= qtd;
    }
}