#include "Menu.h"
#include "Auxiliar.h"
#include <iostream>
#include <iomanip>
#include <windows.h> 

using namespace std;

#define RESET   "\033[0m"
#define CYAN    "\033[36m"
#define GREEN   "\033[38;5;22m"
#define BOLD    "\033[1m"
#define YELLOW  "\033[38;5;3m"
#define BG_BLUE   "\033[44m"
#define FG_MAGENTA "\033[35m"
#define FG_BRIGHT_WHITE "\033[97m"
#define BG_GRAY "\033[40m"
#define RED "\033[31m"
#define FG_BLUE "\033[34m"
#define MARGEM "    "

int Menu::menuPrincipal() {
    system("cls");
    preencherTela(BG_BLUE, FG_BRIGHT_WHITE);
    desenharNALATA();
       vector< string> opcoes = {
        "Efetuar Vendas",
        "Gerir Produtos",
        "Gerir Clientes",
        "Relatorios",
        "Sair"
    };
    return mostrarMenu("MENU PRINCIPAL", opcoes);
}

int Menu::menuGerirProdutos() {
    system("cls");
    preencherTela(BG_BLUE, FG_BRIGHT_WHITE);
    desenharNALATA();
    vector< string> opcoes = {
        "Criar Produto",
        "Adicionar Stock",
        "Eliminar Produto",
        "Listar Produto",
        "Voltar ao Menu Principal"
    };
    return mostrarMenu("GERIR PRODUTOS", opcoes);
}

int Menu::menuGerirClientes() {
    system("cls");
    preencherTela(BG_BLUE, FG_BRIGHT_WHITE);
    desenharNALATA();
    vector< string> opcoes = {
        "Criar Cliente",
        "Eliminar Cliente",
        "Listar Cliente",
        "Alterar Dados Cliente",
        "Voltar ao Menu Principal"
    };
    return mostrarMenu("GERIR CLIENTES", opcoes);
}

int Menu::menuRelatorios() {
    system("cls");
    preencherTela(BG_BLUE, FG_BRIGHT_WHITE);
    desenharNALATA();
    vector< string> opcoes = {
        "Relatorio de Stock",
        "Vendas por Produto",
        "Total de Vendas",
        "Grafico de Vendas",
        "Historico de Vendas",
        "Vendas por Produto Detalhado",
        "Voltar ao Menu Principal"
    };
    return mostrarMenu("RELATORIOS", opcoes);
}

