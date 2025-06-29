
// -*-coding: utf - 8 - *-
#include "Loja.h"
#include "auxiliar.h"
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <limits>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>  

#define RESET   "\033[0m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define BOLD    "\033[1m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define GRAY "\033[90m"


using namespace std;

// Construtor
Loja::Loja() 
{
    proximoIdProduto = 1;
    proximoIdCliente = 1;
    proximaPosicaoVenda = 0;
    vendas.resize(MAX_VENDAS);
}

void Loja::criarProduto(const string& nome, int quantidade, double precoCusto) 
{
    Produto novoProduto(proximoIdProduto, nome, quantidade, precoCusto);
    produtos.push_back(novoProduto);
    cout << "Produto criado com sucesso.\n";
    proximoIdProduto++;
}

void Loja::criarProduto() 
{
    string nome;
    int quantidade;
    double preco;

    cin.ignore();  // Limpa o buffer do cin antes de ler linhas
    cout << "Nome do produto: ";
    getline(cin, nome);

    quantidade = lernumero("Quantidade: ");
    preco = lerFloatPositivo("Preço de custo: ");

    criarProduto(nome, quantidade, preco);  // Chama a versão já implementada
}


void Loja::adicionarStock(int idProduto, int quantidade) 
{
    for (size_t i = 0; i < produtos.size(); i++) 
    {
        if (produtos[i].getId() == idProduto) 
        {
            produtos[i].adicionarStock(quantidade);
            cout << "Stock adicionado com sucesso!\n";
            return;
        }
    }
    cout << "Produto não encontrado.\n";
}

void Loja::eliminarProduto(int idProduto)
{
    //listarProdutos();
    for (size_t i = 0; i < produtos.size(); i++) 
    {
        if (produtos[i].getId() == idProduto) 
        {
            produtos.erase(produtos.begin() + i);
            cout << "Produto removido.\n";
            //listarProdutos();
            return;
        }
    }
    cout << "Produto não encontrado.\n";
}

void Loja::eliminarProduto() 
{
    listarProdutos();
    int id = lernumero("Digite o ID do produto que deseja eliminar: ");
    eliminarProduto(id);
}


void Loja::listarProdutos() const 
{
    cout << "\n-------------------------- Produtos Disponíveis --------------------------\n";
    cout << left
        << setw(6) << "ID"
        << setw(30) << "Nome"
        << setw(10) << "Qtde"
        << setw(15) << "Preço de Custo "
        << setw(15) << "Preço de Venda"
        << "\n";
    cout << string(76, '-') << "\n";

    cout << fixed << setprecision(2);
    for (const auto& p : produtos) 
    {
        cout << left
            << setw(6) << p.getId()
            << setw(30) << p.getNome()
            << setw(10) << p.getQuantidade()
            << setw(15) << p.getPrecoCusto()
            << setw(15) << p.getPrecoVenda()
            << "\n";
    }
}

void Loja::criarCliente(const string& nome, const string& telefone, const string& morada) 
{
    int novoId;

    if (!idsClientesDisponiveis.empty()) {
        novoId = idsClientesDisponiveis.back();
        idsClientesDisponiveis.pop_back();
    }
    else {
        // Encontra o próximo ID disponível
        vector<int> idsExistentes;
        for (const auto& c : clientes) {
            idsExistentes.push_back(c.getIdCliente());
        }

        if (idsExistentes.empty()) {
            novoId = 1;
        }
        else {
            sort(idsExistentes.begin(), idsExistentes.end());
            novoId = idsExistentes.back() + 1;
        }
    }

    Cliente novoCliente(nome, telefone, morada);
    novoCliente.setIdCliente(novoId);
    clientes.push_back(novoCliente);

    // Mantém a lista de clientes ordenada por ID
    sort(clientes.begin(), clientes.end(),
        [](const Cliente& a, const Cliente& b) {
            return a.getIdCliente() < b.getIdCliente();
        });

    cout << "Cliente criado com ID: " << novoId << "\n";

}

void Loja::eliminarCliente(int idCliente) 
{
    for (size_t i = 0; i < clientes.size(); i++) 
    {
        if (clientes[i].getIdCliente() == idCliente) 
        {
            idsClientesDisponiveis.push_back(idCliente);
            sort(idsClientesDisponiveis.begin(), idsClientesDisponiveis.end());
            clientes.erase(clientes.begin() + i);
            cout << "Cliente removido. ID " ;
            return;
        }
    }
    cout << "Cliente não encontrado.\n";
}

