#include "Menu.h"
#include "Auxiliar.h"
#include <iostream>
#include <iomanip>
#include <windows.h> 

using namespace std;

#define RESET   "\033[0m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define BOLD    "\033[1m"
#define YELLOW  "\033[33m"


int Menu::menuPrincipal() {
    system("cls");
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

