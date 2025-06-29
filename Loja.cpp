#define NOMINMAX
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
    preco = lerFloatPositivo("Preco de custo: ");

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
    cout << "Produto nao encontrado.\n";
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
    cout << "Produto nao encontrado.\n";
}

void Loja::eliminarProduto() 
{
    listarProdutos();
    int id = lernumero("Digite o ID do produto que deseja eliminar: ");
    eliminarProduto(id);
}


void Loja::listarProdutos() const
{
    const string margem = "    ";
    std::vector<std::string> cabecalho = {"ID", "Nome", "Qtd", "Preco Custo", "Preco Venda"};
    std::vector<int> larguras = {4, 28, 8, 13, 13};
    cout << margem << "\n";
    imprimirLinhaTabela(cabecalho, larguras);
    cout << margem << "+" << string(4 + 28 + 8 + 13 + 13 + 6 * 3, '-') << "+" << endl;
    for (const auto& p : produtos) {
        std::vector<std::string> linha = {
            to_string(p.getId()),
            p.getNome(),
            to_string(p.getQuantidade()),
            (ostringstream{} << fixed << setprecision(2) << p.getPrecoCusto() << " EUR").str(),
            (ostringstream{} << fixed << setprecision(2) << p.getPrecoVenda() << " EUR").str()
        };
        imprimirLinhaTabela(linha, larguras);
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
    cout << "Cliente nao encontrado.\n";
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
    cout << "Cliente nao encontrado.\n";
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

    const int larguraTotal = 86;

    imprimirTituloCentralizado("LISTA DE CLIENTES", larguraTotal);

    // Cabeçalho
    cout << left
        << "| " << setw(4) << "ID"
        << "| " << setw(28) << "Nome"
        << "| " << setw(15) << "Telefone"
        << "| " << setw(30) << "Morada"
        << "|\n";

    cout << string(larguraTotal, '-') << "\n";

    if (clientesOrdenados.empty()) {
        cout << "| " << setw(larguraTotal - 4) << left << "Nenhum cliente cadastrado." << " |\n";
    }
    else {
        for (const auto& c : clientesOrdenados) {
            cout << left
                << "| " << setw(4) << c.getIdCliente()
                << "| " << setw(28) << c.getNome()
                << "| " << setw(15) << c.getTelefone()
                << "| " << setw(30) << c.getMorada()
                << "|\n";
        }
    }

    cout << "+" << string(larguraTotal - 2, '=') << "+" << endl;
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
        cout << "Cliente nao encontrado.\n";
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

        novaVenda.adicionarItem(
            produtoSelecionado->getNome(),
            quantidade,
            produtoSelecionado->getPrecoVenda(),
            produtoSelecionado->getPrecoCusto()
        );

        produtoSelecionado->removerStock(quantidade);

        cout << "Adicionar mais produtos? (s/n): ";
        cin >> mais;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer
    } while (mais == 's' || mais == 'S');

    double totalVenda = novaVenda.getValorTotal();
    cout << fixed << setprecision(2);
    cout << "\nTotal a pagar: " << totalVenda << "€\n";

    //Sorteio do talao gratis (10% de chance)
    srand(static_cast<unsigned>(time(0)));
    bool ganhouTalaoGratis = (rand() % 10 == 0); 

    double valorFinal = ganhouTalaoGratis ? 0.0 : totalVenda;

    if (ganhouTalaoGratis) {
        cout << GREEN << "PARABENS! Voce ganhou o talao gratis!\n" << RESET;
    }

    double valorEntregue = 0.0;
    if (ganhouTalaoGratis) {
        valorEntregue = 0.0;
    } else {
        bool valorValido = false;
        while (!valorValido) {
            bool primeiraTentativa = true;
            while (true) {
                if (!primeiraTentativa) {
                    cout << RED << "Entrada inválida. Digite um número válido (>= 0)." << RESET << endl;
                }
                cout << "Valor entregue pelo cliente: ";
                string input;
                getline(cin, input);
                replace(input.begin(), input.end(), ',', '.');
                istringstream iss(input);
                if (iss >> valorEntregue && valorEntregue >= 0.0) {
                    char extra;
                    if (!(iss >> extra)) break;
                }
                primeiraTentativa = false;
            }
            try {
                novaVenda.finalizarVenda(valorEntregue);
                valorValido = true;
            } catch (const std::invalid_argument& e) {
                cout << RED << e.what() << RESET << endl;
                // Repete o loop para pedir o valor novamente
            }
        }
    }

    mostrarResumoVenda(novaVenda);
    cout << "\nPressione Enter para ver o talão...";
    limparBuffer();
    cin.get();
    system("cls");
    novaVenda.imprimirTalao();
    cout << "\nPressione Enter para voltar...";
    cin.get();
    system("cls");

    if (!ganhouTalaoGratis)
        clienteEncontrado->adicionarCompra(totalVenda);

    vendas[proximaPosicaoVenda] = novaVenda;
    proximaPosicaoVenda = (proximaPosicaoVenda + 1) % MAX_VENDAS;
    salvarVendas("dados_loja/vendas.txt");
}