void Loja::alterarNomeCliente(int idCliente, const string& novoNome) 
{
    for (size_t i = 0; i < clientes.size(); i++) 
    {
        if (clientes[i].getIdCliente() == idCliente) 
        {
            clientes[i].setNome(novoNome);
            cout << "Nome alterado.\n";
            return;
        }
    }
    cout << "Cliente não encontrado.\n";
}
bool Loja::carregarClientes(const string& caminho) {
    if (!arquivoExiste(caminho)) {
        // Se arquivo não existe, limpa lista de IDs disponíveis
        idsClientesDisponiveis.clear();
        return false;
    }

    ifstream arquivo(caminho);
    if (!arquivo.is_open()) {
        return false;
    }

    clientes.clear();
    idsClientesDisponiveis.clear();

    string linha;
    bool primeiraLinha = true;

    while (getline(arquivo, linha)) {
        if (primeiraLinha && linha.find("#IDs_DISPONIVEIS:") != string::npos) {
            // Processa lista de IDs disponíveis
            string idsStr = linha.substr(17);
            istringstream idsStream(idsStr);
            string idStr;

            while (getline(idsStream, idStr, ',')) {
                if (!idStr.empty()) {
                    try {
                        idsClientesDisponiveis.push_back(stoi(idStr));
                    }
                    catch (...) {
                        continue; // Ignora IDs inválidos
                    }
                }
            }
            primeiraLinha = false;
            continue;
        }

        primeiraLinha = false;

        // Processa dados do cliente
        istringstream iss(linha);
        string idStr, nome, telefone, morada, totalStr;

        if (getline(iss, idStr, ';') &&
            getline(iss, nome, ';') &&
            getline(iss, telefone, ';') &&
            getline(iss, morada, ';') &&
            getline(iss, totalStr)) {

            try {
                int id = stoi(idStr);
                double totalComprado = stof(totalStr);

                Cliente cliente(nome, telefone, morada, totalComprado);
                cliente.setIdCliente(id);
                clientes.push_back(cliente);
            }
            catch (...) {
                continue; // Ignora linhas corrompidas
            }
        }
    }

    arquivo.close();

    // Ordena clientes por ID para consistência
    sort(clientes.begin(), clientes.end(),
        [](const Cliente& a, const Cliente& b) {
            return a.getIdCliente() < b.getIdCliente();
        });

    return true;
}

void Loja::listarClientes() const {
    // Ordena clientes por ID antes de exibir
    vector<Cliente> clientesOrdenados = clientes;
    sort(clientesOrdenados.begin(), clientesOrdenados.end(),
        [](const Cliente& a, const Cliente& b) {
            return a.getIdCliente() < b.getIdCliente();
        });
    cout << "\n--- Lista de Clientes (Ordenada por ID) ---\n";
    for (const auto& c : clientesOrdenados) 
    {
        cout << "ID: " << c.getIdCliente()
            << " | Nome: " << c.getNome()
            << " | Telefone: " << c.getTelefone()
            << " | Morada: " << c.getMorada()
            << "\n";
    }
}

bool Loja::criarCarteiraClientes(const string& carteiraClientes) const 
{
    ofstream arquivo(carteiraClientes);
    if (!arquivo.is_open()) 
    {
        cout << "Erro ao abrir o arquivo para escrita: " << carteiraClientes << "\n";
        return false;
    }

    for (const auto& cliente : clientes) 
    {
        arquivo << cliente.getNome() << ";"
            << cliente.getTelefone() << ";"
            << cliente.getMorada() << "\n";
    }

    arquivo.close();
    cout << "Carteira de clientes salva com sucesso no arquivo '" << carteiraClientes << "'.\n";
    return true;
}

bool Loja::carregarCarteiraClientes(const string& carteiraClientes) 
{
    ifstream arquivo(carteiraClientes);
    if (!arquivo.is_open()) 
    {
        cout << "Erro ao abrir o arquivo: " << carteiraClientes << "\n";
        return false;
    }

    clientes.clear();
    string linha;
    while (getline(arquivo, linha)) 
    {
        istringstream iss(linha);
        string nome, telefone, morada;

        if (getline(iss, nome, ';') && getline(iss, telefone, ';') && getline(iss, morada)) {
            criarCliente(nome, telefone, morada);
        }
    }

    arquivo.close();
    cout << "Clientes carregados com sucesso do arquivo '" << carteiraClientes << "'.\n";
    return true;
}

