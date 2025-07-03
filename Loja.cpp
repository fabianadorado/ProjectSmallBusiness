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
    proximoIdProduto++;
}

void Loja::criarProduto() 
{
    string nome;
    int quantidade;
    double preco;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true) {
        cout << std::string(MARGEM) << "Nome do produto: ";
        getline(cin, nome);
        nome = toUpper(nome);
        if (!(nome.empty() || nome.find_first_not_of(' ') == string::npos)) {
            break;
        }
        cout << RED << "Nome do produto nao pode ser vazio!" << RESET << endl;
    }

    // Verifica se o produto já existe
    Produto* existente = encontrarProdutoPorNome(nome, produtos);
    if (existente != nullptr) {
        cout << YELLOW << "Produto ja existe no estoque com " << existente->getQuantidade() << " unidades." << RESET << endl;
        cout << "Deseja adicionar mais quantidade? (S/N): ";
        char opcao;
        cin >> opcao;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        opcao = toupper(opcao);
        if (opcao == 'S') {
            int qnt;
            cout << std::string(MARGEM) << "Quantidade a adicionar: ";
            bool primeiraTentativa = true;
            while (true) {
                if (!primeiraTentativa) {
                    cout << RED << "Entrada invalida. Digite um numero inteiro maior que 0." << RESET << endl;
                }
                cout << "Quantidade: ";
                string input;
                getline(cin, input);
                istringstream iss(input);
                if ((iss >> qnt) && qnt > 0) break;
                primeiraTentativa = false;
            }
            existente->adicionarStock(qnt);
            cout << GREEN << "Quantidade adicionada com sucesso!" << RESET << endl;
        } else {
            cout << RED << "Operacao cancelada." << RESET << endl;
        }
        return;
    }

    // Se o produto não existir, continua o cadastro normal
    bool primeiraTentativa = true;
    while (true) {
        if (!primeiraTentativa) {
            cout << RED << "Entrada invalida. Digite um numero inteiro maior que 0." << RESET << endl;
        }
        cout << std::string(MARGEM) << "Quantidade: ";
        string input;
        getline(cin, input);
        istringstream iss(input);
        if ((iss >> quantidade) && quantidade > 0) break;
        primeiraTentativa = false;
    }
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
            cout << GREEN << "Stock adicionado com sucesso!\n" << RESET;
            return;
        }
    }
    cout << RED << "Produto nao encontrado.\n" << RESET;
}

void Loja::eliminarProduto(int idProduto)
{
    for (size_t i = 0; i < produtos.size(); i++)
    {
        if (produtos[i].getId() == idProduto)
        {
            if (!confirmarAcao("Tem certeza que deseja remover este produto?")) {
                cout << RED << "Operacao cancelada.\n" << RESET;
                return;
            }
            produtos.erase(produtos.begin() + i);
            // Reorganizar IDs
            for (size_t j = 0; j < produtos.size(); ++j) {
                produtos[j].setId(j + 1);
            }
            proximoIdProduto = produtos.size() + 1;
            cout << GREEN << "Produto removido.\n" << RESET;
            return;
        }
    }
    cout << RED << "Produto nao encontrado.\n" << RESET;
}

void Loja::eliminarProduto() 
{
    listarProdutos();
    int id = lernumero(std::string(MARGEM) + "Digite o ID do produto que deseja eliminar: ");
    eliminarProduto(id);
}


