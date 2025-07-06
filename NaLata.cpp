#define NOMINMAX
#include <iomanip>
#include <string>
#include <ctime>
#include <sstream>
#include <iostream>
#include <limits>
#include <windows.h>
#include <locale>
#include <algorithm>
#include "Loja.h"
#include "Menu.h"
#include "Auxiliar.h"
#include <regex>

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
            loja.efetuarVenda();
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
                    do {
                        system("cls");
                        preencherTela(BG_GRAY, FG_BLUE);
                        cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "REGISTO DE PRODUTO" << RESET << endl;
                        cin.clear();
                        string nome;
                        int quantidade;
                        double preco;
                        
                        // Solicitar nome do produto
                        while (true) {
                            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Nome do produto: " << BG_GRAY << flush;
                            getline(cin, nome);
                            cout << RESET;
                            nome = toUpper(nome);
                            if (!nome.empty() && nome.find_first_not_of(' ') != string::npos) {
                                break;
                            }
                            cout << BG_GRAY << RED << std::string(MARGEM) << "Nome do produto nao pode ser vazio!" << RESET << endl;
                        }

                        // Verifica se o produto já existe
                        Produto* existente = encontrarProdutoPorNome(nome, loja.getProdutos());
                        if (existente != nullptr) {
                            cout << BG_GRAY << YELLOW << std::string(MARGEM) << "Produto ja existe." << RESET << endl;
                            cout << BG_GRAY << std::string(MARGEM) << "Quantidade atual: " << existente->getQuantidade() << RESET << endl;
                            cout << BG_GRAY << std::string(MARGEM) << "Preco de custo atual: " << fixed << setprecision(2) << existente->getPrecoCusto() << RESET << endl;
                            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Deseja atualizar a quantidade e o preco de custo? (s/n): " << BG_GRAY << flush;
                            char opcao;
                            while (true) {
                                cin >> opcao;
                                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                opcao = toupper(opcao);
                                if (opcao == 'S' || opcao == 'N') break;
                                cout << BG_GRAY << RED << std::string(MARGEM) << "Entrada invalida. Digite 's' para sim ou 'n' para nao." << RESET << endl;
                            }
                            if (opcao == 'S') {
                                int qnt;
                                cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Quantidade a adicionar: " << BG_GRAY << flush;
                                bool primeiraTentativaQuantidade = true;
                                while (true) {
                                    if (!primeiraTentativaQuantidade) {
                                        cout << BG_GRAY << RED << std::string(MARGEM) << "Entrada invalida. Digite um numero inteiro maior que 0." << RESET << endl;
                                    }
                                    string input;
                                    getline(cin, input);
                                    cout << RESET;
                                    istringstream iss(input);
                                    if ((iss >> qnt) && qnt > 0) break;
                                    primeiraTentativaQuantidade = false;
                                }
                                existente->adicionarStock(qnt);

                                // Pedir novo preço de custo (opcional)
                                cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Novo preco de custo (pressione Enter para manter o atual): " << BG_GRAY << flush;
                                string inputPreco;
                                getline(cin, inputPreco);
                                cout << RESET;
                                                            if (!inputPreco.empty()) {
                                // Normaliza entrada decimal
                                inputPreco = normalizarDecimal(inputPreco);
                                istringstream iss(inputPreco);
                                    double novoPreco;
                                    if ((iss >> novoPreco) && novoPreco >= 0.0) {
                                        char extra;
                                        if (!(iss >> extra)) {
                                            existente->setPrecoCusto(novoPreco);
                                            cout << BG_GRAY << GREEN << std::string(MARGEM) << "Preco de custo atualizado com sucesso!" << RESET << endl;
                                        } else {
                                            cout << BG_GRAY << RED << std::string(MARGEM) << "Valor invalido. Preco de custo mantido." << RESET << endl;
                                        }
                                    } else {
                                        cout << BG_GRAY << RED << std::string(MARGEM) << "Valor invalido. Preco de custo mantido." << RESET << endl;
                                    }
                                }
                            } else {
                                cout << BG_GRAY << RED << std::string(MARGEM) << "Operacao cancelada." << RESET << endl;
                            }
                            continue;
                        }

                        // Se o produto não existir, continua o cadastro normal
                        bool primeiraTentativaQuantidade = true;
                        while (true) {
                            if (!primeiraTentativaQuantidade) {
                                cout << BG_GRAY << RED << std::string(MARGEM) << "Entrada invalida. Digite um numero inteiro maior que 0." << RESET << endl;
                            }
                            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Quantidade: " << BG_GRAY << flush;
                            string input;
                            getline(cin, input);
                            cout << RESET;
                            istringstream iss(input);
                            if ((iss >> quantidade) && quantidade > 0) break;
                            primeiraTentativaQuantidade = false;
                        }
                        
                        // Solicitar preço de custo
                        bool primeiraTentativaPreco = true;
                        while (true) {
                            if (!primeiraTentativaPreco) {
                                cout << BG_GRAY << RED << std::string(MARGEM) << "Entrada invalida. Digite um numero maior ou igual a 0." << RESET << endl;
                            }
                            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Preco de custo: " << BG_GRAY << flush;
                            string input;
                            getline(cin, input);
                            cout << RESET;
                            // Normaliza entrada decimal
                            input = normalizarDecimal(input);
                            istringstream iss(input);
                            if ((iss >> preco) && preco >= 0.0) {
                                char extra;
                                if (!(iss >> extra)) break;
                            }
                            primeiraTentativaPreco = false;
                        }
                        
                        loja.criarProduto(nome, quantidade, preco);
                        cout << BG_GRAY << std::string(MARGEM) << GREEN << "Produto registado com sucesso!" << RESET << endl;
                    } while (confirmarAcao("Deseja adicionar outro produto?"));
                    break;
                }
                case 2:
                { // Adicionar Stock
                    system("cls");
                    preencherTela(BG_GRAY, FG_BLUE);
                    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "ADICIONAR STOCK" << RESET << endl << endl;
                    do {
                        system("cls");
                        preencherTela(BG_GRAY, FG_BLUE);
                        loja.listarProdutos();
                        cin.clear();
                        int id;
                        bool primeiraTentativaId = true;
                        while (true) {
                            if (!primeiraTentativaId) {
                                cout << BG_GRAY << RED << std::string(MARGEM) << "ID invalido. Escolha um ID da lista acima." << RESET << endl;
                            }
                            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "ID do Produto: " << BG_GRAY << flush;
                            string input;
                            getline(cin, input);
                            cout << RESET;
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
                                cout << BG_GRAY << RED << std::string(MARGEM) << "Entrada invalida. Digite um numero inteiro maior que 0." << RESET << endl;
                            }
                            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Quantidade a adicionar: " << BG_GRAY << flush;
                            string input;
                            getline(cin, input);
                            cout << RESET;
                            istringstream iss(input);
                            if ((iss >> qtd) && qtd > 0) break;
                            primeiraTentativa = false;
                        }
                        loja.adicionarStock(id, qtd);
                    } while (confirmarAcao("Deseja adicionar estoque para outro produto?"));
                    break;
                }
                case 3:
                { // Eliminar Produto
                    system("cls");
                    preencherTela(BG_GRAY, FG_BLUE);
                    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "ELIMINAR PRODUTO" << RESET << endl << endl;
                    do {
                        system("cls");
                        preencherTela(BG_GRAY, FG_BLUE);
                        loja.listarProdutos();
                        cin.clear();
                        int id;
                        bool primeiraTentativaId = true;
                        while (true) {
                            if (!primeiraTentativaId) {
                                cout << BG_GRAY << RED << std::string(MARGEM) << "ID invalido. Escolha um ID da lista acima." << RESET << endl;
                            }
                            id = lerIDPositivo(std::string(BG_GRAY) + std::string(FG_BLUE) + std::string(MARGEM) + "ID do Produto a eliminar: ");
                            // Verifica se o ID existe na lista de produtos
                            bool existe = false;
                            for (const auto& p : loja.getProdutos()) {
                                if (p.getId() == id) {
                                    existe = true;
                                    break;
                                }
                            }
                            if (existe) break;
                            primeiraTentativaId = false;
                        }
                        // Confirmação antes de eliminar
                        string nomeProduto;
                        for (const auto& p : loja.getProdutos()) {
                            if (p.getId() == id) {
                                nomeProduto = p.getNome();
                                break;
                            }
                        }
                        char confirma;
                        do {
                            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Tem certeza que deseja eliminar o produto '" << nomeProduto << "'? (s/n): " << BG_GRAY << flush;
                            string inputConfirma;
                            getline(cin, inputConfirma);
                            cout << RESET;
                            if (!inputConfirma.empty()) confirma = toupper(inputConfirma[0]);
                            else confirma = 'N';
                        } while (confirma != 'S' && confirma != 'N');
                        if (confirma == 'S') {
                            if (loja.eliminarProduto(id)) {
                                cout << BG_GRAY << std::string(MARGEM) << GREEN << "Produto removido com sucesso!" << RESET << endl;
                            } else {
                                cout << BG_GRAY << YELLOW << std::string(MARGEM) << "Produto nao removido. Verifique o ID ou o estoque antes de tentar remover." << RESET << endl;
                            }
                        } else {
                            cout << BG_GRAY << YELLOW << std::string(MARGEM) << "Operacao cancelada pelo usuario." << RESET << endl;
                        }
                    } while (confirmarAcao("Deseja eliminar outro produto?"));
                    break;
                }
                case 4:
                { // Listar Produtos
                    system("cls");
                    preencherTela(BG_GRAY, FG_BLUE);
                    loja.listarProdutos();
                    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Pressione Enter para voltar..." << RESET;
                    cin.get();
                    break;
                }
                case 5: // Voltar ao Menu Principal
                    break;
                default:
                    cout << BG_BLUE << RED << std::string(MARGEM) << "Opcao invalida!" << RESET << endl;
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
                    do {
                        system("cls");
                        preencherTela(BG_GRAY, FG_BLUE);
                        cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "REGISTO DE CLIENTE" << RESET << endl;
                        cin.clear();
                        string nome, tel, morada, dataNasc;
                        while (true) {
                            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Nome: " << BG_GRAY << flush;
                            getline(cin, nome);
                            cout << RESET;
                            nome = toUpper(nome);
                            if (!(nome.empty() || nome.find_first_not_of(' ') == string::npos)) {
                                break;
                            }
                            cout << BG_GRAY << RED << std::string(MARGEM) << "Nome do cliente nao pode ser vazio!" << RESET << endl;
                        }
                        // Verificar duplicidade de nome
                        if (encontrarClientePorNome(nome, loja.getClientes())) {
                            cout << BG_GRAY << RED << std::string(MARGEM) << "Cliente com este nome ja existe!" << RESET << endl;
                            continue;
                        }
                        // Garantir telefone válido
                        while (true) {
                            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Telefone: " << BG_GRAY << flush;
                            getline(cin, tel);
                            cout << RESET;
                            // Validação: exatamente 9 dígitos
                            if (tel.length() == 9 && std::all_of(tel.begin(), tel.end(), ::isdigit)) {
                                break;
                            }
                            cout << BG_GRAY << RED << std::string(MARGEM) << "Telefone invalido. Digite exatamente 9 numeros." << RESET << endl;
                        }
                        // Garantir morada não vazia
                        while (true) {
                            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Morada: " << BG_GRAY << flush;
                            getline(cin, morada);
                            cout << RESET;
                            morada = toUpper(morada);
                            if (!(morada.empty() || morada.find_first_not_of(' ') == string::npos)) {
                                break;
                            }
                            cout << BG_GRAY << RED << std::string(MARGEM) << "Morada nao pode ser vazia!" << RESET << endl;
                        }
                        // Solicitar data de nascimento
                        while (true) {
                            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Data de nascimento (dd-mm-aaaa): " << BG_GRAY << flush;
                            getline(cin, dataNasc);
                            cout << RESET;
                            if (regex_match(dataNasc, regex("^\\d{2}-\\d{2}-\\d{4}$"))) break;
                            cout << BG_GRAY << RED << std::string(MARGEM) << "Data invalida! Use o formato dd-mm-aaaa." << RESET << endl;
                        }
                        loja.criarCliente(nome, tel, morada, dataNasc);

                        cout << BG_GRAY << std::string(MARGEM) << GREEN << "Cliente registado com sucesso!" << RESET << endl;
                    } while (confirmarAcao("Deseja registar outro cliente?"));
                    break;
                }
                case 2:
                { // Eliminar Cliente
                    system("cls");
                    preencherTela(BG_GRAY, FG_BLUE);
                    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "ELIMINAR CLIENTE" << RESET << endl << endl;
                    do {
                        system("cls");
                        preencherTela(BG_GRAY, FG_BLUE);
                        loja.listarClientes();
                        cin.clear();
                        int id;
                        while (true) {
                            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "ID do Cliente a remover: " << BG_GRAY << flush;
                            string input;
                            getline(cin, input);
                            cout << RESET;
                            cout << endl;
                            istringstream iss(input);
                            if ((iss >> id) && id > 0) {
                                break;
                            }
                            cout << BG_GRAY << RED << std::string(MARGEM) << "Entrada invalida. Digite um numero inteiro maior que 0." << RESET << endl;
                        }
                        loja.eliminarCliente(id);
                    } while (confirmarAcao("Deseja remover outro cliente?"));
                    break;
                }
                case 3:
                { // Listar Clientes
                    system("cls");
                    preencherTela(BG_GRAY, FG_BLUE);
                    loja.listarClientes();
                    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Pressione Enter para voltar..." << RESET;
                    cin.get();
                    break;
                }
                case 4:
                { // Alterar Dados
                    do {
                        system("cls");
                        preencherTela(BG_GRAY, FG_BLUE);
                        cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "ALTERACAO DE DADOS DO CLIENTE" << RESET << endl << endl;
                        loja.listarClientes();
                        cout << endl;
                        cin.clear();
                        int id;
                        while (true) {
                            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "ID do Cliente: " << BG_GRAY << flush;
                            string input;
                            getline(cin, input);
                            cout << RESET;
                            istringstream iss(input);
                            if ((iss >> id) && id > 0) {
                                break;
                            }
                            cout << BG_GRAY << RED << std::string(MARGEM) << "Entrada invalida. Digite um numero inteiro maior que 0." << RESET << endl;
                        }
                        string novoNome;
                        cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Novo nome: " << BG_GRAY << flush;
                        getline(cin, novoNome);
                        cout << RESET;
                        novoNome = toUpper(novoNome);
                        loja.alterarNomeCliente(id, novoNome);
                    } while (confirmarAcao("Deseja alterar outro cliente?"));
                    break;
                }
                case 5: // Voltar ao Menu Principal
                    break;
                default:
                    cout << BG_BLUE << RED << std::string(MARGEM) << "Opcao invalida!" << RESET << endl;
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
                    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Pressione Enter para voltar..." << RESET;
                    cin.get();
                    break;
                case 2:
                { // Vendas por Produto
                    do {
                        system("cls");
                        preencherTela(BG_GRAY, FG_BLUE);
                        string nome;
                        cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Relatorio por Produto" << RESET << endl << endl;
                        cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Nome do produto: " << BG_GRAY << flush;
                        if (cin.peek() == '\n') cin.ignore();
                        getline(cin, nome);
                        cout << RESET;
                        nome = trim(toUpper(nome));
                        bool encontrou = loja.relatorioVendasPorProduto(nome);
                        if (!encontrou) {
                            cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Nenhuma venda encontrada para este produto." << RESET << endl;
                        }
                    } while (confirmarAcao("Deseja ver o relatorio de outro produto?"));
                    break;
                }
                case 3: // Total de Vendas
                    system("cls");
                    loja.relatorioTotalVendas();
                    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Pressione Enter para voltar..." << RESET;
                    cin.get();
                    break;
                case 4: // Gráfico de Vendas
                    system("cls");
                    loja.relatorioGraficoVendas();
                    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Pressione Enter para voltar..." << RESET;
                    cin.get();
                    break;
                case 5: // Histórico de Vendas
                    system("cls");
                    loja.listarHistoricoVendas();
                    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Pressione Enter para voltar..." << RESET;
                    cin.get();
                    break;
                case 6: // Vendas Detalhadas
                    system("cls");
                    loja.relatorioVendasDetalhadoPorProduto();
                    cout << BG_GRAY << FG_BLUE << std::string(MARGEM) << "Pressione Enter para voltar..." << RESET;
                    cin.get();
                    break;
                case 7: // Voltar ao Menu Principal
                    break;
                default:
                    cout << BG_BLUE << RED << std::string(MARGEM) << "Opcao invalida!" << RESET << endl;
                    Sleep(1000);
                }
            } while (sub_relatorios != 7);
            break;
        }
        case 5: // Sair
            loja.salvarDados();
            return 0;
        default:
            cout << BG_BLUE << RED << std::string(MARGEM) << "Opcao invalida!" << RESET << endl;
            Sleep(1000);
            system("cls");
        }
    } while (opcao != 5);

    return 0;
}