void Loja::efetuarVenda(int idCliente)
{
    Cliente* clienteEncontrado = nullptr;
    for (auto& c : clientes)
    {
        if (c.getIdCliente() == idCliente)
        {
            clienteEncontrado = &c;
            break;
        }
    }

    if (!clienteEncontrado)
    {
        cout << "Cliente não encontrado.\n";
        return;
    }

    Venda novaVenda(idCliente);
    //proximoNumeroFatura++;

    listarProdutos();

    char mais;
    do
    {
        int idProduto = lernumero("ID do produto: ");
        int quantidade = lernumero("Quantidade: ");

        Produto* produtoSelecionado = nullptr;
        for (auto& p : produtos)
        {
            if (p.getId() == idProduto)
            {
                produtoSelecionado = &p;
                break;
            }
        }

        if (!produtoSelecionado || produtoSelecionado->getQuantidade() < quantidade)
        {
            cout << "Produto inexistente ou stock insuficiente.\n";
            continue;
        }

        novaVenda.adicionarItem(produtoSelecionado->getNome(), quantidade, produtoSelecionado->getPrecoVenda());
        produtoSelecionado->removerStock(quantidade);

        cout << "Adicionar mais produtos? (s/n): ";
        cin >> mais;
    } while (mais == 's' || mais == 'S');

    double totalVenda = novaVenda.getValorTotal();
    cout << fixed << setprecision(2);
    cout << "\nTotal a pagar: " << totalVenda << "€\n";
    double valorEntregue = lerFloatPositivo("Valor entregue pelo cliente: ");

    novaVenda.finalizarVenda(valorEntregue);
    novaVenda.imprimirTalao();
    clienteEncontrado->adicionarCompra(totalVenda);

    vendas[proximaPosicaoVenda] = novaVenda;
    proximaPosicaoVenda = (proximaPosicaoVenda + 1) % MAX_VENDAS;
}

bool Loja::salvarDados(const string& diretorio) {
    criarDiretorioSeNaoExistir(diretorio);

    bool sucesso = true;
    sucesso &= salvarClientes(diretorio + "/clientes.txt");
    sucesso &= salvarProdutos(diretorio + "/produtos.txt");
    sucesso &= salvarVendas(diretorio + "/vendas.txt");

    if (sucesso) {
        cout << GREEN << "Dados salvos com sucesso no diretório '" << diretorio << "'" << RESET << endl;
    }
    else {
        cout << RED << "Erro ao salvar alguns dados." << RESET << endl;
    }

    return sucesso;
}

bool Loja::carregarDados(const string& diretorio) {
    bool sucesso = true;
    sucesso &= carregarClientes(diretorio + "/clientes.txt");
    sucesso &= carregarProdutos(diretorio + "/produtos.txt");
    sucesso &= carregarVendas(diretorio + "/vendas.txt");

    if (sucesso) {
        cout << GREEN << "Dados carregados com sucesso do diretório '" << diretorio << "'" << RESET << endl;
    }
    else {
        cout << YELLOW << "Aviso: Alguns dados não puderam ser carregados." << RESET << endl;
    }

    return sucesso;
}

bool Loja::salvarClientes(const string& caminho) {
    ofstream arquivo(caminho);
    if (!arquivo.is_open()) {
        return false;
    }
    // Primeiro salva os IDs disponíveis
    arquivo << "#IDs_DISPONIVEIS:";
    for (int id : idsClientesDisponiveis) {
        arquivo << id << ",";
    }
    arquivo << "\n";

    for (const auto& cliente : clientes) {
        arquivo << cliente.getIdCliente() << ";"
            << cliente.getNome() << ";"
            << cliente.getTelefone() << ";"
            << cliente.getMorada() << ";"
            << fixed << setprecision(2)
            << cliente.getTotalComprado() << "\n";
    }

    arquivo.close();
    return true;
}

