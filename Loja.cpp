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

// Função criarProduto() sem parâmetros removida - usar apenas a versão com parâmetros


void Loja::adicionarStock(int idProduto, int quantidade) 
{
    for (size_t i = 0; i < produtos.size(); i++) 
    {
        if (produtos[i].getId() == idProduto) 
        {
            produtos[i].adicionarStock(quantidade);
            cout << BG_GRAY << GREEN << std::string(MARGEM) << "Stock atualizado com sucesso!" << RESET << endl;
            return;
        }
    }
    cout << BG_GRAY << RED << "Produto nao encontrado.\n" << RESET;
}

bool Loja::eliminarProduto(int idProduto)
{
    for (size_t i = 0; i < produtos.size(); i++) 
    {
        if (produtos[i].getId() == idProduto) 
        {
            produtos.erase(produtos.begin() + i);
            // Reorganizar IDs
            for (size_t j = 0; j < produtos.size(); ++j) {
                produtos[j].setId(j + 1);
            }
            proximoIdProduto = produtos.size() + 1;
            return true;
        }
    }
    cout << BG_GRAY << RED << "Produto nao encontrado.\n" << RESET;
    return false;
}

void Loja::listarProdutos() const {
    
    // Definição das larguras das colunas
    const int wId = 4, wNome = 28, wQtd = 8, wCusto = 13, wVenda = 13;
    const int nPipes = 6; // 5 separadores + 1 de início
    const int larguraTabela = wId + wNome + wQtd + wCusto + wVenda + nPipes;

    // Título
    cout << BG_GRAY << FG_BLUE;
    cout << std::string(MARGEM) << "+" << string(larguraTabela - 2, '=') << "+" << endl;
    cout << std::string(MARGEM) << "|" << centro("LISTA DE PRODUTOS", larguraTabela - 2) << "|" << endl;
    cout << std::string(MARGEM) << "+" << string(larguraTabela - 2, '=') << "+" << endl;
    cout << RESET;

    // Cabeçalho
    cout << BG_GRAY << FG_BLUE;
    cout << std::string(MARGEM)
        << "|" << centro("ID", wId) << "|"
        << centro("NOME", wNome) << "|"
        << centro("QTD", wQtd) << "|"
        << centro("PRECO CUSTO", wCusto) << "|"
        << centro("PRECO VENDA", wVenda) << "|" << endl;
    // Linha simples de separação alinhada
    cout << std::string(MARGEM)
        << string(wId, '-') << "+" << string(wNome, '-') << "+" << string(wQtd, '-') << "+" << string(wCusto, '-') << "+" << string(wVenda, '-') << "+" << endl;
    cout << RESET;

    if (produtos.empty()) {
        cout << BG_GRAY << FG_BLUE;
        cout << std::string(MARGEM) << "|" << centro("Nenhum produto cadastrado.", larguraTabela - 2) << "|" << endl;
        cout << RESET;
    } else {
        for (const auto& p : produtos) {
            string idStr = to_string(p.getId());
            string nomeStr = toUpper(p.getNome());
            string qtdStr = to_string(p.getQuantidade());
            ostringstream ossCusto, ossVenda;
            ossCusto << fixed << setprecision(2) << p.getPrecoCusto() << " EUR";
            ossVenda << fixed << setprecision(2) << p.getPrecoVenda() << " EUR";
            cout << BG_GRAY << FG_BLUE;
            cout << std::string(MARGEM)
                << "|" << centro(idStr, wId)
                << "|" << centro(nomeStr, wNome)
                << "|" << centro(qtdStr, wQtd)
                << "|" << centro(ossCusto.str(), wCusto)
                << "|" << centro(ossVenda.str(), wVenda)
                << "|" << endl;
            cout << RESET;
        }
    }
    cout << BG_GRAY << FG_BLUE;
    cout << std::string(MARGEM) << string(wId, '=') << "+" << string(wNome, '=') << "+" << string(wQtd, '=') << "+" << string(wCusto, '=') << "+" << string(wVenda, '=') << "+" << endl;
    cout << BG_GRAY << std::string(MARGEM) << string(larguraTabela, ' ') << RESET << endl;
    cout << RESET;
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

    cout << BG_GRAY << FG_BLUE;
    cout << std::string(MARGEM) << GREEN << "Cliente criado com ID: " << novoId << "\n" << RESET;
}

