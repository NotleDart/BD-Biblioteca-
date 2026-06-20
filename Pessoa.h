#ifndef PESSOA_H
#define PESSOA_H

#include <string>

class Pessoa {
private:
    std::string nome;
    std::string cpf;

public:
    Pessoa(std::string nome, std::string cpf);
    virtual ~Pessoa();
    std::string getNome();
    void setNome(std::string nome);
    std::string getCpf();
    void setCpf(std::string cpf);

    virtual void exibir();  
};

#endif