void Loja::listarProdutos() const
{
    // Definição das larguras das colunas
    const int wId = 4, wNome = 28, wQtd = 8, wCusto = 13, wVenda = 13;
    const int nPipes = 6; // 5 separadores + 1 de início
    const int larguraTabela = wId + wNome + wQtd + wCusto + wVenda + nPipes;

    // Título
    cout << std::string(MARGEM) << "+" << string(larguraTabela - 2, '=') << "+" << endl;
    cout << std::string(MARGEM) << "|" << centro("LISTA DE PRODUTOS", larguraTabela - 2) << "|" << endl;
    cout << std::string(MARGEM) << "+" << string(larguraTabela - 2, '=') << "+" << endl;

    // Cabeçalho
    cout << std::string(MARGEM)
        << "|" << centro("ID", wId) << "|"
        << centro("NOME", wNome) << "|"
        << centro("QTD", wQtd) << "|"
        << centro("PRECO CUSTO", wCusto) << "|"
        << centro("PRECO VENDA", wVenda) << "|" << endl;
    cout << std::string(MARGEM) << string(wId, '-') << "+" << string(wNome, '-') << "+" << string(wQtd, '-') << "+" << string(wCusto, '-') << "+" << string(wVenda, '-') << endl;

    if (produtos.empty()) {
        cout << std::string(MARGEM) << "|" << centro("Nenhum produto cadastrado.", larguraTabela - 2) << "|" << endl;
    } else {
        for (const auto& p : produtos) {
            string idStr = to_string(p.getId());
            string nomeStr = toUpper(p.getNome());
            string qtdStr = to_string(p.getQuantidade());
            ostringstream ossCusto, ossVenda;
            ossCusto << fixed << setprecision(2) << p.getPrecoCusto() << " EUR";
            ossVenda << fixed << setprecision(2) << p.getPrecoVenda() << " EUR";
            cout << std::string(MARGEM)
                << "|" << centro(idStr, wId)
                << "|" << centro(nomeStr, wNome)
                << "|" << centro(qtdStr, wQtd)
                << "|" << centro(ossCusto.str(), wCusto)
                << "|" << centro(ossVenda.str(), wVenda)
                << "|" << endl;
        }
    }
    cout << std::string(MARGEM) << string(wId, '=') << "+" << string(wNome, '=') << "+" << string(wQtd, '=') << "+" << string(wCusto, '=') << "+" << string(wVenda, '=') << "+" << endl;
}


void Loja::criarCliente(const string& nome, const string& telefone, const string& morada, const string& dataNascimento) 
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

    Cliente novoCliente(nomeMaiusculo, telefoneFormatado, moradaMaiuscula, dataNascimento);
    novoCliente.setIdCliente(novoId);
    clientes.push_back(novoCliente);

    // Mantém a lista de clientes ordenada por ID
    sort(clientes.begin(), clientes.end(),
        [](const Cliente& a, const Cliente& b) {
            return a.getIdCliente() < b.getIdCliente();
        });

    cout << GREEN << "Cliente criado com ID: " << novoId << "\n" << RESET;
}

void Loja::eliminarCliente(int idCliente) 
{
    for (size_t i = 0; i < clientes.size(); i++) 
    {
        if (clientes[i].getIdCliente() == idCliente) 
        {
            if (!confirmarAcao("Tem certeza que deseja remover este cliente?")) {
                cout << RED << "Operacao cancelada.\n" << RESET;
                return;
            }
            idsClientesDisponiveis.push_back(idCliente);
            sort(idsClientesDisponiveis.begin(), idsClientesDisponiveis.end());
            clientes.erase(clientes.begin() + i);
            cout << GREEN << "Cliente removido. ID " << idCliente << RESET;
            return;
        }
    }
    cout << RED << "Cliente nao encontrado.\n" << RESET;
}

