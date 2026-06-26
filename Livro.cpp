#include "Livro.h"
#include <iostream>

Livro::Livro(std::string isbn, std::string titulo, std::string autor) {
    this->isbn = isbn;
    this->titulo = titulo;
    this->autor = autor;
    disponivel = true;
}

std::string Livro::getIsbn() {
    return isbn;
}

void Livro::setIsbn(std::string isbn) {
    this->isbn = isbn;
}

std::string Livro::getTitulo() {
    return titulo;
}

void Livro::setTitulo(std::string titulo) {
    this->titulo = titulo;
}

std::string Livro::getAutor() {
    return autor;
}

void Livro::setAutor(std::string autor) {
    this->autor = autor;
}

bool Livro::isDisponivel() {
    return disponivel;
}

void Livro::setDisponivel(bool disponivel) {
    this->disponivel = disponivel;
}

void Livro::emprestar() {
    if (disponivel) {
        disponivel = false;
        std::cout << "Livro \"" << titulo << "\" emprestado.\n";
    } else {
        std::cout << "Livro já está emprestado.\n";
    }
}

void Livro::devolver() {
    if (!disponivel) {
        disponivel = true;
        std::cout << "Livro \"" << titulo << "\" devolvido.\n";
    } else {
        std::cout << "Livro já está disponível.\n";
    }
}