void Loja::eliminarCliente(int idCliente) 
{
    for (size_t i = 0; i < clientes.size(); i++) 
    {
        if (clientes[i].getIdCliente() == idCliente) 
        {
            if (!confirmarAcao("Tem certeza que deseja remover este cliente?")) {
                cout << BG_GRAY << std::string(MARGEM) << RED << "Operacao cancelada." << RESET << endl;
                return;
            }
            idsClientesDisponiveis.push_back(idCliente);
            sort(idsClientesDisponiveis.begin(), idsClientesDisponiveis.end());
            clientes.erase(clientes.begin() + i);
            cout << BG_GRAY << std::string(MARGEM) << GREEN << "Cliente removido. ID " << idCliente << RESET << endl;
            return;
        }
    }
    cout << BG_GRAY << std::string(MARGEM) << RED << "Cliente nao encontrado." << RESET << endl;
}

void Loja::alterarNomeCliente(int idCliente, const string& novoNome) 
{
    string nomeMaiusculo = toUpper(novoNome);
    for (size_t i = 0; i < clientes.size(); i++) 
    {
        if (clientes[i].getIdCliente() == idCliente) 
        {
            clientes[i].setNome(nomeMaiusculo);
            cout << BG_GRAY << std::string(MARGEM) << GREEN << "Nome alterado com sucesso!" << RESET << endl;
            return;
        }
    }
    cout << BG_GRAY << std::string(MARGEM)<< RED << "Cliente nao encontrado.\n" << RESET;
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


    // Título centralizado perfeitamente com a tabela
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "+" << string(larguraTabela - 2, '=') << "+" << RESET << endl;
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "|" << centro("LISTA DE CLIENTES", larguraTabela - 2) << "|" << RESET << endl;
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "+" << string(larguraTabela - 2, '=') << "+" << RESET << endl;

    // Cabeçalho centralizado
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM)
        << "|" << centro("ID", wId) << "|"
        << centro("Nome", wNome) << "|"
        << centro("Telefone", wTel) << "|"
        << centro("Morada", wMorada) << "|" << RESET << endl;
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM)
         << string(wId, '-') << "+" << string(wNome, '-') << "+" << string(wTel, '-') << "+" << string(wMorada, '-') << "+" << RESET << endl;

    if (clientesOrdenados.empty()) {
        cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "|" << centro("Nenhum cliente cadastrado.", larguraTabela - 2) << "|" << RESET << endl;
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
            cout << BG_GRAY << FG_BLUE << std::string(MARGEM)
                << "|" << centro(idStr, wId) << "|"
                << centro(nomeStr, wNome) << "|"
                << centro(telStr, wTel) << "|"
                << centro(moradaStr, wMorada) << "|" << RESET << endl;
        }
    }
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM)
         << string(wId, '=') << "+" << string(wNome, '=') << "+" << string(wTel, '=') << "+" << string(wMorada, '=') << "+" << RESET << endl;
}