void Loja::mostrarResumoVenda(const Venda& venda) const {
    const int largura = 76;
    const string margem = "    "; // 4 espaços
    string titulo = "RESUMO DA VENDA";
    int espaco = largura - 4 - titulo.length();
    int esq = espaco / 2;
    int dir = espaco - esq;
    cout << margem << "+" << string(largura - 2, '=') << "+" << endl;
    cout << margem << "|" << string(esq, ' ') << titulo << string(dir, ' ') << "|" << endl;
    cout << margem << "+" << string(largura - 2, '=') << "+" << endl;

    ostringstream ossId;
    ossId << "ID do cliente: " << venda.getIdCliente();
    cout << margem << "| " << left << setw(largura - 4) << ossId.str() << " |" << endl;
    cout << margem << "+" << string(largura - 2, '-') << "+" << endl;

    cout << left
        << "| " << setw(3) << "#"
        << "| " << setw(22) << "Produto"
        << "| " << setw(6) << "Qtd"
        << "| " << setw(13) << "Preco (s/IVA)"
        << "| " << setw(13) << "Preco (c/IVA)"
        << "| " << setw(13) << "Total Item" << "|" << endl;
    cout << margem << "+" << string(largura - 2, '-') << "+" << endl;

    int idx = 1;
    for (const auto& item : venda.getItens()) {
        double precoSemIVA = item.precoUnitario;
        double precoComIVA = precoSemIVA * 1.23;
        double totalItem = precoComIVA * item.quantidade;
        cout << margem << left
            << "| " << setw(3) << idx
            << "| " << setw(22) << item.nomeProduto
            << "| " << setw(6) << item.quantidade
            << "| " << setw(13) << fixed << setprecision(2) << precoSemIVA
            << "| " << setw(13) << precoComIVA
            << "| " << setw(13) << totalItem << "|" << endl;
        idx++;
    }
    cout << margem << "+" << string(largura - 2, '-') << "+" << endl;
    ostringstream ossTotal;
    ossTotal << "Total da venda (com IVA): " << fixed << setprecision(2) << venda.getTotalComIVA();
    cout << margem << "|" << right << setw(largura - 3) << ossTotal.str() << " |" << endl;
    cout << margem << "+" << string(largura - 2, '=') << "+" << endl;
}