bool Loja::salvarProdutos(const string& caminho) {
    ofstream arquivo(caminho);
    if (!arquivo.is_open()) {
        return false;
    }
    arquivo.imbue(locale::classic());
    arquivo << fixed << setprecision(2);
    for (const auto& produto : produtos) {
        arquivo << produto.getId() << ";"
            << produto.getNome() << ";"
            << produto.getQuantidade() << ";" 
            << produto.getPrecoCusto() << "\n";
    }

    ofstream idArquivo(caminho + ".id");
    if (idArquivo.is_open()) {
        idArquivo << proximoIdProduto;
        idArquivo.close();
    }

    arquivo.close();
    return true;
}

bool Loja::salvarVendas(const string& caminho) {
    ofstream arquivo(caminho);
    if (!arquivo.is_open()) {
        return false;
    }

    arquivo << proximaPosicaoVenda << "\n";

    for (int i = 0; i < MAX_VENDAS; i++) {
        int pos = (proximaPosicaoVenda + i) % MAX_VENDAS;
        const Venda& venda = vendas[pos];

        if (venda.getNumeroFatura().empty()) {
            continue;
        }

        arquivo << venda.getNumeroFatura() << ";"
            << venda.getIdCliente() << ";"
            << fixed << setprecision(2)
            << venda.getValorTotal() << ";"
            << venda.getTroco() << ";";

        const auto& itens = venda.getItens();
        for (size_t j = 0; j < itens.size(); j++) {
            const auto& item = itens[j];
            arquivo << item.nomeProduto << ","
                << item.quantidade << ","
                << fixed << setprecision(2)
                << item.precoSemIVA << ","
                << item.iva << ","
                << item.totalComIVA;

            if (j < itens.size() - 1) {
                arquivo << "|";
            }
        }

        arquivo << "\n";
    }

    arquivo.close();
    return true;
}

bool Loja::carregarProdutos(const string& caminho) {
    if (!arquivoExiste(caminho)) return false;

    ifstream arquivo(caminho);
    if (!arquivo.is_open()) {
        return false;
    }
    arquivo.imbue(locale::classic());

    produtos.clear();
    string linha;
    while (getline(arquivo, linha)) {
        istringstream iss(linha);
        string idStr, nome, qtdStr, precoStr;

        if (getline(iss, idStr, ';') &&
            getline(iss, nome, ';') &&
            getline(iss, qtdStr, ';') &&
            getline(iss, precoStr)) {

           
            try {
                // Limpeza de espaços e substituição de vírgula por ponto
                precoStr.erase(precoStr.find_last_not_of(" \n\r\t") + 1);
                precoStr.erase(0, precoStr.find_first_not_of(" \n\r\t"));
                replace(precoStr.begin(), precoStr.end(), ',', '.');

                int id = stoi(idStr);
                int quantidade = stoi(qtdStr);
                double precoCusto = stod(precoStr);

                produtos.emplace_back(id, nome, quantidade, precoCusto);

                if (id >= proximoIdProduto) {
                    proximoIdProduto = id + 1;
                }
            }
            catch (...) {
                continue;
            }
        }
    }

    arquivo.close();

    ifstream idArquivo(caminho + ".id");
    if (idArquivo.is_open()) {
               
        idArquivo >> proximoIdProduto;
        idArquivo.close();
             
    }

   
    return true;
}

bool Loja::carregarVendas(const string& caminho) {
    if (!arquivoExiste(caminho)) return false;

    ifstream arquivo(caminho);
    if (!arquivo.is_open()) {
        return false;
    }

    vendas.clear();
    vendas.resize(MAX_VENDAS);
    proximaPosicaoVenda = 0;

    string primeiraLinha;
    if (getline(arquivo, primeiraLinha)) {
        try {
            proximaPosicaoVenda = stoi(primeiraLinha) % MAX_VENDAS;
        }
        catch (...) {
            proximaPosicaoVenda = 0;
        }
    }

    string linha;
    while (getline(arquivo, linha)) {
        istringstream iss(linha);
        string nFatura, idClienteStr, totalStr, trocoStr, itensStr;

        if (getline(iss, nFatura, ';') &&
            getline(iss, idClienteStr, ';') &&
            getline(iss, totalStr, ';') &&
            getline(iss, trocoStr, ';') &&
            getline(iss, itensStr)) {

            try {
                int idCliente = stoi(idClienteStr);
                double total = stod(totalStr);
                double troco = stod(trocoStr);

                Venda venda(idCliente);
                venda.finalizarVenda(total + troco);

                vector<string> itensVenda;
                istringstream itemsIss(itensStr);
                string itemToken;
                while (getline(itemsIss, itemToken, '|')) {
                    istringstream itemIss(itemToken);
                    string nome, qtdStr, precoStr, ivaStr, totalItemStr;

                    if (getline(itemIss, nome, ',') &&
                        getline(itemIss, qtdStr, ',') &&
                        getline(itemIss, precoStr, ',') &&
                        getline(itemIss, ivaStr, ',') &&
                        getline(itemIss, totalItemStr)) {

                        int qtd = stoi(qtdStr);
                        double precoUnit = stod(precoStr);

                        venda.adicionarItem(nome, qtd, precoUnit);
                    }
                }

                vendas[proximaPosicaoVenda] = venda;
                proximaPosicaoVenda = (proximaPosicaoVenda + 1) % MAX_VENDAS;
            }
            catch (...) {
                continue;
            }
        }
    }

    arquivo.close();
    return true;
}