void Loja::efetuarVenda() {
    preencherTela(BG_GRAY, FG_BLUE, 40, 120); // Garante fundo cinza em toda a tela
    int idCliente;
    Cliente* clienteEncontrado = nullptr;
    bool tentarNovamenteCliente = true;

    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "REALIZAR VENDA" << RESET << endl << endl;
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa buffer só uma vez antes do loop (REMOVIDO)

    while (tentarNovamenteCliente) {
        idCliente = lerIDPositivo(std::string(BG_GRAY) + std::string(FG_BLUE) + std::string(MARGEM) + "ID do cliente: ");
        clienteEncontrado = nullptr;
        for (auto& c : clientes) {
            if (c.getIdCliente() == idCliente) {
                clienteEncontrado = &c;
                break;
            }
        }
        if (!clienteEncontrado) {
            cout << BG_GRAY << RED << std::string(MARGEM) << "ID invalido. Digite apenas numeros de um cliente existente." << RESET << endl;
            string resposta;
            while (true) {
                cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Deseja tentar novamente? (s/n): ";
                getline(cin, resposta);
                if (!resposta.empty() && (resposta[0] == 's' || resposta[0] == 'S')) {
                    std::cout << std::endl; // Espaço visual antes do novo prompt (opcional)
                    tentarNovamenteCliente = true;
                    break;
                } else if (!resposta.empty() && (resposta[0] == 'n' || resposta[0] == 'N')) {
                    tentarNovamenteCliente = false;
                    break;
                } else {
                    cout << std::string(MARGEM) << RED << "Entrada invalida. Digite 's' para sim ou 'n' para nao." << RESET << endl;
                }
            }
        } else {
            break;
        }
    }
    if (!clienteEncontrado) {
        cout << std::string(MARGEM) << RED << "Operacao de venda cancelada." << RESET << endl;
        return;
    }

    // Exibe produtos logo abaixo do ID do cliente
    listarProdutos();

    Venda novaVenda(idCliente);
    novaVenda.setNomeCliente(clienteEncontrado->getNome());
    novaVenda.setDataNascimentoCliente(clienteEncontrado->getDataNascimento());
    int pontosTotais = static_cast<int>(clienteEncontrado->getTotalComprado() + novaVenda.getValorTotal());
    novaVenda.setMonstersPoints(pontosTotais);

    std::string mais;
    bool adicionouProduto = false;
    bool primeiraVez = true;
    Produto* produtoSelecionado = nullptr;
    do {
        if (!primeiraVez) {
            system("cls");
            listarProdutos();
        }
        primeiraVez = false;
        int idProduto;
        bool tentarNovamente = true;
        while (tentarNovamente) {
            idProduto = lerIDPositivo(std::string(BG_GRAY) + std::string(FG_BLUE) + std::string(MARGEM) + "ID do Produto: ");
            if (idProduto < 1) {
                // ID inválido
                cout << std::string(MARGEM) << RED << "ID invalido." << RESET << endl;
            } else {
                // Procura o produto
                produtoSelecionado = nullptr;
                for (auto& p : produtos) {
                    if (p.getId() == idProduto) {
                        produtoSelecionado = &p;
                        break;
                    }
                }
                if (!produtoSelecionado) {
                    cout << BG_GRAY << RED << std::string(MARGEM) << "Produto inexistente." << RESET << endl;
                } else if (produtoSelecionado->getQuantidade() == 0) {
                    cout << BG_GRAY << std::string(MARGEM) << RED << "Produto sem estoque. Escolha outro produto." << RESET << endl;
                } else {
                    // Produto válido encontrado
                    break;
                }
            }
            // Pergunta se deseja tentar novamente
            string resposta;
            while (true) {
                cout << BG_GRAY << FG_BLUE;
                cout << std::string(MARGEM) << "Deseja tentar novamente? (s/n): ";
                getline(cin, resposta);
                if (!resposta.empty() && (resposta[0] == 's' || resposta[0] == 'S')) {
                    std::cout << std::endl; // Espaço visual antes do novo prompt (opcional)
                    tentarNovamente = true;
                    break;
                } else if (!resposta.empty() && (resposta[0] == 'n' || resposta[0] == 'N')) {
                    tentarNovamente = false;
                    break;
                } else {
                    cout << std::string(MARGEM) << RED << "Entrada invalida. Digite 's' para sim ou 'n' para nao." << RESET << endl;
                }
            }
        }
        if (!produtoSelecionado || produtoSelecionado->getQuantidade() == 0) {
            // Usuário optou por não tentar novamente ou produto sem estoque
            break;
        }
        // Só pede quantidade e adiciona produto se produtoSelecionado for válido e tem estoque
        int quantidade;
        bool primeiraTentativaQuantidade = true;
        while (true) {
            if (!primeiraTentativaQuantidade) {
                cout << BG_GRAY << RED << std::string(MARGEM) << "Entrada invalida. Digite um numero inteiro maior que 0." << RESET << endl;
            }
            cout << BG_GRAY << FG_BLUE;
            cout << std::string(MARGEM) << "Quantidade: ";
            string quantidadeStr;
            getline(cin, quantidadeStr);
            istringstream issQtd(quantidadeStr);
            if ((issQtd >> quantidade) && quantidade > 0) break;
            primeiraTentativaQuantidade = false;
        }
        cout << RESET;
        novaVenda.adicionarItem(
            produtoSelecionado->getNome(),
            quantidade,
            produtoSelecionado->getPrecoVenda(),
            produtoSelecionado->getPrecoCusto()
        );
        produtoSelecionado->removerStock(quantidade);
        adicionouProduto = true;
        while (true) {
            cout << BG_GRAY << FG_BLUE;
            cout << std::string(MARGEM) << "Adicionar mais produtos? (s/n): ";
            getline(cin, mais);
            if (!mais.empty() && (mais[0] == 's' || mais[0] == 'S' || mais[0] == 'n' || mais[0] == 'N')) {
                break;
            } else {
                cout << std::string(MARGEM) << RED << "Entrada invalida. Digite 's' para sim ou 'n' para nao." << RESET << endl;
            }
        }
    } while (!mais.empty() && (mais[0] == 's' || mais[0] == 'S') && produtoSelecionado);

    if (!adicionouProduto) {
        cout << std::string(MARGEM) << RED << "Nenhum produto foi adicionado a venda. Operação cancelada." << RESET << endl;
        return;
    }

    // Limpa a tela e mostra o resumo da venda antes do menu de opções
    mostrarResumoVenda(novaVenda);
    // Agora exibe o menu de opções logo abaixo do resumo
    while (true) {
        cout << endl;
        cout << BG_GRAY << FG_BLUE;
        cout << std::string(MARGEM) << "O que deseja fazer?\n";
        cout << std::string(MARGEM) << "1 - Prosseguir para pagamento\n";
        cout << std::string(MARGEM) << "2 - Remover um item\n";
        cout << std::string(MARGEM) << "3 - Cancelar toda a venda\n";
        int op = -1;
        do {
            system("cls");
            preencherTela(BG_GRAY, FG_BLUE);
            mostrarResumoVenda(novaVenda);
            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "O que deseja fazer?" << RESET << endl;
            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "1 - Prosseguir para pagamento" << RESET << endl;
            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "2 - Remover um item" << RESET << endl;
            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "3 - Cancelar toda a venda" << RESET << endl;
            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Opcao: " << BG_GRAY << FG_BLUE;
            string input;
            getline(cin, input);
            cout << RESET;
            istringstream iss(input);
            if ((iss >> op) && (op >= 1 && op <= 3)) {
                char extra;
                if (!(iss >> extra)) break;
            }
            // Se inválido, repete o loop e redesenha o menu
        } while (true);
        if (op == 1) {
            break;
        } else if (op == 2) {
            if (novaVenda.getItens().empty()) {
                cout << std::string(MARGEM) << RED << "Nao ha itens para remover!" << RESET << endl;
                continue;
            }
            int linha;
            while (true) {
                linha = lerIDPositivo(std::string(MARGEM) + "Digite o numero do item a remover: ", true);
                // Verifica se o item existe
                const auto& itensVenda = novaVenda.getItens();
                auto it = std::find_if(itensVenda.begin(), itensVenda.end(), [linha](const ItemVenda& item) {
                    return item.numeroLinha == linha;
                });
                if (it != itensVenda.end()) {
                    reporEstoqueItem(*it);
                    novaVenda.removerItemPorLinha(linha);
                    if (novaVenda.getItens().empty()) {
                        cout << std::string(MARGEM) << RED << "Todos os itens foram removidos. Venda cancelada automaticamente." << RESET << endl;
                        return;
                    }
                    // Atualiza tela e resumo após remoção de item
                    mostrarResumoVenda(novaVenda);
                    break;
                }
                // Se não encontrou, apenas repete o prompt sem mostrar erro
            }
        } else if (op == 3) {
            // Confirmação antes de cancelar
            string resposta;
            while (true) {
                cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Tem certeza que deseja cancelar toda a venda? (s/n): " << BG_GRAY << FG_BLUE;
                getline(cin, resposta);
                cout << RESET;
                if (!resposta.empty() && (resposta[0] == 's' || resposta[0] == 'S')) {
                    reporEstoqueVenda(novaVenda);
                    novaVenda.cancelarVenda();
                    return;
                } else if (!resposta.empty() && (resposta[0] == 'n' || resposta[0] == 'N')) {
                    system("cls");
                    mostrarResumoVenda(novaVenda);
                    break; // Volta ao menu de opções
                } else {
                    cout << std::string(MARGEM) << RED << "Entrada invalida. Digite 's' para sim ou 'n' para nao." << RESET << endl;
                }
            }
        } else {
            cout << std::string(MARGEM) << RED << "Opção invalida!" << RESET << endl;
        }
    }

    // Sorteio do talao gratis (10% de chance) e mensagem de parabéns só agora
    srand(static_cast<unsigned>(time(0)));
    bool ganhouTalaoGratis = (rand() % 10 == 0);
    double valorFinal = ganhouTalaoGratis ? 0.0 : novaVenda.getValorTotal();
    if (ganhouTalaoGratis) {
        cout << std::string(MARGEM) << GREEN << "PARABENS! Voce ganhou o talao gratis!\n" << RESET;
    }

    // Mostrar valor a pagar antes de pedir valor entregue
    cout << "\n" << BG_GRAY << FG_BLUE;
    cout << std::string(MARGEM) << "Valor a pagar: ";
    cout << fixed << setprecision(2) << (ganhouTalaoGratis ? 0.0 : novaVenda.getValorTotal()) << " EUR\n";
    cout << RESET;

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
                cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Valor entregue pelo cliente: " << BG_GRAY << flush;
                string valorStr;
                getline(cin, valorStr);
                std::replace(valorStr.begin(), valorStr.end(), ',', '.');
                istringstream issValor(valorStr);
                if (issValor >> valorEntregue && valorEntregue >= 0.0) {
                    char extra;
                    if (!(issValor >> extra)) break;
                }
                primeiraTentativa = false;
            }
            try {
    novaVenda.finalizarVenda(valorEntregue);
                valorValido = true;
            } catch (const std::invalid_argument& e) {
                cout << endl << RED << std::string(MARGEM )<< "Erro: " << e.what() << RESET << endl;
                // Repete o loop para pedir o valor novamente
            }
        }
    }

    system("cls");
    // Defina a margem padrão (exemplo: 4 espaços)
    cout << RESET;
    novaVenda.imprimirTalao(MARGEM);
    cout << RESET;

    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Pressione Enter para continuar..." << RESET;
    cin.get();
}

