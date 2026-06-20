#ifndef LIVRO_H
#define LIVRO_H

#include <string>

class Livro {
private:
    std::string isbn;
    std::string titulo;
    std::string autor;
    bool disponivel;

public:
    Livro(std::string isbn, std::string titulo, std::string autor);
    std::string getIsbn();
    void setIsbn(std::string isbn);
    std::string getTitulo();
    void setTitulo(std::string titulo);
    std::string getAutor();
    void setAutor(std::string autor);
    bool isDisponivel();
    void setDisponivel(bool disponivel);

    void emprestar();
    void devolver();
};

#endif