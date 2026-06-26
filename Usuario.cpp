#include "Usuario.h"
#include "Livro.h"
#include "Emprestimo.h"
#include <iostream>

Usuario::Usuario(std::string nome, std::string cpf, int matricula)
    : Pessoa(nome, cpf) {
    this->matricula = matricula;
}

int Usuario::getMatricula() {
    return matricula;
}

void Usuario::setMatricula(int matricula) {
    this->matricula = matricula;
}

void Usuario::adicionarEmprestimo(Emprestimo* emp) {
    historico.push_back(emp);
}

void Usuario::consultarLivros(std::vector<Livro*>& acervo) {
    std::cout << "\n--- ACERVO ---\n";
    for (size_t i = 0; i < acervo.size(); i++) {
        Livro* l = acervo[i];
        std::cout << "Título: " << l->getTitulo()
                  << ", Autor: " << l->getAutor()
                  << ", ISBN: " << l->getIsbn()
                  << ", Disponível: " << (l->isDisponivel() ? "Sim" : "Não") << "\n";
    }
}

void Usuario::exibir() {
    Pessoa::exibir();
    std::cout << ", Matrícula: " << matricula;
}