void Loja::mostrarResumoVenda(const Venda& venda) const {
    preencherTela(BG_GRAY, FG_BLUE);
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
        cout << BG_GRAY << FG_BLUE << std::string(MARGEM)
            << "|" << centro(to_string(idx), wId)
            << "|" << centro(prod, wProd)
            << "|" << centro(to_string(item.quantidade), wQtd)
            << "|" << centro((ostringstream{} << fixed << setprecision(2) << item.precoSemIVA).str(), wPSemIVA)
            << "|" << centro((ostringstream{} << fixed << setprecision(2) << item.precoUnitario * 1.23).str(), wPComIVA)
            << "|" << centro((ostringstream{} << fixed << setprecision(2) << item.precoUnitario * 1.23 * item.quantidade).str(), wTotal)
            << "|" << RESET << endl;
        idx++;
    }
    // Linha dupla de igual com fundo cinza
    cout << BG_GRAY << FG_BLUE << string(4, ' ') << "+" << string(wId, '=') << "+" << string(wProd, '=') << "+" << string(wQtd, '=') << "+" << string(wPSemIVA, '=') << "+" << string(wPComIVA, '=') << "+" << string(wTotal, '=') << "+" << RESET << endl;
    ostringstream ossTotal;
    ossTotal << "TOTAL DA VENDA (COM IVA): " << fixed << setprecision(2) << venda.getTotalComIVA();
    cout << BG_GRAY << FG_BLUE << string(4, ' ') << "+" << string(larguraTabela - 2, '=') << "+" << RESET << endl;
    cout << BG_GRAY << FG_BLUE << string(4, ' ') << "|" << centro(ossTotal.str(), larguraTabela - 2) << "|" << RESET << endl;
    cout << BG_GRAY << FG_BLUE << string(4, ' ') << "+" << string(larguraTabela - 2, '=') << "+" << RESET << endl;
    cout << RESET;
}

