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
            cout << "\nPressione Enter para voltar...";
            cin.get();
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
                case 1: { // Criar Produto
                    system("cls");
                    string nome;
                    int qtd;
                    double preco;
                    cout << "ADICIONAR NOVO PRODUTO\n\n";
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    while (true) {
                        cout << "Nome: ";
                        getline(cin, nome);
                        nome = toUpper(nome);
                        if (!(nome.empty() || nome.find_first_not_of(' ') == string::npos)) {
                            break;
                        }
                        cout << RED << "Nome do produto nao pode ser vazio!" << RESET << endl;
                    }
                    qtd = lernumero("Quantidade: ");
                    preco = lerFloatPositivo("Preco de custo: ");
                    loja.criarProduto(nome, qtd, preco);


                    cout << GREEN << "\nProduto criado com sucesso!" << RESET;
                    cout << "\n\nPressione Enter para continuar...";
                    limparBuffer();
                    cin.get();
                    break;
                }
                case 2:
                { // Adicionar Stock
                    system("cls");
                    loja.listarProdutos();
                    cout << "\n";
                    int id = lernumero("ID do Produto: ");
                    int qtd = lernumero("Quantidade a adicionar: ");
                    loja.adicionarStock(id, qtd);

                    cout << GREEN << "\nStock atualizado com sucesso!" << RESET;
                    cout << "\n\nPressione Enter para continuar...";
                    limparBuffer();
                    cin.get();
                    break;
                }
                case 3:
                { // Eliminar Produto
                    system("cls");
                    loja.listarProdutos();
                    cout << "\n";
                    int id = lernumero("ID do Produto a eliminar: ");
                    loja.eliminarProduto(id);

                    cout << GREEN << "\nProduto removido com sucesso!" << RESET;
                    cout << "\n\nPressione Enter para continuar...";
                    limparBuffer();
                    cin.get();
                    break;
                }
                case 4:
                { // Listar Produtos
                    system("cls");
                    loja.listarProdutos();
                    cout << "\nPressione Enter para voltar...";
                    limparBuffer();
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
                    string nome, tel, morada;
                    cout << "CADASTRO DE CLIENTE\n\n";
                    cin.ignore();
                    cout << "Nome: ";
                    getline(cin, nome);
                    nome = toUpper(nome);
                    cout << "Telefone: ";
                    tel = lerTelefone("");
                    cout << "Morada: ";
                    getline(cin, morada);
                    morada = toUpper(morada);
                    loja.criarCliente(nome, tel, morada);

                    cout << GREEN << "\nCliente cadastrado com sucesso!" << RESET;
                    cout << "\n\nPressione Enter para continuar...";
                    limparBuffer();
                    cin.get();
                    break;
                }
                case 2:
                { // Eliminar Cliente
                    system("cls");
                    loja.listarClientes();
                    cout << "\n";
                    int id = lernumero("ID do Cliente a remover: ");
                    loja.eliminarCliente(id);

                    cout << GREEN << "\nCliente removido com sucesso!" << RESET;
                    cout << "\n\nPressione Enter para continuar...";
                    limparBuffer();
                    cin.get();
                    break;
                }
                case 3:
                { // Listar Clientes
                    system("cls");
                    loja.listarClientes();
                    cout << "\nPressione Enter para voltar...";
                    limparBuffer();
                    cin.get();
                    break;
                }
                case 4:
                { // Alterar Dados
                    system("cls");
                    loja.listarClientes();
                    cout << "\n";
                    int id = lernumero("ID do Cliente: ");
                    string novoNome;
                    cout << "Novo nome: ";
                    cin.ignore();
                    getline(cin, novoNome);
                    novoNome = toUpper(novoNome);
                    loja.alterarNomeCliente(id, novoNome);

                    cout << GREEN << "\nNome alterado com sucesso!" << RESET;
                    cout << "\n\nPressione Enter para continuar...";
                    limparBuffer();
                    cin.get();
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
                    limparBuffer();
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
                    limparBuffer();
                    cin.get();
                    break;
                }
                case 3: // Total de Vendas
                    system("cls");
                    loja.relatorioTotalVendas();
                    cout << "\nPressione Enter para voltar...";
                    limparBuffer();
                    cin.get();
                    break;
                case 4: // Gráfico de Vendas
                    system("cls");
                    loja.relatorioGraficoVendas();
                    cout << "\nPressione Enter para voltar...";
                    limparBuffer();
                    cin.get();
                    break;
                case 5: // Histórico de Vendas
                    system("cls");
                    loja.listarHistoricoVendas();
                    cout << "\nPressione Enter para voltar...";
                    limparBuffer();
                    cin.get();
                    break;
                case 6: // Vendas Detalhadas
                    system("cls");
                    loja.relatorioVendasDetalhadoPorProduto();
                    cout << "\nPressione Enter para voltar...";
                    limparBuffer();
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