void Loja::listarHistoricoVendas() const {
    cout << CYAN << "\n╔══════════════════════════════╗\n";
    cout << BOLD << "       HISTÓRICO DE VENDAS" << RESET << CYAN << "\n╚══════════════════════════════╝\n" << RESET;
    const int largura = 60;

    bool encontrouVendas = false;

    for (int i = MAX_VENDAS - 1; i >= 0; i--) {
        int pos = (proximaPosicaoVenda + i) % MAX_VENDAS;
        const Venda& venda = vendas[pos];

        if (venda.getNumeroFatura().empty()) {
            continue;
        }

        encontrouVendas = true;

        cout << "\n" << string(largura, '-') << "\n";
        cout << "Fatura Nº: " << venda.getNumeroFatura()
            << " | Cliente ID: " << venda.getIdCliente() << "\n";
        cout << "Total da Venda: " << fixed << setprecision(2)
            << venda.getValorTotal() << "€\n";

        cout << string(largura, '-') << "\n";
        cout << left << setw(25) << "Produto"
            << setw(10) << "Qtd"
            << setw(15) << "Total c/IVA" << "\n";
        cout << string(largura, '-') << "\n";

        for (const ItemVenda& item : venda.getItens()) {
            cout << left << setw(25) << item.nomeProduto
                << setw(10) << item.quantidade
                << fixed << setprecision(2) << item.totalComIVA << "€\n";
        }
    }

    if (!encontrouVendas) {
        cout << "Nenhuma venda registrada.\n";
    }
}
//void Loja::listarHistoricoVendas() const {
//    cout << CYAN << "\n╔══════════════════════════════╗\n";
//    cout << BOLD << "       HISTÓRICO DE VENDAS" << RESET << CYAN << "\n╚══════════════════════════════╝\n" << RESET;
//    const int largura = 60;
//    if (vendas.empty()) {
//        cout << "Nenhuma venda registrada.\n";
//        return;
//    }
//
//    for (const Venda& venda : vendas) {
//        cout << "\n" << string(largura, '-') << "\n";
//        cout << "Fatura Nº: " << venda.getNumeroFatura()
//            << " | Cliente ID: " << venda.getIdCliente() << "\n";
//        cout << "Total da Venda: " << fixed << setprecision(2)
//            << venda.getValorTotal() << "€\n";
//
//        cout << string(largura, '-') << "\n";
//        cout << left << setw(25) << "Produto"
//            << setw(10) << "Qtd"
//            << setw(15) << "Total c/IVA" << "\n";
//        cout << string(largura, '-') << "\n";
//
//        for (const ItemVenda& item : venda.getItens()) {
//            cout << left << setw(25) << item.nomeProduto
//                << setw(10) << item.quantidade
//                << fixed << setprecision(2) << item.totalComIVA << "€\n";
//        }
//    }
//
//}


void Loja::relatorioStock() const {
    cout << CYAN << "\n╔═════════════════════════════════════╗\n";
    cout << BOLD << "     RELATÓRIO DE STOCK" << RESET << CYAN << "\n╚═════════════════════════════════════╝\n" << RESET;

    if (produtos.empty()) {
        cout << RED << "Nenhum produto encontrado.\n" << RESET;
        return;
    }

    cout << GREEN << setw(25) << left << "Produto" << "Quantidade\n" << RESET;
    for (const auto& p : produtos) {
        cout << setw(25) << p.getNome() << p.getQuantidade() << "\n";
    }
}


