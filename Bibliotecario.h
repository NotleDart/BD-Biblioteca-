#ifndef BIBLIOTECARIO_H
#define BIBLIOTECARIO_H

#include "Pessoa.h"
#include <vector>

class Livro;
class Usuario;
class Emprestimo;

class Bibliotecario : public Pessoa {
private:
    int codigo;

public:
    Bibliotecario(std::string nome, std::string cpf, int codigo);
    int getCodigo();
    void setCodigo(int codigo);

    void cadastrarLivro(std::vector<Livro*>& acervo, Livro* livro);
    void realizarEmprestimo(std::vector<Emprestimo*>& emprestimos,
                            Usuario* usuario,
                            Livro* livro,
                            std::string dataEmp,
                            std::string dataPrev);

    void exibir();
};

#endif