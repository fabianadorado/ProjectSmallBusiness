#pragma once
#include <string>
#include <iostream>

using namespace std;


class Produto {
private:
    static int proximoId;
    int id;
    string nome;
    int quantidade;
    double precoCusto;

public:
    Produto();
    Produto(int id, string nome, int quantidade, double precoCusto);

    int getId() const;
    string getNome() const;
    int getQuantidade() const;
    double getPrecoCusto() const;
    double getPrecoVenda() const;           // Preco com margem
    double getPrecoVendaComIVA() const;     // Preco com margem + IVA

    void adicionarStock(int qtd);
    void removerStock(int qtd);
    void setId(int novoId) { id = novoId; }
    void setPrecoCusto(double novoPreco) { precoCusto = novoPreco; }
};
