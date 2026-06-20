#ifndef EMPRESTIMO_H
#define EMPRESTIMO_H

#include <string>

class Usuario;
class Livro;

class Emprestimo {
private:
    std::string dataEmprestimo;
    std::string dataPrevista;
    Usuario* usuario;
    Livro* livro;

public:
    Emprestimo(std::string dataEmp, std::string dataPrev, Usuario* usuario, Livro* livro);
    std::string getDataEmprestimo();
    void setDataEmprestimo(std::string data);
    std::string getDataPrevista();
    void setDataPrevista(std::string data);
    Usuario* getUsuario();
    void setUsuario(Usuario* usuario);
    Livro* getLivro();
    void setLivro(Livro* livro);

    void registrar();
};

#endif