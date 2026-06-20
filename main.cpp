#include <iostream>
#include <vector>
#include <string>
#include "Usuario.h"
#include "Bibliotecario.h"
#include "Livro.h"
#include "Emprestimo.h"

using namespace std;

Usuario* buscarUsuario(vector<Usuario*>& usuarios, int mat) {
    for (size_t i = 0; i < usuarios.size(); i++) {
        if (usuarios[i]->getMatricula() == mat)
            return usuarios[i];
    }
    return NULL;
}

Livro* buscarLivro(vector<Livro*>& acervo, string isbn) {
    for (size_t i = 0; i < acervo.size(); i++) {
        if (acervo[i]->getIsbn() == isbn)
            return acervo[i];
    }
    return NULL;
}

int main() {
    vector<Livro*> acervo;
    vector<Usuario*> usuarios;
    vector<Emprestimo*> emprestimos;

    Bibliotecario* bib = new Bibliotecario("Carlos", "123.456.789-00", 1001);

    int op;
    do {
        cout << "\n--- SISTEMA DE BIBLIOTECA ---\n";
        cout << "1. Cadastrar livro\n";
        cout << "2. Cadastrar usuário\n";
        cout << "3. Realizar empréstimo\n";
        cout << "4. Devolver livro\n";
        cout << "5. Consultar acervo\n";
        cout << "6. Ver empréstimos de um usuário\n";
        cout << "7. Sair\n";
        cout << "Opção: ";
        cin >> op;
        cin.ignore();

        switch (op) {
            case 1: {
                string isbn, titulo, autor;
                cout << "ISBN: "; getline(cin, isbn);
                cout << "Título: "; getline(cin, titulo);
                cout << "Autor: "; getline(cin, autor);
                Livro* l = new Livro(isbn, titulo, autor);
                bib->cadastrarLivro(acervo, l);
                break;
            }
            case 2: {
                string nome, cpf;
                int mat;
                cout << "Nome: "; getline(cin, nome);
                cout << "CPF: "; getline(cin, cpf);
                cout << "Matrícula: "; cin >> mat; cin.ignore();
                usuarios.push_back(new Usuario(nome, cpf, mat));
                cout << "Usuário cadastrado.\n";
                break;
            }
            case 3: {
                int mat;
                string isbn, dataEmp, dataPrev;
                cout << "Matrícula do usuário: "; cin >> mat; cin.ignore();
                Usuario* u = buscarUsuario(usuarios, mat);
                if (!u) {
                    cout << "Usuário não encontrado.\n";
                    break;
                }
                cout << "ISBN do livro: "; getline(cin, isbn);
                Livro* l = buscarLivro(acervo, isbn);
                if (!l) {
                    cout << "Livro não encontrado.\n";
                    break;
                }
                cout << "Data empréstimo (dd/mm): "; getline(cin, dataEmp);
                cout << "Data devolução prevista (dd/mm): "; getline(cin, dataPrev);
                bib->realizarEmprestimo(emprestimos, u, l, dataEmp, dataPrev);
                break;
            }
            case 4: {
                string isbn;
                cout << "ISBN do livro: "; getline(cin, isbn);
                Livro* l = buscarLivro(acervo, isbn);
                if (l) {
                    l->devolver();
                } else {
                    cout << "Livro não encontrado.\n";
                }
                break;
            }
            case 5: {
                if (usuarios.empty()) {
                    cout << "Nenhum usuário cadastrado para consultar.\n";
                } else {
                    usuarios[0]->consultarLivros(acervo);
                }
                break;
            }
            case 6: {
                int mat;
                cout << "Matrícula: "; cin >> mat; cin.ignore();
                Usuario* u = buscarUsuario(usuarios, mat);
                if (!u) {
                    cout << "Usuário não encontrado.\n";
                    break;
                }
                cout << "Empréstimos de " << u->getNome() << ":\n";
                bool tem = false;
                for (size_t i = 0; i < emprestimos.size(); i++) {
                    Emprestimo* e = emprestimos[i];
                    if (e->getUsuario() == u) {
                        e->registrar();
                        tem = true;
                    }
                }
                if (!tem) cout << "Nenhum empréstimo ativo.\n";
                break;
            }
            case 7:
                cout << "Saindo...\n";
                break;
            default:
                cout << "Opção inválida.\n";
        }
    } while (op != 7);

    // Libera memória
    for (size_t i = 0; i < emprestimos.size(); i++) delete emprestimos[i];
    for (size_t i = 0; i < acervo.size(); i++) delete acervo[i];
    for (size_t i = 0; i < usuarios.size(); i++) delete usuarios[i];
    delete bib;

    return 0;
}