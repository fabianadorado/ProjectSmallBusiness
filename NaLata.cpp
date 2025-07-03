#define NOMINMAX
#include <iomanip>
#include <string>
#include <ctime>
#include <sstream>
#include <iostream>
#include <limits>
#include <windows.h>
#include <locale>
#include "Loja.h"
#include "Menu.h"
#include "Auxiliar.h"


#define RESET   "\033[0m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define BOLD    "\033[1m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"

using namespace std;

int main() {
    // Locale para portugues e UTF-8
    // setlocale(LC_ALL, "pt_PT.UTF-8"); // Força acentuação correta
    // setlocale(LC_NUMERIC, "C");

    Menu menu;
    Loja loja;

    // Carrega os dados
    loja.carregarDados();

    system("cls");

    int opcao;
    do {
        opcao = menu.menuPrincipal();

        switch (opcao)
        {
        case 1:
        { // Efetuar Vendas
            system("cls");
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            int id = lernumero("ID do cliente: ");
            loja.efetuarVenda(id);
            system("cls");
            break;
        }
        case 2:
        { // Gerir Produtos
            int sub_produtos;
            do {
                system("cls");
                sub_produtos = menu.menuGerirProdutos();

                switch (sub_produtos)
                {
                case 1:
                { // Adicionar Produto
                    system("cls");
                    bool primeiraVez = true;
                    do {
                        if (!primeiraVez) system("cls");
                        string nome;
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        cout << "Nome: ";
                        getline(cin, nome);
                        nome = toUpper(nome);
                        // Verificar duplicidade de produto
                        Produto* existente = encontrarProdutoPorNome(nome, loja.getProdutos());
                        if (existente) {
                            cout << YELLOW << "Produto ja existe." << RESET << endl;
                            cout << "Quantidade atual: " << existente->getQuantidade() << endl;
                            cout << "Preco de custo atual: " << fixed << setprecision(2) << existente->getPrecoCusto() << endl;
                            char opcao;
                            while (true) {
                                cout << "Deseja atualizar a quantidade e o preco de custo? (s/n): ";
                                cin >> opcao;
                                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                if (opcao == 's' || opcao == 'S' || opcao == 'n' || opcao == 'N') break;
                                cout << RED << "Entrada invalida. Digite 's' para sim ou 'n' para nao." << RESET << endl;
                            }
                            if (opcao == 's' || opcao == 'S') {
                                int qtdNova;
                                double precoNovo;
                                bool primeiraTentativa = true;
                                while (true) {
                                    if (!primeiraTentativa) {
                                        cout << RED << "Entrada invalida. Digite um numero inteiro maior que 0." << RESET << endl;
                                    }
                                    cout << "Nova quantidade a adicionar: ";
                                    string input;
                                    getline(cin, input);
                                    istringstream iss(input);
                                    if ((iss >> qtdNova) && qtdNova > 0) break;
                                    primeiraTentativa = false;
                                }
                                precoNovo = lerFloatPositivo("Novo preco de custo: ");
                                existente->adicionarStock(qtdNova);
                                existente->setPrecoCusto(precoNovo);
                                cout << GREEN << "\nEstoque e preco atualizados com sucesso!" << RESET << endl;
                            } else {
                                cout << YELLOW << "Operacao cancelada pelo usuario." << RESET << endl;
                            }
                            primeiraVez = false;
                            continue;
                        } else {
                            int qtd;
                            double preco;
                            bool primeiraTentativa = true;
                            while (true) {
                                if (!primeiraTentativa) {
                                    cout << RED << "Entrada invalida. Digite um numero inteiro maior que 0." << RESET << endl;
                                }
                                cout << "Quantidade: ";
                                string input;
                                getline(cin, input);
                                istringstream iss(input);
                                if ((iss >> qtd) && qtd > 0) break;
                                primeiraTentativa = false;
                            }
                            preco = lerFloatPositivo("Preco de custo: ");
                            loja.criarProduto(nome, qtd, preco);
                            cout << GREEN << "\nProduto criado com sucesso!" << RESET << endl;
                        }
                        primeiraVez = false;
                    } while (desejaContinuar("Deseja adicionar outro produto?"));
                    break;
                }
                case 2:
                { // Adicionar Stock
                    system("cls");
                    bool primeiraVez = true;
                    do {
                        if (!primeiraVez) system("cls");
                        loja.listarProdutos();
                        cout << "\n";
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        int id;
                        bool primeiraTentativaId = true;
                        while (true) {
                            if (!primeiraTentativaId) {
                                cout << RED << "ID invalido. Escolha um ID da lista acima." << RESET << endl;
                            }
                            cout << "ID do Produto: ";
                            string input;
                            getline(cin, input);
                            istringstream iss(input);
                            if ((iss >> id) && id > 0) {
                                // Verifica se o ID existe na lista de produtos
                                bool existe = false;
                                for (const auto& p : loja.getProdutos()) {
                                    if (p.getId() == id) {
                                        existe = true;
                                        break;
                                    }
                                }
                                if (existe) break;
                            }
                            primeiraTentativaId = false;
                        }
                        int qtd;
                        bool primeiraTentativa = true;
                        while (true) {
                            if (!primeiraTentativa) {
                                cout << RED << "Entrada invalida. Digite um numero inteiro maior que 0." << RESET << endl;
                            }
                            cout << "Quantidade a adicionar: ";
                            string input;
                            getline(cin, input);
                            istringstream iss(input);
                            if ((iss >> qtd) && qtd > 0) break;
                            primeiraTentativa = false;
                        }
                        loja.adicionarStock(id, qtd);

                        cout << GREEN << "\nStock atualizado com sucesso!" << RESET << endl;
                        primeiraVez = false;
                    } while (desejaContinuar("Deseja adicionar estoque para outro produto?"));
                    break;
                }
                case 3:
                { // Eliminar Produto
                    system("cls");
                    bool primeiraVez = true;
                    do {
                        if (!primeiraVez) system("cls");
                        loja.listarProdutos();
                        cout << "\n";
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        int id;
                        bool primeiraTentativaId = true;
                        while (true) {
                            if (!primeiraTentativaId) {
                                cout << RED << "ID invalido. Escolha um ID da lista acima." << RESET << endl;
                            }
                            cout << "ID do Produto a eliminar: ";
                            string input;
                            getline(cin, input);
                            istringstream iss(input);
                            if ((iss >> id) && id > 0) {
                                // Verifica se o ID existe na lista de produtos
                                bool existe = false;
                                for (const auto& p : loja.getProdutos()) {
                                    if (p.getId() == id) {
                                        existe = true;
                                        break;
                                    }
                                }
                                if (existe) break;
                            }
                            primeiraTentativaId = false;
                        }
                        loja.eliminarProduto(id);

                        cout << GREEN << "\nProduto removido com sucesso!" << RESET << endl;
                        primeiraVez = false;
                    } while (desejaContinuar("Deseja eliminar outro produto?"));
                    break;
                }
                case 4:
                { // Listar Produtos
                    system("cls");
                    loja.listarProdutos();
                    cout << "\nPressione Enter para voltar...";
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    cin.get();
                    break;
                }
                case 5: // Voltar ao Menu Principal
                    break;
                default:
                    cout << RED << "\nOpcao invalida!" << RESET;
                    Sleep(1000);
                }
            } while (sub_produtos != 5);
            break;
        }
        case 3:
        { // Gerir Clientes
            int sub_clientes;
            do {
                system("cls");
                sub_clientes = menu.menuGerirClientes();

                switch (sub_clientes)
                {
                case 1:
                { // Criar Cliente
                    system("cls");
                    cout << "CADASTRO DE CLIENTE\n\n";
                    bool primeiraVez = true;
                    do {
                        if (!primeiraVez) system("cls");
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        string nome, tel, morada;
                        while (true) {
                            cout << "Nome: ";
                            getline(cin, nome);
                            nome = toUpper(nome);
                            if (!(nome.empty() || nome.find_first_not_of(' ') == string::npos)) {
                                break;
                            }
                            cout << RED << "Nome do cliente nao pode ser vazio!" << RESET << endl;
                        }
                        // Verificar duplicidade de nome
                        if (encontrarClientePorNome(nome, loja.getClientes())) {
                            cout << RED << "Cliente com este nome ja existe!" << RESET << endl;
                            continue;
                        }
                        // Garantir telefone válido
                        while (true) {
                            cout << "Telefone: ";
                            tel = lerTelefone("");
                            if (!tel.empty()) break;
                            cout << RED << "Telefone nao pode ser vazio!" << RESET << endl;
                        }
                        // Garantir morada não vazia
                        while (true) {
                            cout << "Morada: ";
                            getline(cin, morada);
                            morada = toUpper(morada);
                            if (!(morada.empty() || morada.find_first_not_of(' ') == string::npos)) {
                                break;
                            }
                            cout << RED << "Morada nao pode ser vazia!" << RESET << endl;
                        }
                        loja.criarCliente(nome, tel, morada);

                        cout << GREEN << "\nCliente cadastrado com sucesso!" << RESET << endl;
                        primeiraVez = false;
                    } while (desejaContinuar("Deseja cadastrar outro cliente?"));
                    break;
                }
                case 2:
                { // Eliminar Cliente
                    bool primeiraVez = true;
                    do {
                        if (!primeiraVez) system("cls");
                        loja.listarClientes();
                        cout << "\n";
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        int id;
                        bool primeiraTentativaId = true;
                        while (true) {
                            if (!primeiraTentativaId) {
                                cout << RED << "Entrada invalida. Digite um numero inteiro maior que 0." << RESET << endl;
                            }
                            cout << "ID do Cliente a remover: ";
                            string input;
                            getline(cin, input);
                            istringstream iss(input);
                            if ((iss >> id) && id > 0) break;
                            primeiraTentativaId = false;
                        }
                        if (confirmarAcao("Tem certeza que deseja remover este cliente?")) {
                            loja.eliminarCliente(id);
                            cout << GREEN << "\nCliente removido com sucesso!" << RESET << endl;
                        } else {
                            cout << YELLOW << "\nOperacao cancelada pelo usuario." << RESET << endl;
                        }
                        primeiraVez = false;
                    } while (desejaContinuar("Deseja remover outro cliente?"));
                    break;
                }
                case 3:
                { // Listar Clientes
                    system("cls");
                    loja.listarClientes();
                    cout << "\nPressione Enter para voltar...";
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    cin.get();
                    break;
                }
                case 4:
                { // Alterar Dados
                    system("cls");
                    cout << "ALTERACAO DE DADOS DO CLIENTE\n\n";
                    loja.listarClientes();
                    cout << "\n";
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    int id;
                    bool primeiraTentativa = true;
                    while (true) {
                        if (!primeiraTentativa) {
                            cout << RED << "Entrada invalida. Digite um numero inteiro maior que 0." << RESET << endl;
                        }
                        cout << "ID do Cliente: ";
                        string input;
                        getline(cin, input);
                        istringstream iss(input);
                        if ((iss >> id) && id > 0) break;
                        primeiraTentativa = false;
                    }
                    string novoNome;
                    cout << "Novo nome: ";
                    cin.ignore();
                    getline(cin, novoNome);
                    novoNome = toUpper(novoNome);
                    loja.alterarNomeCliente(id, novoNome);

                    cout << GREEN << "\nNome alterado com sucesso!" << RESET << endl;
                    if (!desejaContinuar("Deseja alterar outro cliente?")) {
                        break;
                    }
                    system("cls");
                    cout << "ALTERACAO DE DADOS DO CLIENTE\n\n";
                    loja.listarClientes();
                    cout << "\n";
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
                case 5: // Voltar ao Menu Principal
                    break;
                default:
                    cout << RED << "\nOpcao invalida!" << RESET;
                    Sleep(1000);
                }
            } while (sub_clientes != 5);
            break;
        }
        case 4:
        { // Relatórios
            int sub_relatorios;
            do {
                system("cls");
                sub_relatorios = menu.menuRelatorios();

                switch (sub_relatorios)
                {
                case 1: // Relatório de Stock
                    system("cls");
                    loja.relatorioStock();
                    cout << "\nPressione Enter para voltar...";
                    cin.get();
                    break;
                case 2:
                { // Vendas por Produto
                    system("cls");
                    string nome;
                    cout << "Relatorio por Produto\n\n";
                    cout << "Nome do produto: ";
                    cin.ignore();
                    getline(cin, nome);
                    nome = toUpper(nome);
                    loja.relatorioVendasPorProduto(nome);
                    cout << "\nPressione Enter para voltar...";
                    cin.get();
                    break;
                }
                case 3: // Total de Vendas
                    system("cls");
                    loja.relatorioTotalVendas();
                    cout << "\nPressione Enter para voltar...";
                    cin.get();
                    break;
                case 4: // Gráfico de Vendas
                    system("cls");
                    loja.relatorioGraficoVendas();
                    cout << "\nPressione Enter para voltar...";
                    cin.get();
                    break;
                case 5: // Histórico de Vendas
                    system("cls");
                    loja.listarHistoricoVendas();
                    cout << "\nPressione Enter para voltar...";
                    cin.get();
                    break;
                case 6: // Vendas Detalhadas
                    system("cls");
                    loja.relatorioVendasDetalhadoPorProduto();
                    cout << "\nPressione Enter para voltar...";
                    cin.get();
                    break;
                case 7: // Voltar ao Menu Principal
                    break;
                default:
                    cout << RED << "\nOpcao invalida!" << RESET;
                    Sleep(1000);
                }
            } while (sub_relatorios != 7);
            break;
        }
        case 5: // Sair
            loja.salvarDados();
            cout << GREEN << "\nEncerrando sistema..." << RESET << endl;
            break;
        default:
            cout << RED << "Opcao invalida!" << RESET;
            Sleep(1000);
            system("cls");
        }
    } while (opcao != 5);

    return 0;
}