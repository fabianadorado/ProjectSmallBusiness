#define NOMINMAX
#include "Loja.h"
#include "Auxiliar.h"
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
    string nomeMaiusculo = toUpper(nome);
    Produto novoProduto(proximoIdProduto, nomeMaiusculo, quantidade, precoCusto);
    produtos.push_back(novoProduto);
    cout << "Produto criado com sucesso.\n";
    proximoIdProduto++;
}

void Loja::criarProduto() 
{
    string nome;
    int quantidade;
    double preco;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true) {
        cout << "Nome do produto: ";
        getline(cin, nome);
        nome = toUpper(nome);
        if (!(nome.empty() || nome.find_first_not_of(' ') == string::npos)) {
            break;
        }
        cout << RED << "Nome do produto nao pode ser vazio!" << RESET << endl;
    }

    // Verifica se o produto já existe
    Produto* existente = encontrarProdutoPorNome(nome);
    if (existente != nullptr) {
        cout << YELLOW << "Produto ja existe no estoque com " << existente->getQuantidade() << " unidades." << RESET << endl;
        cout << "Deseja adicionar mais quantidade? (S/N): ";
        char opcao;
        cin >> opcao;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        opcao = toupper(opcao);
        if (opcao == 'S') {
            int qnt;
            cout << "Quantidade a adicionar: ";
            while (!(cin >> qnt) || qnt <= 0) {
                cout << endl << RED << "Atencao, apenas pode inserir numeros e tem de ser maior que 0.\n" << RESET;
                cout << "Quantidade: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }
            existente->adicionarStock(qnt);
            cout << GREEN << "Quantidade adicionada com sucesso!" << RESET << endl;
        } else {
            cout << RED << "Operacao cancelada." << RESET << endl;
        }
        return;
    }

    // Se o produto não existir, continua o cadastro normal
    quantidade = lernumero("Quantidade: ");
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
    vector<string> cabecalho = {"ID", "NOME", "QTD", "PRECO CUSTO", "PRECO VENDA"};
    vector<int> larguras = {4, 28, 8, 13, 13};
    const int larguraTabela = 4 + 28 + 8 + 13 + 13 + 5; // 5 pipes

    // Título
    cout << margem << "+" << string(larguraTabela - 2, '=') << "+" << endl;
    cout << margem << "|" << centro("LISTA DE PRODUTOS", larguraTabela - 2) << "|" << endl;
    cout << margem << "+" << string(larguraTabela - 2, '=') << "+" << endl;

    // Cabeçalho
    cout << margem
        << "|" << centro("ID", 4) << "|"
        << centro("NOME", 28) << "|"
        << centro("QTD", 8) << "|"
        << centro("PRECO CUSTO", 13) << "|"
        << centro("PRECO VENDA", 13) << "|" << endl;
    cout << margem
        << "+" << string(4, '-') << "+"
        << string(28, '-') << "+"
        << string(8, '-') << "+"
        << string(13, '-') << "+"
        << string(13, '-') << "+" << endl;

    if (produtos.empty()) {
        cout << margem << "|" << centro("Nenhum produto cadastrado.", larguraTabela - 2) << "|\n";
    } else {
        for (const auto& p : produtos) {
            string idStr = to_string(p.getId());
            string nomeStr = toUpper(p.getNome());
            string qtdStr = to_string(p.getQuantidade());
            ostringstream ossCusto, ossVenda;
            ossCusto << fixed << setprecision(2) << p.getPrecoCusto() << " EUR";
            ossVenda << fixed << setprecision(2) << p.getPrecoVenda() << " EUR";
            cout << margem
                << "|" << centro(idStr, 4) << "|"
                << centro(nomeStr, 28) << "|"
                << centro(qtdStr, 8) << "|"
                << centro(ossCusto.str(), 13) << "|"
                << centro(ossVenda.str(), 13) << "|" << endl;
        }
    }
    cout << margem << "+" << string(4, '=') << "+" << string(28, '=') << "+" << string(8, '=') << "+" << string(13, '=') << "+" << string(13, '=') << "+" << endl;
}


