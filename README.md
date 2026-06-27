## Sistema de Biblioteca

Projeto simples em C++ para gerenciar livros, usuários e empréstimos, desenvolvido para a disciplina de Programação Orientada a Objetos.

## Como compilar e executar:
Compilar (no terminal, dentro da pasta do projeto):
    g++ -o biblioteca *.cpp -lsqlite3

Executar:
    ./biblioteca


## O que o programa faz?
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

## Conceitos usados
Encapsulamento (atributos privados, getters/setters)
Herança (Usuario e Bibliotecario herdam de Pessoa)
Polimorfismo (método exibir() sobrescrito)
Associação (Emprestimo relaciona Usuario e Livro)
Composição (Usuario guarda seu histórico de empréstimos)

## Requisitos implementados

### Estrutura do projeto
- [x] Classes separadas em arquivos .h e .cpp
- [x] Construtores
- [x] Getters e Setters
- [x] Uso de std::vector
- [x] Menu interativo em terminal
- [x] Integração com SQLite3

### Funcionalidades

- [x] Cadastro de usuários
- [x] Cadastro de livros
- [x] Registro de empréstimos
- [x] Devolução de livros
- [x] Consulta de livros disponíveis
- [x] Consulta de livros por autor
- [x] Consulta da quantidade de livros emprestados por usuário
- [x] Consulta de empréstimos atrasados

### Funcionalidades pendentes

- [ ] CRUD completo de Bibliotecário
- [ ] Edição e exclusão de Usuário
- [ ] Edição e exclusão de Livro
- [ ] CRUD completo de Empréstimo

## Como executar com  a adição do banco de dados no código:

1. Instale a biblioteca SQLite3.
 - Se for no Windows:
 Acesse https://www.sqlite.org/download.html
  Baixe os pacotes:
sqlite-tools
sqlite-dll
sqlite-amalgamation
- Se for no Linux: Instale o SQLite3 nativo: Abra o terminal e digite sudo apt install sqlite3 (em distros baseadas em Debian/Ubuntu) e instale o DB Browser: Digite sudo apt install sqlitebrowser;
- Outras distros: sudo dnf install sqlite (Fedora) e depois sudo dnf install sqlite-devel ou sudo pacman -S sqlite (Arch);
- Se for no Mac o SQLite normalmente já vem instalado (verifique usando:sqlite3 --version).
   
3. Compile o projeto.
4. Execute o programa.
5. O banco de dados `biblioteca.db` será criado automaticamente caso não exista.
4. Explicar a estrutura
   
## Estrutura

- Pessoa
- Usuario
- Bibliotecario
- Livro
- Emprestimo
- Biblioteca

## Observações
Os dados são persistidos em um banco de dados SQLite3 (biblioteca.db). Além do armazenamento em memória com std::vector, as operações também utilizam o banco de dados para persistência das informações.

Código simples, focado em aprendizado.

Disciplina: Programação Orientada a Objetos# Bibliotecasimples