void Loja::alterarNomeCliente(int idCliente, const string& novoNome) 
{
    string nomeMaiusculo = toUpper(novoNome);
    for (size_t i = 0; i < clientes.size(); i++) 
    {
        if (clientes[i].getIdCliente() == idCliente) 
        {
            clientes[i].setNome(nomeMaiusculo);
            cout << GREEN << "Nome alterado.\n" << RESET;
            return;
        }
    }
    cout << RED << "Cliente nao encontrado.\n" << RESET;
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
        string idStr, nome, telefone, morada, dataNasc, totalStr;

        if (getline(iss, idStr, ';') &&
            getline(iss >> ws, nome, ';') &&
            getline(iss >> ws, telefone, ';') &&
            getline(iss >> ws, morada, ';') &&
            getline(iss >> ws, dataNasc, ';') &&
            getline(iss >> ws, totalStr)) {

            try {
                int id = stoi(idStr);
                double totalComprado = stof(totalStr);

                Cliente cliente(nome, telefone, morada, dataNasc, totalComprado);
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
    vector<Cliente> clientesOrdenados = clientes;
    sort(clientesOrdenados.begin(), clientesOrdenados.end(),
        [](const Cliente& a, const Cliente& b) {
            return a.getIdCliente() < b.getIdCliente();
        });

    // Larguras das colunas
    const int wId = 4, wNome = 28, wTel = 11, wMorada = 30;
    const int larguraTabela = wId + wNome + wTel + wMorada + 5; // 5 pipes
    const int larguraTotal = larguraTabela;

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
    cout << "+" << string(larguraTabela - 2, '=') << "+" << endl;
    cout << "|" << centro("LISTA DE CLIENTES", larguraTabela - 2) << "|" << endl;
    cout << "+" << string(larguraTabela - 2, '=') << "+" << endl;

    // Cabeçalho centralizado
    cout <<
        "|" << centro("ID", wId) << "|"
        << centro("Nome", wNome) << "|"
        << centro("Telefone", wTel) << "|"
        << centro("Morada", wMorada) << "|" << endl;

    cout <<
         string(wId, '-') << "+" << string(wNome, '-') << "+" << string(wTel, '-') << "+" << string(wMorada, '-') << endl;

    if (clientesOrdenados.empty()) {
        cout << "|" << centro("Nenhum cliente cadastrado.", larguraTabela - 2) << "|\n";
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
            cout <<
                "|" << centro(idStr, wId) << "|"
                << centro(nomeStr, wNome) << "|"
                << centro(telStr, wTel) << "|"
                << centro(moradaStr, wMorada) << "|" << endl;
        }
    }

    cout << string(wId, '=') << "+" << string(wNome, '=') << "+" << string(wTel, '=') << "+" << string(wMorada, '=') << "+" << endl;
}

void Loja::efetuarVenda(int idCliente)
{
    Cliente* clienteEncontrado = nullptr;
    while (true) {
        clienteEncontrado = nullptr;
        for (auto& c : clientes)
        {
            if (c.getIdCliente() == idCliente)
            {
                clienteEncontrado = &c;
                break;
            }
        }
        if (!clienteEncontrado) {
            if (!confirmarAcao("Cliente nao encontrado. Deseja tentar outro ID?")) {
                return;
            }
            idCliente = lernumero(std::string(MARGEM) + "ID do cliente: ");
            continue;
        }
        break;
    }

    Venda novaVenda(idCliente);
    novaVenda.setNomeCliente(clienteEncontrado->getNome());
    novaVenda.setDataNascimentoCliente(clienteEncontrado->getDataNascimento());
    int pontosTotais = static_cast<int>(clienteEncontrado->getTotalComprado() + novaVenda.getValorTotal());
    novaVenda.setMonstersPoints(pontosTotais);
    listarProdutos();
    char mais = 'n';
    bool adicionouProduto = false;
    bool primeiraVez = true;
    do {
        if (!primeiraVez) {
            system("cls");
            listarProdutos();
        }
        primeiraVez = false;
        Produto* produtoSelecionado = nullptr;
        int idProduto;
        bool erroInput = false;
        bool desistiu = false;
        // Loop até encontrar um produto válido e com estoque
        while (true) {
            if (erroInput) {
                cout << RED << "Entrada invalida. Digite um numero inteiro maior ou igual a 0." << RESET << endl;
            }
            cout << std::string(MARGEM) << "ID do Produto: ";
            string input;
            getline(cin, input);
            istringstream iss(input);
            if ((iss >> idProduto) && idProduto >= 0) {
                if (idProduto == 0) {
                    desistiu = true;
                    break;
                }
                // Procura o produto
                produtoSelecionado = nullptr;
                for (auto& p : produtos) {
                    if (p.getId() == idProduto) {
                        produtoSelecionado = &p;
                        break;
                    }
                }
                if (!produtoSelecionado) {
                    cout << RED << "Produto inexistente." << RESET << endl;
                    continue;
                }
                if (produtoSelecionado->getQuantidade() == 0) {
                    cout << RED << "Produto sem estoque. Escolha outro produto ou digite 0 para desistir." << RESET << endl;
                    continue;
                }
                break; // Produto válido encontrado
            }
            erroInput = true;
        }
        if (desistiu) break;
        int quantidade;
        bool primeiraTentativaQuantidade = true;
        while (true) {
            if (!primeiraTentativaQuantidade) {
                cout << RED << "Entrada invalida. Digite um numero inteiro maior que 0." << RESET << endl;
            }
            cout << std::string(MARGEM) << "Quantidade: ";
            string input;
            getline(cin, input);
            istringstream iss(input);
            if ((iss >> quantidade) && quantidade > 0) break;
            primeiraTentativaQuantidade = false;
        }
        novaVenda.adicionarItem(
            produtoSelecionado->getNome(),
            quantidade,
            produtoSelecionado->getPrecoVenda(),
            produtoSelecionado->getPrecoCusto()
        );
        produtoSelecionado->removerStock(quantidade);
        adicionouProduto = true;
        while (true) {
            cout << std::string(MARGEM) << "Adicionar mais produtos? (s/n): ";
            cin >> mais;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (mais == 's' || mais == 'S' || mais == 'n' || mais == 'N') {
                break;
            } else {
                cout << RED << "Entrada invalida. Digite 's' para sim ou 'n' para nao." << RESET << endl;
            }
        }
    } while (mais == 's' || mais == 'S');

    if (!adicionouProduto) {
        cout << RED << "Nenhum produto foi adicionado a venda. Operação cancelada." << RESET << endl;
        return;
    }

    // Limpa a tela e mostra o resumo da venda antes do menu de opções
    system("cls");
    mostrarResumoVenda(novaVenda);
    // Agora exibe o menu de opções logo abaixo do resumo
    while (true) {
        // Não limpa a tela novamente, mantém o resumo visível
        cout << endl;
        cout << std::string(MARGEM) << "O que deseja fazer?\n";
        cout << std::string(MARGEM) << "1 - Prosseguir para pagamento\n";
        cout << std::string(MARGEM) << "2 - Remover um item\n";
        cout << std::string(MARGEM) << "3 - Cancelar toda a venda\n";
        cout << std::string(MARGEM) << "Opcao: ";
        int op;
        cin >> op;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (op == 1) {
            break;
        } else if (op == 2) {
            if (novaVenda.getItens().empty()) {
                cout << RED << "Nao ha itens para remover!" << RESET << endl;
                continue;
            }
            int linha;
            cout << std::string(MARGEM) << "Digite o numero do item a remover: ";
            cin >> linha;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            // Antes de remover, repor estoque do item
            const auto& itensVenda = novaVenda.getItens();
            auto it = std::find_if(itensVenda.begin(), itensVenda.end(), [linha](const ItemVenda& item) {
                return item.numeroLinha == linha;
            });
            if (it != itensVenda.end()) {
                reporEstoqueItem(*it);
            }
            novaVenda.removerItemPorLinha(linha);
            if (novaVenda.getItens().empty()) {
                cout << RED << "Todos os itens foram removidos. Venda cancelada automaticamente." << RESET << endl;
                return;
            }
            // Atualiza tela e resumo após remoção de item
            system("cls");
            mostrarResumoVenda(novaVenda);
        } else if (op == 3) {
            // Antes de cancelar, repor estoque de todos os itens
            reporEstoqueVenda(novaVenda);
            novaVenda.cancelarVenda();
            return;
        } else {
            cout << RED << "Opção invalida!" << RESET << endl;
        }
    }

    // Sorteio do talao gratis (10% de chance) e mensagem de parabéns só agora
    srand(static_cast<unsigned>(time(0)));
    bool ganhouTalaoGratis = (rand() % 10 == 0);
    double valorFinal = ganhouTalaoGratis ? 0.0 : novaVenda.getValorTotal();
    if (ganhouTalaoGratis) {
        cout << GREEN << "PARABENS! Voce ganhou o talao gratis!\n" << RESET;
        Sleep(5000); // Pausa 5 segundos para o usuário ver a mensagem e os valores
    }

    // Mostrar valor a pagar antes de pedir valor entregue
    cout << fixed << setprecision(2);
    cout << std::string(MARGEM) << "\nValor a pagar: " << (ganhouTalaoGratis ? 0.0 : novaVenda.getValorTotal()) << " EUR\n";

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
                cout << std::string(MARGEM) << "Valor entregue pelo cliente: ";
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

    system("cls");
    novaVenda.imprimirTalao();
    cout << std::string(MARGEM) << "Pressione Enter para voltar..." << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    system("cls");

    if (!ganhouTalaoGratis)
        clienteEncontrado->adicionarCompra(valorFinal);

    vendas[proximaPosicaoVenda] = novaVenda;
    proximaPosicaoVenda = (proximaPosicaoVenda + 1) % MAX_VENDAS;
    salvarVendas("dados_loja/vendas.txt");
}

void Loja::mostrarResumoVenda(const Venda& venda) const {
    // Buscar nome do cliente se não estiver disponível na venda
    string nomeCliente = venda.getNomeCliente();
    if (nomeCliente.empty() || nomeCliente == "Desconhecido") {
        for (const auto& c : clientes) {
            if (c.getIdCliente() == venda.getIdCliente()) {
                nomeCliente = c.getNome();
                break;
            }
        }
    }
    // Exibir nome do cliente no resumo
    // Definição das larguras das colunas
    const int wId = 3, wProd = 22, wQtd = 6, wPSemIVA = 13, wPComIVA = 13, wTotal = 13;
    const int nPipes = 7; // 6 separadores + 1 de início
    const int larguraTabela = wId + wProd + wQtd + wPSemIVA + wPComIVA + wTotal + nPipes;
    string titulo = "RESUMO DA VENDA";
    cout << string(4, ' ') << "+" << string(larguraTabela - 2, '=') << "+" << endl;
    cout << string(4, ' ') << "|" << centro(titulo, larguraTabela - 2) << "|" << endl;
    cout << string(4, ' ') << "+" << string(larguraTabela - 2, '=') << "+" << endl;

    ostringstream ossId;
    ossId << "ID DO CLIENTE: " << venda.getIdCliente();
    cout << string(4, ' ') << "|" << centro(ossId.str(), larguraTabela - 2) << "|" << endl;
    cout << string(4, ' ') << string(larguraTabela - 2, '-') << endl;

    // Cabeçalho
    cout << string(4, ' ')
        << "|" << centro("#", wId)
        << "|" << centro("PRODUTO", wProd)
        << "|" << centro("QTD", wQtd)
        << "|" << centro("PRECO S/IVA", wPSemIVA)
        << "|" << centro("PRECO C/IVA", wPComIVA)
        << "|" << centro("TOTAL ITEM", wTotal)
        << "|" << endl;
    cout << string(4, ' ') << string(wId, '-') << "+" << string(wProd, '-') << "+" << string(wQtd, '-') << "+" << string(wPSemIVA, '-') << "+" << string(wPComIVA, '-') << "+" << string(wTotal, '-') << endl;

    int idx = 1;
    for (const auto& item : venda.getItens()) {
        string prod = toUpper(item.nomeProduto);
        if ((int)prod.length() > wProd) prod = prod.substr(0, wProd);
        cout << string(4, ' ')
            << "|" << centro(to_string(idx), wId)
            << "|" << centro(prod, wProd)
            << "|" << centro(to_string(item.quantidade), wQtd)
            << "|" << centro((ostringstream{} << fixed << setprecision(2) << item.precoSemIVA).str(), wPSemIVA)
            << "|" << centro((ostringstream{} << fixed << setprecision(2) << item.precoUnitario * 1.23).str(), wPComIVA)
            << "|" << centro((ostringstream{} << fixed << setprecision(2) << item.precoUnitario * 1.23 * item.quantidade).str(), wTotal)
            << "|" << endl;
        idx++;
    }
    cout << string(4, ' ') << "+" << string(wId, '=') << "+" << string(wProd, '=') << "+" << string(wQtd, '=') << "+" << string(wPSemIVA, '=') << "+" << string(wPComIVA, '=') << "+" << string(wTotal, '=') << "+" << endl;
    ostringstream ossTotal;
    ossTotal << "TOTAL DA VENDA (COM IVA): " << fixed << setprecision(2) << venda.getTotalComIVA();
    cout << string(4, ' ') << "|" << centro(ossTotal.str(), larguraTabela - 2) << "|" << endl;
    cout << string(4, ' ') << "+" << string(larguraTabela - 2, '=') << "+" << endl;
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
            << cliente.getDataNascimento() << ";"
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

                // Padronizar nome para maiúsculas
                nome = toUpper(nome);

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
    // Definição de larguras das colunas
    vector<string> cabecalho = {"#", "FATURA Nº", "CLIENTE ID", "TOTAL VENDA", "PRODUTO", "QTD", "TOTAL C/IVA"};
    vector<int> larguras = {4, 12, 10, 14, 18, 6, 14};
    int larguraTabela = 1; // pipe inicial
    for (int l : larguras) larguraTabela += l + 1; // +1 para cada pipe

    string titulo = "HISTORICO DE VENDAS";
    cout << string(4, ' ') << "+" << string(larguraTabela - 2, '=') << "+" << endl;
    cout << string(4, ' ') << "|" << centro(titulo, larguraTabela - 2) << "|" << endl;
    cout << string(4, ' ') << "+" << string(larguraTabela - 2, '=') << "+" << endl;

    bool encontrouVendas = false;
    // Cabeçalho
    cout << string(4, ' ') << "|";
    for (size_t i = 0; i < cabecalho.size(); ++i) cout << centro(cabecalho[i], larguras[i]) << "|";
    cout << endl;
    cout << string(4, ' ') << string(larguraTabela - 2, '-') << endl;

    int numFatura = 1;
    for (int i = MAX_VENDAS - 1; i >= 0; i--) {
        int pos = (proximaPosicaoVenda + i) % MAX_VENDAS;
        const Venda& venda = vendas[pos];
        if (venda.getNumeroFatura().empty() || venda.getIdCliente() == 0) continue;
        encontrouVendas = true;
        for (const auto& item : venda.getItens()) {
            vector<string> linha = {
                to_string(numFatura),
                venda.getNumeroFatura(),
                to_string(venda.getIdCliente()),
                (ostringstream{} << fixed << setprecision(2) << venda.getValorTotal() << " EUR").str(),
                toUpper(item.nomeProduto),
                to_string(item.quantidade),
                (ostringstream{} << fixed << setprecision(2) << item.totalComIVA << " EUR").str()
            };
            cout << string(4, ' ') << "|";
            for (size_t j = 0; j < linha.size(); ++j) cout << centro(linha[j], larguras[j]) << "|";
            cout << endl;
        }
        numFatura++;
        cout << string(4, ' ') << string(larguraTabela - 2, '-') << endl;
    }
    if (!encontrouVendas) {
        cout << string(4, ' ') << "|" << centro("Nenhuma venda registrada.", larguraTabela - 2) << "|" << endl;
    }
    cout << string(4, ' ') << "+" << string(larguraTabela - 2, '=') << "+" << endl;
}

void Loja::relatorioStock() const {
    // Definição das larguras das colunas
    const int wProd = 28, wQtd = 12;
    const int larguraTabela = wProd + wQtd + 3; // 2 pipes + 1 separador
    imprimirTituloCentralizado("RELATORIO DE STOCK", larguraTabela, string(4, ' '));
    // Cabeçalho
    cout << string(4, ' ') << "+" << string(wProd, '=') << "+" << string(wQtd, '=') << "+" << endl;
    cout << string(4, ' ') << "|" << centro("PRODUTO", wProd) << "|" << centro("QUANTIDADE", wQtd) << "|" << endl;
    cout << string(4, ' ') << string(wProd, '-') << "+" << string(wQtd, '-') << "+" << endl;
    for (const auto& p : produtos) {
        cout << string(4, ' ') << "|" << centro(toUpper(p.getNome()), wProd) << "|" << centro(to_string(p.getQuantidade()), wQtd) << "|" << endl;
    }
    cout << string(4, ' ') << "+" << string(wProd, '=') << "+" << string(wQtd, '=') << "+" << endl;
}


void Loja::relatorioVendasPorProduto(const string& nomeProduto) const {
    const string titulo = "DETALHAMENTO DE VENDAS - PRODUTO: " + toUpper(nomeProduto);
    imprimirTituloCentralizado(titulo, 86, string(4, ' '));
    // Cabeçalho
    cout << string(4, ' ') << "+" << string(10, '=') << "+" << string(5, '=') << "+" << string(13, '=') << "+" << string(13, '=') << "+" << string(10, '=') << "+" << string(10, '=') << "+" << endl;
    cout << string(4, ' ') << "|" << centro("FATURA N", 10) << "|" << centro("QTD", 5) << "|" << centro("PRECO UNIT.", 13) << "|" << centro("TOTAL C/IVA", 13) << "|" << centro("CUSTO", 10) << "|" << centro("LUCRO", 10) << "|" << endl;
    cout << string(4, ' ') << string(10, '-') << "+" << string(5, '-') << "+" << string(13, '-') << "+" << string(13, '-') << "+" << string(10, '-') << "+" << string(10, '-') << "+" << endl;
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
                cout << string(4, ' ') << "|" << centro(venda.getNumeroFatura(), 10)
                    << "|" << centro(to_string(quantidade), 5)
                    << "|" << centro((ostringstream{} << fixed << setprecision(2) << precoUnitario << " EUR").str(), 13)
                    << "|" << centro((ostringstream{} << fixed << setprecision(2) << totalComIVAItem << " EUR").str(), 13)
                    << "|" << centro((ostringstream{} << fixed << setprecision(2) << custoTotal << " EUR").str(), 10)
                    << "|" << centro((ostringstream{} << fixed << setprecision(2) << lucroTotal << " EUR").str(), 10)
                    << "|" << endl;
                totalQtd += quantidade;
                totalCusto += custoTotal;
                totalComIVA += totalComIVAItem;
                totalLucro += lucroTotal;
            }
        }
    }
    if (!encontrado) {
        cout << string(4, ' ') << "|" << centro("Nenhuma venda encontrada para este produto.", 82) << "|" << endl;
    } else {
        cout << string(4, ' ') << "+" << string(10, '-') << "+" << string(5, '-') << "+" << string(13, '-') << "+" << string(13, '-') << "+" << string(10, '-') << "+" << string(10, '-') << "+" << endl;
        cout << string(4, ' ') << "|" << left << setw(10) << "TOTAL"
            << "|" << centro(to_string(totalQtd), 5)
            << "|" << centro("-", 13)
            << "|" << centro((ostringstream{} << fixed << setprecision(2) << totalComIVA << " EUR").str(), 13)
            << "|" << centro((ostringstream{} << fixed << setprecision(2) << totalCusto << " EUR").str(), 10)
            << "|" << centro((ostringstream{} << fixed << setprecision(2) << totalLucro << " EUR").str(), 10)
            << "|" << endl;
    }
    cout << string(4, ' ') << "+" << string(10, '=') << "+" << string(5, '=') << "+" << string(13, '=') << "+" << string(13, '=') << "+" << string(10, '=') << "+" << string(10, '=') << "+" << endl;
}

