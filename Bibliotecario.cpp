#include "Bibliotecario.h"
#include "Livro.h"
#include "Usuario.h"
#include "Emprestimo.h"
#include <iostream>

Bibliotecario::Bibliotecario(std::string nome, std::string cpf, int codigo)
    : Pessoa(nome, cpf) {
    this->codigo = codigo;
}

int Bibliotecario::getCodigo() {
    return codigo;
}

void Bibliotecario::setCodigo(int codigo) {
    this->codigo = codigo;
}

void Bibliotecario::cadastrarLivro(std::vector<Livro*>& acervo, Livro* livro) {
    if (livro) {
        acervo.push_back(livro);
        std::cout << "Livro \"" << livro->getTitulo() << "\" cadastrado.\n";
    }
}

void Bibliotecario::realizarEmprestimo(std::vector<Emprestimo*>& emprestimos,
                                       Usuario* usuario,
                                       Livro* livro,
                                       std::string dataEmp,
                                       std::string dataPrev) {
    if (!usuario || !livro) {
        std::cout << "Dados inválidos.\n";
        return;
    }
    if (!livro->isDisponivel()) {
        std::cout << "Livro indisponível.\n";
        return;
    }

    Emprestimo* emp = new Emprestimo(dataEmp, dataPrev, usuario, livro);
    emprestimos.push_back(emp);
    livro->emprestar();
    usuario->adicionarEmprestimo(emp);
    std::cout << "Empréstimo realizado.\n";
}

void Bibliotecario::exibir() {
    Pessoa::exibir();
    std::cout << ", Código: " << codigo;
}