#pragma once
#include <vector>
#include <string>
#include "Produto.h"
#include "Cliente.h"
#include "Venda.h"
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

    // Metodos de persistencia privados
    bool salvarClientes(const string& caminho);
    bool salvarProdutos(const string& caminho);
    bool salvarVendas(const string& caminho);
    bool carregarClientes(const string& caminho);
    bool carregarProdutos(const string& caminho);
    bool carregarVendas(const string& caminho);

public:
    Loja();

    // Produtos
    bool eliminarProduto(int idProduto);
    void criarProduto(const string& nome, int quantidade, double precoCusto);
    void adicionarStock(int idProduto, int quantidade);
    void listarProdutos() const;

    // Clientes
    void criarCliente(const string& nome, const string& telefone, const string& morada, const string& dataNascimento);
    void eliminarCliente(int idCliente);
    void alterarNomeCliente(int idCliente, const string& novoNome);
    void listarClientes() const;
    

    // Vendas
    void efetuarVenda();
    void mostrarResumoVenda(const Venda& venda) const;
    
    // Relatorios
    void relatorioStock() const;
    bool relatorioVendasPorProduto(const string& nomeProduto) const;
    void relatorioTotalVendas() const;
    void relatorioGraficoVendas() const;
    void relatorioVendasDetalhadoPorProduto() const;


    // Hist rico de Vendas
    void listarHistoricoVendas() const;

    // Persistência
    bool salvarDados(const string& diretorio = "dados_loja");
    bool carregarDados(const string& diretorio = "dados_loja");

    // Getters
    std::vector<Produto>& getProdutos() { return produtos; }
    std::vector<Cliente>& getClientes() { return clientes; }

    // Repor estoque ao remover item ou cancelar venda
    void reporEstoqueItem(const ItemVenda& item);
    void reporEstoqueVenda(const Venda& venda);
};