bool Loja::salvarDados(const string& diretorio) {
    criarDiretorioSeNaoExistir(diretorio);

    bool sucesso = true;
    sucesso &= salvarClientes(diretorio + "/clientes.txt");
    sucesso &= salvarProdutos(diretorio + "/produtos.txt");

    if (sucesso) {
        cout << BG_BLUE << std::string(MARGEM) << GREEN << "Dados salvos com sucesso no diretorio '" << diretorio << "'" << RESET << endl;
    }
    else {
        cout << std::string(MARGEM) << RED << "Erro ao salvar alguns dados." << RESET << endl;
    }

    return sucesso;
}

bool Loja::carregarDados(const string& diretorio) {
    bool sucesso = true;
    sucesso &= carregarClientes(diretorio + "/clientes.txt");
    sucesso &= carregarProdutos(diretorio + "/produtos.txt");
    sucesso &= carregarVendas(diretorio + "/vendas.txt");

    if (sucesso) {
        cout << std::string(MARGEM) << GREEN << "Dados carregados com sucesso do diretorio '" << diretorio << "'" << RESET << endl;
    }
    else {
        cout << std::string(MARGEM) << YELLOW << "Aviso: Alguns dados nao puderam ser carregados." << RESET << endl;
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
                
                // Normaliza entrada decimal
                std::replace(precoStr.begin(), precoStr.end(), ',', '.');

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
    system("cls");
    preencherTela(BG_GRAY, FG_BLUE); // Preenche toda a tela de cinza
    vector<string> cabecalho = {"#", "FATURA N", "CLIENTE ID", "TOTAL VENDA", "PRODUTO", "QTD", "TOTAL C/IVA"};
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
    system("cls");
    preencherTela(BG_GRAY, FG_BLUE);
    // Definição das larguras das colunas
    const int wProd = 28, wQtd = 12;
    const int larguraTabela = wProd + wQtd + 3; // 2 pipes + 1 separador
    imprimirTituloCentralizado("RELATORIO DE STOCK", larguraTabela, string(4, ' '));
    // Cabeçalho
    cout << BG_GRAY << FG_BLUE << string(4, ' ') << "+" << string(wProd, '=') << "+" << string(wQtd, '=') << "+" << RESET << endl;
    cout << BG_GRAY << FG_BLUE << string(4, ' ') << "|" << centro("PRODUTO", wProd) << "|" << centro("QUANTIDADE", wQtd) << "|" << RESET << endl;
    cout << BG_GRAY << FG_BLUE << string(4, ' ') << string(wProd, '-') << "+" << string(wQtd, '-') << "+" << RESET << endl;
    for (const auto& p : produtos) {
        cout << BG_GRAY << FG_BLUE << string(4, ' ') << "|" << centro(toUpper(p.getNome()), wProd) << "|" << centro(to_string(p.getQuantidade()), wQtd) << "|" << RESET << endl;
    }
    cout << BG_GRAY << FG_BLUE << string(4, ' ') << "+" << string(wProd, '=') << "+" << string(wQtd, '=') << "+" << RESET << endl;
}


bool Loja::relatorioVendasPorProduto(const string& nomeProduto) const {
    system("cls");
    preencherTela(BG_GRAY, FG_BLUE);
    
    // Primeiro, verificar se existem vendas para este produto
    string nomeNormalizado = normalizarNomeProduto(nomeProduto);
    bool encontrado = false;
    // Percorrer todas as vendas válidas
    for (const auto& venda : vendas) {
        if (venda.getNumeroFatura().empty() || venda.getIdCliente() == 0) continue;
        for (const auto& item : venda.getItens()) {
            if (normalizarNomeProduto(item.nomeProduto) == nomeNormalizado) {
                encontrado = true;
                break;
            }
        }
        if (encontrado) break;
    }
    
    if (!encontrado) {
        return false;
    }
    
    // Se encontrou vendas, buscar o produto na lista de produtos cadastrados
    Produto* produto = encontrarProdutoPorNome(nomeProduto, const_cast<vector<Produto>&>(produtos));
    string nomeProdutoPadronizado = produto ? produto->getNome() : toUpper(nomeProduto);
    
    // Definição das larguras das colunas para o cabeçalho detalhado (escopo local, antes do uso)
    const int wProd = 24, wQtd = 5, wUnit = 13, wTotal = 13, wCusto = 10, wLucro = 10;
    const int nPipes = 7;
    const int larguraTabela = wProd + wQtd + wUnit + wTotal + wCusto + wLucro + nPipes;
    const string titulo = "DETALHAMENTO DE VENDAS - PRODUTO: " + toUpper(nomeProdutoPadronizado);
    // Linha dupla superior
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "+" << string(larguraTabela - 2, '=') << "+" << RESET << endl;
    // Título centralizado na largura da tabela
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "|" << centro(titulo, larguraTabela - 2) << "|" << RESET << endl;
    // Linha dupla logo após o título
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "+" << string(larguraTabela - 2, '=') << "+" << RESET << endl;
    // Cabeçalho de identificação das colunas
    imprimirCabecalhoDetalhadoTabela(std::string(MARGEM), 24, 5, 13, 13, 10, 10);

    int totalQtd = 0;
    double totalCusto = 0, totalLucro = 0, totalComIVA = 0;
    for (const auto& venda : vendas) {
        if (venda.getNumeroFatura().empty() || venda.getIdCliente() == 0) continue;
        for (const auto& item : venda.getItens()) {
            if (normalizarNomeProduto(item.nomeProduto) == nomeNormalizado) {
                double precoUnitario = item.precoUnitario;
                double custoUnitario = item.precoCusto;
                int quantidade = item.quantidade;
                double totalComIVAItem = item.totalComIVA;
                double custoTotal = custoUnitario * quantidade;
                double lucroTotal = (precoUnitario * quantidade) - custoTotal;
                string nomeProdutoStr = item.nomeProduto;
                string qtdStr = to_string(quantidade);
                string precoUnitStr = (ostringstream{} << fixed << setprecision(2) << precoUnitario << " EUR").str();
                string totalComIVAStr = (ostringstream{} << fixed << setprecision(2) << totalComIVAItem << " EUR").str();
                string custoTotalStr = (ostringstream{} << fixed << setprecision(2) << custoTotal << " EUR").str();
                string lucroTotalStr = (ostringstream{} << fixed << setprecision(2) << lucroTotal << " EUR").str();
                cout << BG_GRAY << FG_BLUE << std::string(MARGEM)
                    << "|" << centro(nomeProdutoStr, wProd)
                    << "|" << centro(qtdStr, wQtd)
                    << "|" << centro(precoUnitStr, wUnit)
                    << "|" << centro(totalComIVAStr, wTotal)
                    << "|" << centro(custoTotalStr, wCusto)
                    << "|" << centro(lucroTotalStr, wLucro)
                    << "|" << RESET << endl;
                totalQtd += quantidade;
                totalCusto += custoTotal;
                totalComIVA += totalComIVAItem;
                totalLucro += lucroTotal;
            }
        }
    }
    // Linha simples de separação
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM)
        << string(wProd, '-') << "+" << string(wQtd, '-') << "+" << string(wUnit, '-') << "+" << string(wTotal, '-') << "+" << string(wCusto, '-') << "+" << string(wLucro, '-') << "+" << RESET << endl;
    // Linha de totais
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM)
        << "|" << centro("TOTAL", wProd)
        << "|" << centro(to_string(totalQtd), wQtd)
        << "|" << centro("", wUnit)
        << "|" << centro((ostringstream{} << fixed << setprecision(2) << totalComIVA << " EUR").str(), wTotal)
        << "|" << centro((ostringstream{} << fixed << setprecision(2) << totalCusto << " EUR").str(), wCusto)
        << "|" << centro((ostringstream{} << fixed << setprecision(2) << totalLucro << " EUR").str(), wLucro)
        << "|" << RESET << endl;
    // Linha dupla após o total
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "+" << string(larguraTabela - 2, '=') << "+" << RESET << endl;
    return true;
}