void Loja::criarCliente(const string& nome, const string& telefone, const string& morada) 
{
    int novoId;
    string nomeMaiusculo = toUpper(nome);
    string moradaMaiuscula = toUpper(morada);
    string telefoneFormatado = formatarTelefone(telefone);

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

    Cliente novoCliente(nomeMaiusculo, telefoneFormatado, moradaMaiuscula);
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
    string nomeMaiusculo = toUpper(novoNome);
    for (size_t i = 0; i < clientes.size(); i++) 
    {
        if (clientes[i].getIdCliente() == idCliente) 
        {
            clientes[i].setNome(nomeMaiusculo);
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
            getline(iss >> ws, nome, ';') &&
            getline(iss >> ws, telefone, ';') &&
            getline(iss >> ws, morada, ';') &&
            getline(iss >> ws, totalStr)) {

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
    const string margem = "    ";
    vector<Cliente> clientesOrdenados = clientes;
    sort(clientesOrdenados.begin(), clientesOrdenados.end(),
        [](const Cliente& a, const Cliente& b) {
            return a.getIdCliente() < b.getIdCliente();
        });

    // Larguras das colunas
    const int wId = 4, wNome = 28, wTel = 11, wMorada = 30;
    const int larguraTabela = wId + wNome + wTel + wMorada + 5; // 5 pipes
    const int larguraTotal = margem.length() + larguraTabela;

    // Função para centralizar texto, truncando se necessário
    auto centro = [](const string& texto, int largura) {
        string t = texto;
        if ((int)t.length() > largura) t = t.substr(0, largura);
        int espacos = largura - (int)t.length();
        int esq = espacos / 2;
        int dir = espacos - esq;
        return string(esq, ' ') + t + string(dir, ' ');
    };

    // Título centralizado perfeitamente com a tabela
    cout << margem << "+" << string(larguraTabela - 2, '=') << "+" << endl;
    cout << margem << "|" << centro("LISTA DE CLIENTES", larguraTabela - 2) << "|" << endl;
    cout << margem << "+" << string(larguraTabela - 2, '=') << "+" << endl;

    // Cabeçalho centralizado
    cout << margem
        << "|" << centro("ID", wId) << "|"
        << centro("Nome", wNome) << "|"
        << centro("Telefone", wTel) << "|"
        << centro("Morada", wMorada) << "|" << endl;

    cout << margem
         << "+" << string(wId, '-') << "+"
         << string(wNome, '-') << "+"
         << string(wTel, '-') << "+"
         << string(wMorada, '-') << "+" << endl;

    if (clientesOrdenados.empty()) {
        cout << margem << "|" << centro("Nenhum cliente cadastrado.", larguraTabela - 2) << "|\n";
    } else {
        for (const auto& c : clientesOrdenados) {
            string idStr = to_string(c.getIdCliente());
            string nomeStr = toUpper(c.getNome());
            nomeStr.erase(0, nomeStr.find_first_not_of(' '));
            nomeStr.erase(nomeStr.find_last_not_of(' ') + 1);
            if (!nomeStr.empty() && nomeStr[0] == ' ') nomeStr = nomeStr.substr(1);
            string telStr = c.getTelefone();
            string moradaStr = toUpper(c.getMorada());
            moradaStr.erase(0, moradaStr.find_first_not_of(' '));
            moradaStr.erase(moradaStr.find_last_not_of(' ') + 1);
            cout << margem
                << "|" << centro(idStr, wId) << "|"
                << centro(nomeStr, wNome) << "|"
                << centro(telStr, wTel) << "|"
                << centro(moradaStr, wMorada) << "|" << endl;
        }
    }

    cout << margem << "+" << string(wId, '=') << "+" << string(wNome, '=') << "+" << string(wTel, '=') << "+" << string(wMorada, '=') << "+" << endl;
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
        Produto* produtoSelecionado = nullptr;
        for (auto& p : produtos)
        {
            if (p.getId() == idProduto)
            {
                produtoSelecionado = &p;
                break;
            }
        }
        if (!produtoSelecionado) {
            cout << "Produto inexistente.\n";
            continue;
        }
        if (produtoSelecionado->getQuantidade() == 0) {
            cout << RED << "Produto sem estoque. Escolha outro produto." << RESET << endl;
            continue;
        }
        int quantidade;
        while (true) {
            quantidade = lernumero("Quantidade: ");
            if (produtoSelecionado->getQuantidade() < quantidade) {
                cout << RED << "Stock insuficiente. Disponivel: " << produtoSelecionado->getQuantidade() << " unidades." << RESET << endl;
            } else {
                break;
            }
        }
        novaVenda.adicionarItem(
            produtoSelecionado->getNome(),
            quantidade,
            produtoSelecionado->getPrecoVenda(),
            produtoSelecionado->getPrecoCusto()
        );
        produtoSelecionado->removerStock(quantidade);
        while (true) {
            cout << "Adicionar mais produtos? (s/n): ";
            cin >> mais;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (mais == 's' || mais == 'S' || mais == 'n' || mais == 'N') {
                break;
            } else {
                cout << RED << "Entrada invalida. Digite 's' para sim ou 'n' para nao." << RESET << endl;
            }
        }
    } while (mais == 's' || mais == 'S');

    //Sorteio do talao gratis (10% de chance)
    srand(static_cast<unsigned>(time(0)));
    bool ganhouTalaoGratis = (rand() % 10 == 0); 

    double valorFinal = ganhouTalaoGratis ? 0.0 : novaVenda.getValorTotal();

    if (ganhouTalaoGratis) {
        cout << GREEN << "PARABENS! Voce ganhou o talao gratis!\n" << RESET;
    }

    // Mostrar resumo da venda antes do pagamento
    mostrarResumoVenda(novaVenda);

    // Mostrar valor a pagar antes de pedir valor entregue
    cout << fixed << setprecision(2);
    cout << "\nValor a pagar: " << valorFinal << " EUR\n";

    double valorEntregue = 0.0;
    if (ganhouTalaoGratis) {
        valorEntregue = 0.0;
    } else {
        bool valorValido = false;
        while (!valorValido) {
            bool primeiraTentativa = true;
            while (true) {
                if (!primeiraTentativa) {
                    cout << endl << RED << "Entrada inválida. Digite um número válido (>= 0)." << RESET << endl;
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
                cout << endl << RED << "Erro: " << e.what() << RESET << endl;
                // Repete o loop para pedir o valor novamente
            }
        }
    }

    cout << endl << "Pressione Enter para ver o talão..." << endl;
    limparBuffer();
    cin.get();
    system("cls"); // Limpa a tela antes de mostrar o talão
    novaVenda.imprimirTalao();
    cout << "\nPressione Enter para voltar...";
    cin.get();
    system("cls");

    if (!ganhouTalaoGratis)
        clienteEncontrado->adicionarCompra(valorFinal);

    vendas[proximaPosicaoVenda] = novaVenda;
    proximaPosicaoVenda = (proximaPosicaoVenda + 1) % MAX_VENDAS;
    salvarVendas("dados_loja/vendas.txt");
}

void Loja::mostrarResumoVenda(const Venda& venda) const {
    const int wId = 3, wProd = 22, wQtd = 6, wPSemIVA = 13, wPComIVA = 13, wTotal = 13;
    const int larguraTabela = 4 + wId + wProd + wQtd + wPSemIVA + wPComIVA + wTotal; // 6 pipes + colunas
    const string margem = "    ";
    string titulo = "RESUMO DA VENDA";
    cout << margem << "+" << string(larguraTabela - 2, '=') << "+" << endl;
    cout << margem << "|" << centro(titulo, larguraTabela - 2) << "|" << endl;
    cout << margem << "+" << string(larguraTabela - 2, '=') << "+" << endl;

    ostringstream ossId;
    ossId << "ID DO CLIENTE: " << venda.getIdCliente();
    cout << margem << "|" << centro(ossId.str(), larguraTabela - 2) << "|" << endl;
    cout << margem << "+" << string(larguraTabela - 2, '-') << "+" << endl;

    // Cabeçalho centralizado
    cout << margem
        << "|" << centro("#", wId) << "|"
        << centro("PRODUTO", wProd) << "|"
        << centro("QTD", wQtd) << "|"
        << centro("PRECO S/IVA", wPSemIVA) << "|"
        << centro("PRECO C/IVA", wPComIVA) << "|"
        << centro("TOTAL ITEM", wTotal) << "|" << endl;
    cout << margem
        << "+" << string(wId, '-') << "+"
        << string(wProd, '-') << "+"
        << string(wQtd, '-') << "+"
        << string(wPSemIVA, '-') << "+"
        << string(wPComIVA, '-') << "+"
        << string(wTotal, '-') << "+" << endl;

    int idx = 1;
    for (const auto& item : venda.getItens()) {
        string prod = toUpper(item.nomeProduto);
        if ((int)prod.length() > wProd) prod = prod.substr(0, wProd);
        cout << margem
            << "|" << centro(to_string(idx), wId) << "|"
            << centro(prod, wProd) << "|"
            << centro(to_string(item.quantidade), wQtd) << "|"
            << centro((ostringstream{} << fixed << setprecision(2) << item.precoUnitario).str(), wPSemIVA) << "|"
            << centro((ostringstream{} << fixed << setprecision(2) << item.precoUnitario * 1.23).str(), wPComIVA) << "|"
            << centro((ostringstream{} << fixed << setprecision(2) << item.precoUnitario * 1.23 * item.quantidade).str(), wTotal) << "|" << endl;
        idx++;
    }
    cout << margem
        << "+" << string(wId, '=') << "+"
        << string(wProd, '=') << "+"
        << string(wQtd, '=') << "+"
        << string(wPSemIVA, '=') << "+"
        << string(wPComIVA, '=') << "+"
        << string(wTotal, '=') << "+" << endl;
    ostringstream ossTotal;
    ossTotal << "TOTAL DA VENDA (COM IVA): " << fixed << setprecision(2) << venda.getTotalComIVA();
    cout << margem << "|" << centro(ossTotal.str(), larguraTabela - 2) << "|" << endl;
    cout << margem << "+" << string(larguraTabela - 2, '=') << "+" << endl;
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
    const string margem = "    ";
    string titulo = "HISTORICO DE VENDAS";
    cout << margem << "+" << string(largura - 2, '=') << "+" << endl;
    cout << margem << "|" << centro(titulo, largura - 2) << "|" << endl;
    cout << margem << "+" << string(largura - 2, '=') << "+" << endl;

    bool encontrouVendas = false;
    // Definição de larguras das colunas
    vector<string> cabecalho = {"FATURA Nº", "CLIENTE ID", "TOTAL VENDA", "PRODUTO", "QTD", "TOTAL C/IVA"};
    vector<int> larguras = {12, 10, 14, 18, 6, 14};
    int larguraTabela = 0;
    for (int l : larguras) larguraTabela += l + 1; // +1 para cada pipe
    larguraTabela += 1; // pipe final

    // Cabeçalho
    cout << margem << "|";
    for (size_t i = 0; i < cabecalho.size(); ++i) cout << centro(cabecalho[i], larguras[i]) << "|";
    cout << endl;
    cout << margem << "+";
    for (int l : larguras) cout << string(l, '-') << "+";
    cout << endl;

    for (int i = MAX_VENDAS - 1; i >= 0; i--) {
        int pos = (proximaPosicaoVenda + i) % MAX_VENDAS;
        const Venda& venda = vendas[pos];
        if (venda.getNumeroFatura().empty() || venda.getIdCliente() == 0) continue;
        encontrouVendas = true;
        for (const auto& item : venda.getItens()) {
            vector<string> linha = {
                venda.getNumeroFatura(),
                to_string(venda.getIdCliente()),
                (ostringstream{} << fixed << setprecision(2) << venda.getValorTotal() << " EUR").str(),
                toUpper(item.nomeProduto),
                to_string(item.quantidade),
                (ostringstream{} << fixed << setprecision(2) << item.totalComIVA << " EUR").str()
            };
            cout << margem << "|";
            for (size_t j = 0; j < linha.size(); ++j) cout << centro(linha[j], larguras[j]) << "|";
            cout << endl;
        }
        cout << margem << "+";
        for (int l : larguras) cout << string(l, '-') << "+";
        cout << endl;
    }
    if (!encontrouVendas) {
        cout << margem << "|" << centro("Nenhuma venda registrada.", larguraTabela - 2) << "|" << endl;
    }
    cout << margem << "+";
    for (int l : larguras) cout << string(l, '=') << "+";
    cout << endl;
}

void Loja::relatorioStock() const {
    const int largura = 86;
    const string margem = "    ";
    imprimirTituloCentralizado("RELATORIO DE STOCK", largura, margem);
    vector<string> cabecalho = {"PRODUTO", "QUANTIDADE"};
    vector<int> larguras = {25, 12};
    cout << margem << "|";
    for (size_t i = 0; i < cabecalho.size(); ++i) cout << centro(cabecalho[i], larguras[i]) << "|";
    cout << endl;
    cout << margem << "+" << string(larguras[0], '-') << "+" << string(larguras[1], '-') << "+" << endl;
    for (const auto& p : produtos) {
        cout << margem << "|" << centro(toUpper(p.getNome()), larguras[0]) << "|" << centro(to_string(p.getQuantidade()), larguras[1]) << "|" << endl;
    }
    cout << margem << "+" << string(larguras[0], '=') << "+" << string(larguras[1], '=') << "+" << endl;
}


void Loja::relatorioVendasPorProduto(const string& nomeProduto) const {
    const int largura = 86;
    const string margem = "    ";
    string titulo = "DETALHAMENTO DE VENDAS - PRODUTO: " + toUpper(nomeProduto);
    imprimirTituloCentralizado(titulo, largura, margem);
    vector<string> cabecalho = {"FATURA Nº", "QTD", "PRECO UNIT.", "TOTAL C/IVA", "CUSTO", "LUCRO"};
    vector<int> larguras = {10, 5, 13, 13, 10, 10};
    cout << margem << "|";
    for (size_t i = 0; i < cabecalho.size(); ++i) cout << centro(cabecalho[i], larguras[i]) << "|";
    cout << endl;
    cout << margem << "+";
    for (int l : larguras) cout << string(l, '-') << "+";
    cout << endl;
    bool encontrado = false;
    int totalQtd = 0;
    double totalCusto = 0, totalLucro = 0, totalComIVA = 0;
    for (const auto& venda : vendas) {
        for (const auto& item : venda.getItens()) {
            if (toUpper(item.nomeProduto) == toUpper(nomeProduto)) {
                encontrado = true;
                double precoUnitario = item.precoUnitario;
                double custoUnitario = item.precoCusto;
                int quantidade = item.quantidade;
                double totalComIVAItem = item.totalComIVA;
                double custoTotal = custoUnitario * quantidade;
                double lucroTotal = (precoUnitario * quantidade) - custoTotal;
                vector<string> linha = {
                    venda.getNumeroFatura(),
                    to_string(quantidade),
                    (ostringstream{} << fixed << setprecision(2) << precoUnitario << " EUR").str(),
                    (ostringstream{} << fixed << setprecision(2) << totalComIVAItem << " EUR").str(),
                    (ostringstream{} << fixed << setprecision(2) << custoTotal << " EUR").str(),
                    (ostringstream{} << fixed << setprecision(2) << lucroTotal << " EUR").str()
                };
                cout << margem << "|";
                for (size_t j = 0; j < linha.size(); ++j) cout << centro(linha[j], larguras[j]) << "|";
                cout << endl;
                totalQtd += quantidade;
                totalCusto += custoTotal;
                totalComIVA += totalComIVAItem;
                totalLucro += lucroTotal;
            }
        }
    }
    if (!encontrado) {
        cout << margem << "|" << centro("Nenhuma venda encontrada para este produto.", 61) << "|" << endl;
    } else {
        cout << margem << "+";
        for (int l : larguras) cout << string(l, '-') << "+";
        cout << endl;
        vector<string> linhaTotal = {
            "TOTAL",
            to_string(totalQtd),
            "-",
            (ostringstream{} << fixed << setprecision(2) << totalComIVA << " EUR").str(),
            (ostringstream{} << fixed << setprecision(2) << totalCusto << " EUR").str(),
            (ostringstream{} << fixed << setprecision(2) << totalLucro << " EUR").str()
        };
        cout << margem << "|";
        for (size_t j = 0; j < linhaTotal.size(); ++j) cout << centro(linhaTotal[j], larguras[j]) << "|";
        cout << endl;
    }
    cout << margem << "+";
    for (int l : larguras) cout << string(l, '=') << "+";
    cout << endl;
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

Produto* Loja::encontrarProdutoPorNome(const string& nome) {
    string nomeMaiusculo = toUpper(nome);
    for (auto& produto : produtos) {
        if (toUpper(produto.getNome()) == nomeMaiusculo) {
            return &produto;
        }
    }
    return nullptr;
}