bool Loja::salvarDados(const string& diretorio) {
    criarDiretorioSeNaoExistir(diretorio);

    bool sucesso = true;
    sucesso &= salvarClientes(diretorio + "/clientes.txt");
    sucesso &= salvarProdutos(diretorio + "/produtos.txt");

    if (sucesso) {
        cout << GREEN << "Dados salvos com sucesso no diretorio '" << diretorio << "'" << RESET << endl;
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
        cout << GREEN << "Dados carregados com sucesso do diretorio '" << diretorio << "'" << RESET << endl;
    }
    else {
        cout << YELLOW << "Aviso: Alguns dados nao puderam ser carregados." << RESET << endl;
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

    for (const auto& venda : vendas) {
        if (venda.getNumeroFatura().empty() || venda.getIdCliente() == 0)
            continue; // Só grava vendas reais

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
                        double precoSemIVA = stod(precoStr);
                        double precoUnit = precoSemIVA / qtd;
                        double precoCusto = precoUnit / 1.3;

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
    const int largura = 86;
    const string margem = "    "; // 4 espaços
    string titulo = "HISTORICO DE VENDAS";
    int espaco = largura - 4 - titulo.length();
    int esq = espaco / 2;
    int dir = espaco - esq;
    cout << margem << "+" << string(largura - 2, '=') << "+" << endl;
    cout << margem << "|" << string(esq, ' ') << titulo << string(dir, ' ') << "|" << endl;
    cout << margem << "+" << string(largura - 2, '=') << "+" << endl;

    bool encontrouVendas = false;

    // Definição de larguras das colunas
    const int wFat = 15, wFatVal = 8, wCli = 12, wCliVal = 4, wTotal = 16, wTotalVal = 12;
    const int wProd = 30, wQtd = 8, wTotCIVA = 15;

    for (int i = MAX_VENDAS - 1; i >= 0; i--) {
        int pos = (proximaPosicaoVenda + i) % MAX_VENDAS;
        const Venda& venda = vendas[pos];

        if (venda.getNumeroFatura().empty() || venda.getIdCliente() == 0) {
            continue;
        }

        encontrouVendas = true;

        // Linha Fatura e Cliente
        ostringstream ossFat;
        ossFat << "| " << left << setw(wFat) << "Fatura Nº:" << setw(wFatVal) << venda.getNumeroFatura()
               << "| " << left << setw(wCli) << "Cliente ID:" << setw(wCliVal) << venda.getIdCliente() << "|";
        cout << margem << ossFat.str() << endl;

        // Linha Total da Venda
        ostringstream ossTot;
        ossTot << "| " << left << setw(wTotal) << "Total da Venda:"
               << right << setw(wTotalVal) << fixed << setprecision(2) << venda.getValorTotal() << " €";
        int espacos = largura - 3 - wTotal - wTotalVal - 2; // 3 pipes, 2 para ' €'
        ossTot << string(espacos, ' ') << "|";
        cout << margem << ossTot.str() << endl;

        cout << margem << "+" << string(largura - 2, '-') << "+" << endl;

        // Cabeçalho dos itens
        ostringstream ossCab;
        ossCab << "| " << left << setw(wProd) << "Produto"
               << "| " << setw(wQtd) << "Qtd"
               << "| " << setw(wTotCIVA) << "Total c/IVA" << "|";
        cout << margem << ossCab.str() << endl;
        cout << margem << "+" << string(largura - 2, '-') << "+" << endl;

        // Itens
        for (const ItemVenda& item : venda.getItens()) {
            ostringstream ossItem;
            ossItem << "| " << left << setw(wProd) << item.nomeProduto
                    << "| " << setw(wQtd) << item.quantidade
                    << "| " << setw(wTotCIVA - 2) << fixed << setprecision(2) << item.totalComIVA << " €" << " |";
            cout << margem << ossItem.str() << endl;
        }
        cout << margem << "+" << string(largura - 2, '-') << "+" << endl;
    }

    if (!encontrouVendas) {
        cout << margem << "| Nenhuma venda registrada." << setw(largura - 27) << right << "|" << endl;
    }

    cout << margem << "+" << string(largura - 2, '=') << "+" << endl;
}

void Loja::relatorioStock() const {
    const int largura = 86;
    const string margem = "    "; // 4 espaços
    imprimirTituloCentralizado("RELATORIO DE STOCK", largura, margem);
    cout << margem << left << setw(25) << "Produto" << "| " << setw(8) << "Quantidade" << "|" << endl;
    cout << margem << "+" << string(largura - 2, '-') << "+" << endl;
    for (const auto& p : produtos) {
        cout << margem << left << setw(25) << p.getNome() << "| " << setw(8) << p.getQuantidade() << "|" << endl;
    }
    cout << margem << "+" << string(largura - 2, '=') << "+" << endl;
}


void Loja::relatorioVendasPorProduto(const string& nomeProduto) const {
    const int largura = 86;
    const string margem = "    "; // 4 espaços
    string titulo = "DETALHAMENTO DE VENDAS - PRODUTO: " + nomeProduto;
    imprimirTituloCentralizado(titulo, largura, margem);

    // Definição das larguras das colunas
    std::vector<std::string> cabecalho = {"Fatura Nº", "Qtd", "Preço Unit.", "Total c/IVA", "Custo", "Lucro"};
    std::vector<int> larguras = {10, 5, 13, 13, 10, 10};
    imprimirLinhaTabela(cabecalho, larguras);
    cout << margem << "+" << string(largura - 2, '-') << "+" << endl;

    bool encontrado = false;
    int totalQtd = 0;
    double totalCusto = 0, totalLucro = 0, totalComIVA = 0;

    for (const auto& venda : vendas) {
        for (const auto& item : venda.getItens()) {
            if (item.nomeProduto == nomeProduto) {
                encontrado = true;

                double precoUnitario = item.precoUnitario;
                double custoUnitario = item.precoCusto;
                int quantidade = item.quantidade;
                double totalComIVAItem = item.totalComIVA;
                double custoTotal = custoUnitario * quantidade;
                double lucroTotal = (precoUnitario * quantidade) - custoTotal;

                std::vector<std::string> linha = {
                    venda.getNumeroFatura(),
                    std::to_string(quantidade),
                    (ostringstream{} << fixed << setprecision(2) << precoUnitario << " EUR").str(),
                    (ostringstream{} << fixed << setprecision(2) << totalComIVAItem << " EUR").str(),
                    (ostringstream{} << fixed << setprecision(2) << custoTotal << " EUR").str(),
                    (ostringstream{} << fixed << setprecision(2) << lucroTotal << " EUR").str()
                };
                imprimirLinhaTabela(linha, larguras);

                totalQtd += quantidade;
                totalCusto += custoTotal;
                totalComIVA += totalComIVAItem;
                totalLucro += lucroTotal;
            }
        }
    }

    if (!encontrado) {
        cout << margem << "| Nenhuma venda encontrada para este produto." << setw(largura - 38) << right << "|" << endl;
    }
    else {
        cout << margem << "+" << string(largura - 2, '-') << "+" << endl;
        std::vector<std::string> linhaTotal = {
            "TOTAL",
            std::to_string(totalQtd),
            "-",
            (ostringstream{} << fixed << setprecision(2) << totalComIVA << " EUR").str(),
            (ostringstream{} << fixed << setprecision(2) << totalCusto << " EUR").str(),
            (ostringstream{} << fixed << setprecision(2) << totalLucro << " EUR").str()
        };
        imprimirLinhaTabela(linhaTotal, larguras);
    }
    cout << margem << "+" << string(largura - 2, '=') << "+" << endl;
}

void Loja::relatorioTotalVendas() const {
    const int largura = 86;
    const string margem = "    "; // 4 espaços
    imprimirTituloCentralizado("RELATORIO TOTAL DE VENDAS", largura, margem);

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
        cout << margem << "Nenhuma venda registrada.\n" << RESET;
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

    cout << margem << fixed << setprecision(2);
    cout << "-> Total vendido: €" << total << "\n";
    cout << "-> Produto mais vendido: " << maisVendido << " (" << maxQtd << " unidades)\n";
    cout << "-> Produto menos vendido: " << menosVendido << " (" << minQtd << " unidades)\n";
    cout << "-> Lucro do mais vendido: €" << lucroMaisVendido << "\n";
    cout << "-> Cliente que mais comprou (ID): " << idTopCliente << " com €" << maiorCompra << "\n";

    cout << margem << "+" << string(largura - 2, '=') << "+" << endl;
}


void Loja::relatorioGraficoVendas() const {
    const int largura = 86;
    const string margem = "    "; // 4 espaços
    imprimirTituloCentralizado("GRAFICO DE VENDAS POR PRODUTO", largura, margem);

    // Mapeia vendas por produto
    map<string, int> vendasPorProduto;
    map<int, double> totalPorCliente;
    double total = 0.0;
    for (const auto& venda : vendas) {
        total += venda.getValorTotal();
        totalPorCliente[venda.getIdCliente()] += venda.getValorTotal();
        for (const auto& item : venda.getItens()) {
            vendasPorProduto[item.nomeProduto] += item.quantidade;
        }
    }

    // Nada para exibir
    if (vendasPorProduto.empty()) {
        cout << margem << RED << "Nenhuma venda registrada.\n" << RESET;
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

    cout << margem << fixed << setprecision(2);
    cout << "-> Total vendido: €" << total << "\n";
    cout << "-> Produto mais vendido: " << maisVendido << " (" << maxQtd << " unidades)\n";
    cout << "-> Produto menos vendido: " << menosVendido << " (" << minQtd << " unidades)\n";
    cout << "-> Lucro do mais vendido: €" << lucroMaisVendido << "\n";
    cout << "-> Cliente que mais comprou (ID): " << idTopCliente << " com €" << maiorCompra << "\n";
    cout << margem << string(58, '=') << "\n";

    cout << margem << "+" << string(largura - 2, '=') << "+" << endl;
}


void Loja::relatorioVendasDetalhadoPorProduto() const {
    const int largura = 86;
    const string margem = "    "; // 4 espaços
    imprimirTituloCentralizado("RELATORIO DETALHADO POR PRODUTO", largura, margem);

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
        cout << margem << "\nNenhuma venda registrada.\n";
        return;
    }

    cout << margem << "\n" << string(largura, '=') << "\n";
    cout << margem << "| " << setw((largura - 4) / 2) << left << "RELATORIO DETALHADO POR PRODUTO"
        << setw((largura - 4) / 2) << right << " |\n";
    cout << margem << string(largura, '=') << "\n";

    cout << margem << left
        << "| " << setw(25) << "Produto"
        << "| " << setw(6) << "Qtd"
        << "| " << setw(14) << "Preço"
        << "| " << setw(14) << "Receita"
        << "| " << setw(14) << "Custo"
        << "| " << setw(14) << "Lucro" << "|" << endl;
    cout << margem << "+" << string(largura - 2, '-') << "+" << endl;

    double totalLucro = 0.0;

    for (const auto& par : quantidadePorProduto) {
        const string& nome = par.first;
        int qtd = par.second;
        double receita = receitaPorProduto[nome];
        double custo = custoPorProduto[nome];
        double preco = qtd > 0 ? receita / qtd : 0.0;
        double lucro = receita - custo;
        totalLucro += lucro;

        cout << margem << left
            << "| " << setw(25) << nome
            << "| " << setw(6) << qtd
            << "| " << setw(11) << fixed << setprecision(2) << preco << " € "
            << "| " << setw(11) << receita << " € "
            << "| " << setw(11) << custo << " € "
            << "| " << setw(11) << lucro << " € "
            << "|" << endl;
    }

    cout << margem << "+" << string(largura - 2, '=') << "+" << endl;
    cout << margem << "| " << setw(70) << left << "Lucro total estimado:"
        << "€ " << fixed << setprecision(2) << totalLucro << " |" << endl;
    cout << margem << "+" << string(largura - 2, '=') << "+" << endl;
}
