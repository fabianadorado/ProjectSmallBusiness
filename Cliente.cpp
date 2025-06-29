#include "Cliente.h"
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;


int Cliente::proximoCliente = 1;
vector<int> Cliente::idsDisponiveis;

// Construtor padrão
Cliente::Cliente()
    : idCliente(proximoCliente++), nome(""), telefone(""), morada(""), totalComprado(0.0f) {
}

// Construtor com parâmetros
Cliente::Cliente(string nome, string telefone, string morada, double totalComprado)
    : idCliente(proximoCliente++), nome(nome), telefone(telefone), morada(morada), totalComprado(totalComprado) {
    // Verifica se há IDs disponíveis para reutilização
    if (!idsDisponiveis.empty()) {
        idCliente = idsDisponiveis.back();
        idsDisponiveis.pop_back();
    }
    else {
        idCliente = proximoCliente++;
    }

    this->nome = nome;
    this->telefone = telefone;
    this->morada = morada;
    this->totalComprado = totalComprado;
}

// Getters
int Cliente::getIdCliente() const {
    return idCliente;
}

string Cliente::getNome() const {
    return nome;
}

string Cliente::getTelefone() const {
    return telefone;
}

string Cliente::getMorada() const {
    return morada;
}

double Cliente::getTotalComprado() const {
    return totalComprado;
}

// Setters
void Cliente::setIdCliente(int id) {
    idCliente = id;
    if (id >= proximoCliente) {
        proximoCliente = id + 1;
    }
}

void Cliente::setNome(string nome) {
    this->nome = nome;
}

void Cliente::setTelefone(string telefone) {
    this->telefone = telefone;
}

void Cliente::setMorada(string morada) {
    this->morada = morada;
}

// Método para liberar ID quando cliente é removido
void Cliente::liberarId() {
    idsDisponiveis.push_back(idCliente);
    sort(idsDisponiveis.begin(), idsDisponiveis.end());
}

void Cliente::adicionarCompra(double valor) {
    totalComprado += valor;
}

string Cliente::toString() const {
    stringstream dadosCliente;
    dadosCliente << "ID: " << idCliente
        << " | Nome: " << nome
        << " | Telefone: " << telefone
        << " | Morada: " << morada;
    return dadosCliente.str();
}