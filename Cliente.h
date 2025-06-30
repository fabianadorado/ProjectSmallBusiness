#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Cliente {
private:
    static int proximoCliente;
    static vector<int> idsDisponiveis;
    int idCliente;
    string nome;
    string telefone;
    string morada;
    double totalComprado;

public:
    Cliente();
    Cliente(string nome, string telefone, string morada, double totalComprado = 0.0f);

    // Getters
    int getIdCliente() const;
    string getNome() const;
    string getTelefone() const;
    string getMorada() const;
    double getTotalComprado() const;

    // Setters
    void setIdCliente(int id);
    void setNome(string nome);
    void setTelefone(string telefone);
    void setMorada(string morada);

    // Outros métodos
    void liberarId();
    void adicionarCompra(double valor);
    string toString() const;
};