void Loja::relatorioVendasPorProduto(const string& nomeProduto) const {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    const int largura = 66;
    string titulo = "DETALHAMENTO DE VENDAS - PRODUTO: " + nomeProduto;
    int espacoEsq = (largura - titulo.size()) / 2;
    int espacoDir = largura - titulo.size() - espacoEsq;

    cout << CYAN << "\n╔══════════════════════════════════════════════════════════════╗\n";
    cout << "║" << string(espacoEsq, ' ') << BOLD << titulo << RESET << CYAN << string(espacoDir, ' ') << "║\n";
        cout << "\n╚══════════════════════════════════════════════════════════════╝\n" << RESET;
        

    cout << BOLD << left
        << setw(12) << "Fatura Nº"
        << setw(10) << "Qtd"
        << setw(12) << "Preço"
        << setw(12) << "Total c/IVA"
        << setw(10) << "Custo"
        << setw(10) << "Lucro"
        << RESET << "\n";

    cout << string(largura, '-') << "\n";

    bool encontrado = false;
    int totalQtd = 0;
    double totalCusto = 0, totalLucro = 0, totalComIVA = 0;

    for (const auto& venda : vendas) {
        for (const auto& item : venda.getItens()) {
            if (item.nomeProduto == nomeProduto) {
                encontrado = true;

                double precoUnitario = item.precoSemIVA / item.quantidade;
                double custoUnit = precoUnitario / 1.3f;
                double custoTotal = custoUnit * item.quantidade;
                double lucro = (item.precoSemIVA - custoTotal);

                cout << left
                    << setw(12) << venda.getNumeroFatura()
                    << setw(10) << item.quantidade
                    << setw(12) << fixed << setprecision(2) << precoUnitario
                    << setw(12) << item.totalComIVA
                    << setw(10) << custoTotal
                    << setw(10) << lucro
                    << "\n";

                totalQtd += item.quantidade;
                totalCusto += custoTotal;
                totalLucro += lucro;
                totalComIVA += item.totalComIVA;
            }
        }
    }

    if (!encontrado) {
        cout << RED << "Nenhuma venda encontrada para este produto.\n" << RESET;
    }
    else {
        cout << string(largura, '-') << "\n";
        cout << BOLD
            << setw(12) << "TOTAL"
            << setw(10) << totalQtd
            << setw(12) << "-"
            << setw(12) << totalComIVA
            << setw(10) << totalCusto
            << setw(10) << totalLucro
            << RESET << "\n";
    }

}


void Loja::relatorioTotalVendas() const {
    cout << CYAN << "\n╔══════════════════════════════════════════╗\n";
    cout << BOLD << "      RELATÓRIO TOTAL DE VENDAS" << RESET << CYAN << "\n╚══════════════════════════════════════════╝\n" << RESET;

    double total = 0.0f;
    map<string, int> vendasPorProduto;
    map<int, double> totalPorCliente;

    // 1. Calcular total vendido e contagens
    for (const auto& venda : vendas) {
        total += venda.getValorTotal();
        totalPorCliente[venda.getIdCliente()] += venda.getValorTotal();

        for (const auto& item : venda.getItens()) {
            vendasPorProduto[item.nomeProduto] += item.quantidade;
        }
    }

    // 2. Produto mais e menos vendido
    string maisVendido = "N/A", menosVendido = "N/A";
    int maxQtd = -1, minQtd = INT_MAX;
    for (const auto& par : vendasPorProduto) {
        if (par.second > maxQtd) {
            maxQtd = par.second;
            maisVendido = par.first;
        }
        if (par.second < minQtd) {
            minQtd = par.second;
            menosVendido = par.first;
        }
    }

    // 3. Lucro do produto mais vendido
    double lucroMaisVendido = 0.0f;
    for (const auto& p : produtos) {
        if (p.getNome() == maisVendido) {
            double precoCusto = p.getPrecoCusto();
            double precoVendaComIVA = p.getPrecoVendaComIVA();
            lucroMaisVendido = maxQtd * (precoVendaComIVA - precoCusto);
            break;
        }
    }

    // 4. Cliente que mais comprou
    int idTopCliente = -1;
    double maiorCompra = -1;
    for (const auto& par : totalPorCliente) {
        if (par.second > maiorCompra) {
            maiorCompra = par.second;
            idTopCliente = par.first;
        }
    }

    // 5. Exibir resultados
    cout << GREEN << fixed << setprecision(2);
    cout << "\n→ Total vendido: €" << total;
    cout << "\n→ Produto mais vendido: " << maisVendido << " (" << maxQtd << " unidades)";
    cout << "\n→ Produto menos vendido: " << menosVendido << " (" << minQtd << " unidades)";
    cout << "\n→ Lucro estimado do mais vendido: €" << lucroMaisVendido;
    cout << "\n→ Cliente que mais comprou (ID): " << idTopCliente << " com €" << maiorCompra << RESET << "\n";
}


