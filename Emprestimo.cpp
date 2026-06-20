#include "Emprestimo.h"
#include "Usuario.h"
#include "Livro.h"
#include <iostream>

Emprestimo::Emprestimo(std::string dataEmp, std::string dataPrev,
                       Usuario* usuario, Livro* livro) {
    this->dataEmprestimo = dataEmp;
    this->dataPrevista = dataPrev;
    this->usuario = usuario;
    this->livro = livro;
}

std::string Emprestimo::getDataEmprestimo() {
    return dataEmprestimo;
}

void Emprestimo::setDataEmprestimo(std::string data) {
    dataEmprestimo = data;
}

std::string Emprestimo::getDataPrevista() {
    return dataPrevista;
}

void Emprestimo::setDataPrevista(std::string data) {
    dataPrevista = data;
}

Usuario* Emprestimo::getUsuario() {
    return usuario;
}

void Emprestimo::setUsuario(Usuario* usuario) {
    this->usuario = usuario;
}

Livro* Emprestimo::getLivro() {
    return livro;
}

void Emprestimo::setLivro(Livro* livro) {
    this->livro = livro;
}

void Emprestimo::registrar() {
    std::cout << "Empréstimo: " << (usuario ? usuario->getNome() : "N/A")
              << " - Livro: " << (livro ? livro->getTitulo() : "N/A")
              << " - Data: " << dataEmprestimo
              << " - Devolução: " << dataPrevista << "\n";
}