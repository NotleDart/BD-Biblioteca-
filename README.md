Sistema de Biblioteca

Projeto simples em C++ para gerenciar livros, usuários e empréstimos, desenvolvido para a disciplina de Programação Orientada a Objetos.

Como compilar e executar:
Compilar (no terminal, dentro da pasta do projeto):
    make

Executar:
    ./biblioteca


O que o programa faz?
+Cadastrar livros
+Cadastrar usuários
+Realizar empréstimos
+Devolver livros
+Consultar acervo (ver todos os livros e se estão disponíveis)
+Ver empréstimos de um usuário
Tudo isso através de um menu interativo no terminal.

Arquivos do projeto
Pessoa.h / Pessoa.cpp – classe base
Usuario.h / Usuario.cpp – herda de Pessoa
Bibliotecario.h / Bibliotecario.cpp – herda de Pessoa
Livro.h / Livro.cpp – classe de livro
Emprestimo.h / Emprestimo.cpp – liga usuário e livro
main.cpp – menu principal

Conceitos usados
Encapsulamento (atributos privados, getters/setters)
Herança (Usuario e Bibliotecario herdam de Pessoa)
Polimorfismo (método exibir() sobrescrito)
Associação (Emprestimo relaciona Usuario e Livro)
Composição (Usuario guarda seu histórico de empréstimos)

Observações
Os dados ficam apenas na memória (não salvos em arquivo).

Código simples, focado em aprendizado.

Disciplina: Programação Orientada a Objetos# Bibliotecasimples