void Loja::relatorioTotalVendas() const {
    system("cls");
    preencherTela(BG_GRAY, FG_BLUE);
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

    // Centralizar o total vendido e destacar o valor em vermelho, com 'EUR' ao final
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Total vendido: "
         << TEXTO_PRETO << fixed << setprecision(2) << total << BG_GRAY << FG_BLUE << " EUR" << RESET << endl;

    // Restante alinhado à esquerda
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Produto mais vendido: " << maisVendido << " (" << maxQtd << " unidades)" << RESET << endl;
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Produto menos vendido: " << menosVendido << " (" << minQtd << " unidades)" << RESET << endl;
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Lucro do mais vendido: " << lucroMaisVendido << " EUR" << RESET << endl;
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Cliente que mais comprou (ID):" << idTopCliente << " com " << maiorCompra << " EUR" << RESET << endl;

    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "+" << string(largura - 2, '=') << "+" << RESET << endl;
}


void Loja::relatorioGraficoVendas() const {
    system("cls");
    preencherTela(BG_GRAY, FG_BLUE);
    const int largura = 86;
    imprimirTituloCentralizado("GRAFICO DE VENDAS POR PRODUTO", largura, string(4, ' '));

    // Mapeia vendas por produto
    map<string, double> valorPorProduto;
    for (const auto& venda : vendas) {
        for (const auto& item : venda.getItens()) {
            valorPorProduto[item.nomeProduto] += item.totalComIVA;
        }
    }

    if (valorPorProduto.empty()) {
        cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Nenhuma venda registrada.\n" << RESET;
        return;
    }

    // Descobrir o maior valor vendido para normalizar as barras
    double maxValor = 0.0;
    for (const auto& par : valorPorProduto) {
        if (par.second > maxValor) maxValor = par.second;
    }
    const int barraMax = 40; // tamanho máximo da barra

    cout << BG_GRAY << FG_BLUE;
    for (const auto& par : valorPorProduto) {
        int barLen = (int)(barraMax * par.second / maxValor);
        cout << std::string(MARGEM)
             << left << setw(20) << par.first << " | "
             << string(barLen, '#')
             << " " << fixed << setprecision(2) << par.second << " EUR" << endl;
    }
    cout << RESET;
}


