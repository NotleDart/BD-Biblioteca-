#include "Pessoa.h"
#include <iostream>

Pessoa::Pessoa(std::string nome, std::string cpf) {
    this->nome = nome;
    this->cpf = cpf;
}

std::string Pessoa::getNome() {
    return nome;
}

void Pessoa::setNome(std::string nome) {
    this->nome = nome;
}

std::string Pessoa::getCpf() {
    return cpf;
}

void Pessoa::setCpf(std::string cpf) {
    this->cpf = cpf;
}

Pessoa::~Pessoa() {
}

void Pessoa::exibir() {
    std::cout << "Nome: " << nome << ", CPF: " << cpf;
}