void Loja::relatorioTotalVendas() const {
    const int largura = 86;
    imprimirTituloCentralizado("RELATORIO TOTAL DE VENDAS", largura, string(4, ' '));

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
        cout << string(4, ' ') << "Nenhuma venda registrada.\n" << RESET;
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

    // Centralizar o total vendido e destacar o valor em vermelho, com 'euros' ao final
    string totalStr = "Total vendido:";
    ostringstream ossTotal;
    ossTotal << RED << fixed << setprecision(2) << total << RESET << " euros";
    string totalLinha = totalStr + string(10, ' ') + ossTotal.str();
    int larguraTotal = largura - 2;
    int espacos = (larguraTotal - (int)totalLinha.length()) / 2;
    cout << string(4, ' ') << string(espacos, ' ') << totalLinha << endl;

    // Restante alinhado à esquerda
    cout << left << setw(30) << string(4, ' ') + "Produto mais vendido:" << maisVendido << " (" << maxQtd << " unidades)\n";
    cout << left << setw(30) << string(4, ' ') + "Produto menos vendido:" << menosVendido << " (" << minQtd << " unidades)\n";
    cout << left << setw(30) << string(4, ' ') + "Lucro do mais vendido:" << "€" << lucroMaisVendido << "\n";
    cout << left << setw(30) << string(4, ' ') + "Cliente que mais comprou (ID):" << idTopCliente << " com €" << maiorCompra << "\n";

    cout << string(4, ' ') << "+" << string(largura - 2, '=') << "+" << endl;
}


