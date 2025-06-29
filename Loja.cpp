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
}

void Loja::eliminarProduto(int idProduto)
{
    for (size_t i = 0; i < produtos.size(); i++) 
    {
        if (produtos[i].getId() == idProduto) 
        {
            produtos.erase(produtos.begin() + i);
            cout << "Produto removido.\n";
            return;
        }
    }
}

void Loja::eliminarProduto() 
{
    listarProdutos();
    int id = lernumero("Digite o ID do produto que deseja eliminar: ");
    eliminarProduto(id);
}


{
    cout << left

    cout << fixed << setprecision(2);
    {
        cout << left
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
}
bool Loja::carregarClientes(const string& caminho) {
    if (!arquivoExiste(caminho)) {
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
        return;
    }

    Venda novaVenda(idCliente);
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
            }
        }

        if (!produtoSelecionado || produtoSelecionado->getQuantidade() < quantidade)
        {
            cout << "Produto inexistente ou stock insuficiente.\n";
            continue;
        }

        produtoSelecionado->removerStock(quantidade);

        cout << "Adicionar mais produtos? (s/n): ";
        cin >> mais;
    } while (mais == 's' || mais == 'S');

    double totalVenda = novaVenda.getValorTotal();
    cout << fixed << setprecision(2);
    cout << "\nTotal a pagar: " << totalVenda << "€\n";

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
    }
    else {
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
    arquivo.imbue(locale("C"));
    if (!arquivo.is_open()) {
        return false;
    }
    
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
    if (!arquivoExiste(caminho)) 
        return false;

    ifstream arquivo(caminho);
    arquivo.imbue(locale("C"));
    if (!arquivo.is_open()) {
        return false;
    }
    
    produtos.clear();
    string linha;
    
    while (getline(arquivo, linha)) {
        istringstream iss(linha);
        // Configura o stringstream para usar locale C para números
        iss.imbue(locale("C"));
        
        string idStr, nome, qtdStr, precoStr;

        if (getline(iss, idStr, ';') &&
            getline(iss, nome, ';') &&
            getline(iss, qtdStr, ';') &&
            getline(iss, precoStr)) {

           
            try {
                // Remove espaços em branco
                precoStr.erase(precoStr.find_last_not_of(" \n\r\t") + 1);
                precoStr.erase(0, precoStr.find_first_not_of(" \n\r\t"));
                
                // Substitui vírgula por ponto se necessário
                replace(precoStr.begin(), precoStr.end(), ',', '.');

                int id = stoi(idStr);
                int quantidade = stoi(qtdStr);
                double precoCusto = stod(precoStr);

                produtos.emplace_back(id, nome, quantidade, precoCusto);

                if (id >= proximoIdProduto) {
                    proximoIdProduto = id + 1;
                }
            }
            catch (const exception& e) {
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
                        double precoCusto = precoUnit / 1.3;

                        venda.adicionarItem(nome, qtd, precoUnit);
                        venda.adicionarItem(nome, qtd, precoUnit, precoCusto);
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
    const int largura = 60;
    cout << "\n" << string(largura, '=') << "\n";
    cout << "| " << setw((largura - 4) / 2) << left << "HISTORICO DE VENDAS"
        << setw((largura - 4) / 2) << right << " |\n";
    cout << string(largura, '=') << "\n";

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

void Loja::relatorioStock() const {
    const int largura = 60;
    cout << "\n" << string(largura, '=') << "\n";
        cout << "| " << setw((largura - 4) / 2) << left 
            << "RELATORIO DE STOCK"
        << setw((largura - 4) / 2) << right << "|\n";
        cout << string(largura, '=') << "\n";

        if (produtos.empty()) {
            cout << "Nenhum produto encontrado.\n";
            return;
        }

    cout << left << setw(25) << "Produto" << "Quantidade\n";
    cout << string(45, '-') << "\n";

    for (const auto& p : produtos) {
        cout << left << setw(25) << p.getNome() << p.getQuantidade() << "\n";
    }

    cout << string(45, '=') << "\n";
}


void Loja::relatorioVendasPorProduto(const string& nomeProduto) const {
    const int largura = 78;
    cout << "\n" << string(largura, '=') << "\n";
    cout << "| " << setw((largura - 4) / 2) << left
        << "DETALHAMENTO DE VENDAS - PRODUTO: " + nomeProduto
        << setw((largura - 4) / 2) << right << " |\n";
    cout << string(largura, '=') << "\n";

    cout << left
        << "| " << setw(12) << "Fatura Nº"
        << "| " << setw(6) << "Qtd"
        << "| " << setw(12) << "Preço Unit."
        << "| " << setw(12) << "Total c/IVA"
        << "| " << setw(12) << "Custo"
        << "| " << setw(12) << "Lucro" << "|\n";
    cout << string(largura, '-') << "\n";

    bool encontrado = false;
    int totalQtd = 0;
    double totalCusto = 0, totalLucro = 0, totalComIVA = 0;

    for (const auto& venda : vendas) {
        for (const auto& item : venda.getItens()) {
            if (item.nomeProduto == nomeProduto) {
                encontrado = true;

                double precoUnitario = item.precoSemIVA / item.quantidade;
                double custoTotal = item.precoCusto * item.quantidade;
                double lucro = item.totalComIVA - custoTotal;

                cout << left
                    << "| " << setw(12) << venda.getNumeroFatura()
                    << "| " << setw(6) << item.quantidade
                    << "| " << setw(12) << fixed << setprecision(2) << precoUnitario
                    << "| " << setw(12) << item.totalComIVA
                    << "| " << setw(12) << custoTotal
                    << "| " << setw(12) << lucro << "|\n";

                totalQtd += item.quantidade;
                totalCusto += custoTotal;
                totalLucro += lucro;
                totalComIVA += item.totalComIVA;
            }
        }
    }

    if (!encontrado) {
        cout << "| Nenhuma venda encontrada para este produto.                      |\n";
    }
    else {
        cout << string(largura, '-') << "\n";
        cout << left
            << "| " << setw(12) << "TOTAL"
            << "| " << setw(6) << totalQtd
            << "| " << setw(12) << "-"
            << "| " << setw(12) << totalComIVA
            << "| " << setw(12) << totalCusto
            << "| " << setw(12) << totalLucro << "|\n";
    }

    cout << string(largura, '=') << "\n";
}

void Loja::relatorioTotalVendas() const {
    const int largura = 86;
    cout << "\n" << string(largura, '=') << "\n";
    cout << "| " << setw((largura - 4) / 2) << left << "RELATORIO TOTAL DE VENDAS"
        << setw((largura - 4) / 2) << right << " |\n";
    cout << string(largura, '=') << "\n";

    double total = 0.0;
    map<string, int> vendasPorProduto;
    map<int, double> totalPorCliente;

    for (const auto& venda : vendas) {
        total += venda.getValorTotal();
        totalPorCliente[venda.getIdCliente()] += venda.getValorTotal();
        for (const auto& item : venda.getItens()) {
            vendasPorProduto[item.nomeProduto] += item.quantidade;
        }
    }

    if (vendas.empty()) {
        cout << "Nenhuma venda registrada.\n";
        return;
    }

    string maisVendido = "N/A", menosVendido = "N/A";
    int maxQtd = -1, minQtd = INT_MAX;

    for (const auto& par : vendasPorProduto) {
        const string& produto = par.first;
        int qtd = par.second;
        if (qtd > maxQtd) {
            maxQtd = qtd;
            maisVendido = produto;
        }
        if (qtd < minQtd) {
            minQtd = qtd;
            menosVendido = produto;
        }
    }

    double lucroMaisVendido = 0.0;
    for (const auto& venda : vendas) {
        for (const auto& item : venda.getItens()) {
            if (item.nomeProduto == maisVendido) {
                lucroMaisVendido += (item.totalComIVA - (item.precoCusto * item.quantidade));
            }
        }
    }

    int idTopCliente = -1;
    double maiorCompra = -1;
    for (const auto& par : totalPorCliente) {
        int id = par.first;
        double valor = par.second;
        if (valor > maiorCompra) {
            maiorCompra = valor;
            idTopCliente = id;
        }
    }

    cout << fixed << setprecision(2);
    cout << "→ Total vendido: €" << total << "\n";
    cout << "→ Produto mais vendido: " << maisVendido << " (" << maxQtd << " unidades)\n";
    cout << "→ Produto menos vendido: " << menosVendido << " (" << minQtd << " unidades)\n";
    cout << "→ Lucro do mais vendido: €" << lucroMaisVendido << "\n";
    cout << "→ Cliente que mais comprou (ID): " << idTopCliente << " com €" << maiorCompra << "\n";
    cout << string(58, '=') << "\n";
}


void Loja::relatorioGraficoVendas() const {
    desenharCaixaTitulo("GRÁFICO DE VENDAS POR PRODUTO", 30);

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

    string maisVendido = "N/A", menosVendido = "N/A";
    int maxQtd = -1, minQtd = INT_MAX;

    for (const auto& par : vendasPorProduto) {
        const string& produto = par.first;
        int qtd = par.second;
        if (qtd > maxQtd) {
            maxQtd = qtd;
            maisVendido = produto;
        }
        if (qtd < minQtd) {
            minQtd = qtd;
            menosVendido = produto;
        }
    }

    double lucroMaisVendido = 0.0;
    for (const auto& venda : vendas) {
        for (const auto& item : venda.getItens()) {
            if (item.nomeProduto == maisVendido) {
                lucroMaisVendido += (item.totalComIVA - (item.precoCusto * item.quantidade));
            }
        }
    }

    int idTopCliente = -1;
    double maiorCompra = -1;
    for (const auto& par : totalPorCliente) {
        int id = par.first;
        double valor = par.second;
        if (valor > maiorCompra) {
            maiorCompra = valor;
            idTopCliente = id;
        }
    }

    cout << fixed << setprecision(2);
    cout << "→ Total vendido: €" << total << "\n";
    cout << "→ Produto mais vendido: " << maisVendido << " (" << maxQtd << " unidades)\n";
    cout << "→ Produto menos vendido: " << menosVendido << " (" << minQtd << " unidades)\n";
    cout << "→ Lucro do mais vendido: €" << lucroMaisVendido << "\n";
    cout << "→ Cliente que mais comprou (ID): " << idTopCliente << " com €" << maiorCompra << "\n";
    cout << string(58, '=') << "\n";
}


void Loja::relatorioVendasDetalhadoPorProduto() const {
    const int largura = 86;

    map<string, int> quantidadePorProduto;
    map<string, double> receitaPorProduto;
    map<string, double> custoPorProduto;

    for (const auto& venda : vendas) {
        for (const auto& item : venda.getItens()) {
            string nome = item.nomeProduto;
            quantidadePorProduto[nome] += item.quantidade;
            receitaPorProduto[nome] += item.totalComIVA;
            custoPorProduto[nome] += item.precoCusto * item.quantidade;
        }
    }

    if (quantidadePorProduto.empty()) {
        cout << "\nNenhuma venda registrada.\n";
        return;
    }

    cout << "\n" << string(largura, '=') << "\n";
    cout << "| " << setw((largura - 4) / 2) << left << "RELATORIO DETALHADO POR PRODUTO"
        << setw((largura - 4) / 2) << right << " |\n";
    cout << string(largura, '=') << "\n";

    cout << left
        << "| " << setw(25) << "Produto"
        << "| " << setw(6) << "Qtd"
        << "| " << setw(10) << "Preço"
        << "| " << setw(12) << "Receita"
        << "| " << setw(12) << "Custo"
        << "| " << setw(12) << "Lucro" << "|\n";
    cout << string(largura, '-') << "\n";

    double totalLucro = 0.0;

    for (const auto& par : quantidadePorProduto) {
        const string& nome = par.first;
        int qtd = par.second;
        double receita = receitaPorProduto[nome];
        double custo = custoPorProduto[nome];
        double preco = qtd > 0 ? receita / qtd : 0.0;
        double lucro = receita - custo;
        totalLucro += lucro;

        cout << left
            << "| " << setw(25) << nome
            << "| " << setw(6) << qtd
            << "| " << setw(10) << fixed << setprecision(2) << preco
            << "| " << setw(12) << receita
            << "| " << setw(12) << custo
            << "| " << setw(12) << lucro << "|\n";
    }

    cout << string(largura, '=') << "\n";
    cout << "| " << setw(70) << left << "Lucro total estimado:"
        << "€ " << fixed << setprecision(2) << totalLucro << " |\n";
    cout << string(largura, '=') << "\n";
}
void Loja::relatorioGraficoVendas() const {
    // Implemente a lógica aqui, ou apenas coloque algo provisório:
    cout << "Relatorio grafico de vendas ainda nao implementado.\n";
}