void Loja::relatorioVendasDetalhadoPorProduto() const {
    system("cls");
    preencherTela(BG_GRAY, FG_BLUE);
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
    cout << RESET;

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

    // Cabeçalho de identificação das colunas
    imprimirCabecalhoDetalhadoTabela(std::string(MARGEM), 25, 6, 12, 14, 14, 14);

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
        ossPreco << fixed << setprecision(2) << preco << " EUR";
        ossReceita << fixed << setprecision(2) << receita << " EUR";
        ossCusto << fixed << setprecision(2) << custo << " EUR";
        ossLucro << fixed << setprecision(2) << lucro << " EUR";

        cout << BG_GRAY << FG_BLUE << std::string(MARGEM)
            << "|" << centro(nome, wProd)
            << "|" << centro(to_string(qtd), wQtd)
            << "|" << centro(ossPreco.str(), wPreco)
            << "|" << centro(ossReceita.str(), wReceita)
            << "|" << centro(ossCusto.str(), wCusto)
            << "|" << centro(ossLucro.str(), wLucro)
            << "|" << RESET << endl;
    }

    // Total alinhado como linha da tabela
    std::ostringstream ossValor;
    ossValor << std::fixed << std::setprecision(2) << totalLucro << " EUR";
    std::string texto = "Lucro total estimado:";
    int larguraValor = 14; // igual à largura da coluna de lucro
    int larguraTexto = larguraTabela - 2 - larguraValor;
    cout << BG_GRAY << FG_BLUE << std::string(MARGEM)
         << "|" << left << setw(larguraTexto) << texto
         << right << setw(larguraValor) << ossValor.str() << "|" << RESET << endl;
    cout << BG_GRAY << FG_BLUE << string(4, ' ') << "+" << string(larguraTabela - 2, '=') << "+" << RESET << endl;
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