void Loja::relatorioGraficoVendas() const {
    const int largura = 86;
    imprimirTituloCentralizado("GRAFICO DE VENDAS POR PRODUTO", largura, string(4, ' '));

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
        cout << string(4, ' ') << RED << "Nenhuma venda registrada.\n" << RESET;
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

    cout << string(4, ' ') << fixed << setprecision(2);
    cout << "-> Total vendido: €" << total << "\n";
    cout << "-> Produto mais vendido: " << maisVendido << " (" << maxQtd << " unidades)\n";
    cout << "-> Produto menos vendido: " << menosVendido << " (" << minQtd << " unidades)\n";
    cout << "-> Lucro do mais vendido: €" << lucroMaisVendido << "\n";
    cout << "-> Cliente que mais comprou (ID): " << idTopCliente << " com €" << maiorCompra << "\n";
    cout << string(4, ' ') << string(58, '=') << endl;

    cout << string(4, ' ') << "+" << string(largura - 2, '=') << "+" << endl;
}


void Loja::relatorioVendasDetalhadoPorProduto() const {
    // Definição das larguras das colunas
    const int wProd = 25, wQtd = 6, wPreco = 12, wReceita = 14, wCusto = 14, wLucro = 14;
    // Largura total da tabela (incluindo pipes)
    const int larguraTabela = wProd + wQtd + wPreco + wReceita + wCusto + wLucro + 7;

    // Título centralizado e alinhado com a tabela
    cout << string(4, ' ')
         << "+" << string(wProd, '=') << "+" << string(wQtd, '=') << "+" << string(wPreco, '=') << "+" << string(wReceita, '=') << "+" << string(wCusto, '=') << "+" << string(wLucro, '=') << "+" << endl;
    cout << string(4, ' ')
         << "|" << centro("RELATORIO DETALHADO POR PRODUTO", larguraTabela - 2) << "|" << endl;
    cout << string(4, ' ')
         << "+" << string(wProd, '=') << "+" << string(wQtd, '=') << "+" << string(wPreco, '=') << "+" << string(wReceita, '=') << "+" << string(wCusto, '=') << "+" << string(wLucro, '=') << "+" << endl;

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
        cout << string(4, ' ') << "\nNenhuma venda registrada.\n";
        return;
    }

    // Cabeçalho alinhado igual ao corpo usando centro
    cout << string(4, ' ')
        << "|" << centro("Produto", wProd)
        << "|" << centro("Qtd", wQtd)
        << "|" << centro("Preco", wPreco)
        << "|" << centro("Receita", wReceita)
        << "|" << centro("Custo", wCusto)
        << "|" << centro("Lucro", wLucro)
        << "|" << endl;
    cout << string(4, ' ') << string(wProd, '-') << "+" << string(wQtd, '-') << "+" << string(wPreco, '-') << "+" << string(wReceita, '-') << "+" << string(wCusto, '-') << "+" << string(wLucro, '-') << "+" << endl;

    double totalLucro = 0.0;

    for (const auto& par : quantidadePorProduto) {
        const string& nome = par.first;
        int qtd = par.second;
        double receita = receitaPorProduto[nome];
        double custo = custoPorProduto[nome];
        double preco = qtd > 0 ? receita / qtd : 0.0;
        double lucro = receita - custo;
        totalLucro += lucro;

        ostringstream ossPreco, ossReceita, ossCusto, ossLucro;
        ossPreco << fixed << setprecision(2) << preco << " euro";
        ossReceita << fixed << setprecision(2) << receita << " euro";
        ossCusto << fixed << setprecision(2) << custo << " euro";
        ossLucro << fixed << setprecision(2) << lucro << " euro";

        cout << string(4, ' ')
            << "|" << centro(nome, wProd)
            << "|" << centro(to_string(qtd), wQtd)
            << "|" << centro(ossPreco.str(), wPreco)
            << "|" << centro(ossReceita.str(), wReceita)
            << "|" << centro(ossCusto.str(), wCusto)
            << "|" << centro(ossLucro.str(), wLucro)
            << "|" << endl;
    }

    // Total alinhado como linha da tabela
    ostringstream ossTotalLucro;
    ossTotalLucro << fixed << setprecision(2) << totalLucro << " euro";
    cout << string(4, ' ') << "+" << string(wProd, '=') << "+" << string(wQtd, '=') << "+" << string(wPreco, '=') << "+" << string(wReceita, '=') << "+" << string(wCusto, '=') << "+" << string(wLucro, '=') << "+" << endl;
    cout << string(4, ' ')
        << "|" << left << setw(wProd + wQtd + wPreco + wReceita + wCusto + 5) << " Lucro total estimado:"
        << centro(ossTotalLucro.str(), wLucro) << "|" << endl;
    cout << string(4, ' ') << "+" << string(wProd, '=') << "+" << string(wQtd, '=') << "+" << string(wPreco, '=') << "+" << string(wReceita, '=') << "+" << string(wCusto, '=') << "+" << string(wLucro, '=') << "+" << endl;
}

// Adicionar função auxiliar em Loja para repor estoque de um item removido da venda
void Loja::reporEstoqueItem(const ItemVenda& item) {
    for (auto& p : produtos) {
        if (toUpper(p.getNome()) == toUpper(item.nomeProduto)) {
            p.adicionarStock(item.quantidade);
            break;
        }
    }
}

// Adicionar função auxiliar para repor estoque de todos os itens de uma venda
void Loja::reporEstoqueVenda(const Venda& venda) {
    for (const auto& item : venda.getItens()) {
        reporEstoqueItem(item);
    }
}
