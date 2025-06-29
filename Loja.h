#pragma once
#include <vector>
#include <string>
#include "produto.h"
#include "cliente.h"
#include "venda.h"
#include <map>

using namespace std;

class Loja {
private:
    vector<Produto> produtos;
    vector<Cliente> clientes;
    vector<Venda> vendas;
    vector<int> idsClientesDisponiveis;

    int proximoIdProduto;
    int proximoIdCliente;
    //int proximoNumeroFatura;
    //static const int qtd_Clientes = 1000;
    //int totalClientes = 0;
    int proximaPosicaoVenda;
    const int MAX_VENDAS = 100;

    // Métodos de persistência privados
    bool salvarClientes(const std::string& caminho);
    bool salvarProdutos(const std::string& caminho);
    bool salvarVendas(const std::string& caminho);
    bool carregarClientes(const std::string& caminho);
    bool carregarProdutos(const std::string& caminho);
    bool carregarVendas(const std::string& caminho);

public:
    Loja();

    // Produtos
    void criarProduto();
    void eliminarProduto();
    void criarProduto(const string& nome, int quantidade, double precoCusto);
    void adicionarStock(int idProduto, int quantidade);
    void eliminarProduto(int idProduto);
    void listarProdutos() const;

    // Clientes
    void criarCliente(const string& nome, const string& telefone, const string& morada);
    void eliminarCliente(int idCliente);
    void alterarNomeCliente(int idCliente, const string& novoNome);
    void listarClientes() const;
    

    // Carteira de clientes
    bool criarCarteiraClientes(const string& carteiraClientes = "clientes.txt") const;
    bool carregarCarteiraClientes(const string& carteiraClientes = "clientes.txt");

    // Vendas
    void efetuarVenda(int idCliente);
    void mostrarResumoVenda(const Venda& venda) const;
    
    // Relatorios
    void relatorioStock() const;
    void relatorioVendasPorProduto(const string& nomeProduto) const;
    void relatorioTotalVendas() const;
    void relatorioGraficoVendas() const;
    void relatorioVendasDetalhadoPorProduto() const;


    // Hist rico de Vendas
    void listarHistoricoVendas() const;

    // Persistência
    bool salvarDados(const string& diretorio = "dados_loja");
    bool carregarDados(const string& diretorio = "dados_loja");

    // Getters
    const vector<Produto>& getProdutos() const { return produtos; }
};