void Loja::relatorioGraficoVendas() const {
    cout << CYAN << "\n╔══════════════════════════════╗\n";
    cout << BOLD << "  GRÁFICO DE VENDAS POR PRODUTO" << RESET << CYAN << "\n╚══════════════════════════════╝\n" << RESET;

    // Mapeia vendas por produto
    map<string, double> totalPorProduto;
    for (const auto& venda : vendas) {
        for (const auto& item : venda.getItens()) {
            totalPorProduto[item.nomeProduto] += item.totalComIVA;
        }
    }

    // Nada para exibir
    if (totalPorProduto.empty()) {
        cout << RED << "Nenhuma venda registrada.\n" << RESET;
        return;
    }

    // Descobre valor máximo
    double maxValor = 0.0f;
    for (const auto& p : totalPorProduto) {
        if (p.second > maxValor)
            maxValor = p.second;
    }

    // Ordena por valor (decrescente)
    vector<pair<string, double>> produtosOrdenados(totalPorProduto.begin(), totalPorProduto.end());
    sort(produtosOrdenados.begin(), produtosOrdenados.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

    // Mostra gráfico
    for (const auto& p : produtosOrdenados) {
        int barras = static_cast<int>((p.second / maxValor) * 30); // tamanho proporcional
        cout << setw(20) << left << p.first << " │ "
            << GRAY << string(barras, '#') << RESET
            << " " << fixed << setprecision(2) << p.second << "€\n";
    }
}
void Loja::relatorioVendasDetalhadoPorProduto() const {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    map<string, int> quantidadePorProduto;
    map<string, double> receitaPorProduto;

    // Agrupa vendas
    for (const auto& venda : vendas) {
        for (const auto& item : venda.getItens()) {
            quantidadePorProduto[item.nomeProduto] += item.quantidade;
            receitaPorProduto[item.nomeProduto] += item.totalComIVA;
        }
    }

    if (quantidadePorProduto.empty()) {
        cout << RED << "Nenhuma venda registrada.\n" << RESET;
        return;
    }

    // Cabeçalho
    cout << CYAN << "\n╔══════════════════════════════════════════════════════════════╗\n";
    cout << BOLD << "                  RELATÓRIO DETALHADO POR PRODUTO" << RESET << CYAN 
        << "\n╚══════════════════════════════════════════════════════════════╝\n" << RESET;

    cout << BOLD;
    cout << left << setw(20) << "Produto"
        << setw(8) << "Qtde"
        << setw(10) << "Preço"
        << setw(10) << "Receita"
        << setw(10) << "Custo"
        << setw(10) << "Lucro" << RESET << "\n";

    double totalLucro = 0.0;

    for (const auto& par : quantidadePorProduto) {
        const string& nome = par.first;
        int qtd = par.second;
        double receita = receitaPorProduto[nome];
        double preco = 0.0, custo = 0.0;

        // Busca produto na lista original para pegar os preços
        for (const auto& p : produtos) {
            if (p.getNome() == nome) {
                preco = p.getPrecoVendaComIVA();
                custo = p.getPrecoCusto();
                break;
            }
        }

        double custoTotal = qtd * custo;
        double lucro = receita - custoTotal;
        totalLucro += lucro;

        // Imprime linha
        cout << left << setw(20) << nome
            << setw(8) << qtd
            << setw(10) << fixed << setprecision(2) << preco
            << setw(10) << receita
            << setw(10) << custoTotal
            << setw(10) << lucro << "\n";
    }

    // Total
    cout << BOLD << "\nLucro total estimado: " << GREEN << fixed << setprecision(2) << totalLucro << "€" << RESET << "\n";
}
