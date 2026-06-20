#ifndef USUARIO_H
#define USUARIO_H

#include "Pessoa.h"
#include <vector>

class Livro;
class Emprestimo;

class Usuario : public Pessoa {
private:
    int matricula;
    std::vector<Emprestimo*> historico;

public:
    Usuario(std::string nome, std::string cpf, int matricula);
    int getMatricula();
    void setMatricula(int matricula);

    void adicionarEmprestimo(Emprestimo* emp);
    void consultarLivros(std::vector<Livro*>& acervo);

    void exibir(); 
};

#endif