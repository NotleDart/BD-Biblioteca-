#include <iostream>
#include <vector>
#include <string>
#include <sqlite3.h>
#include "Usuario.h"
#include "Bibliotecario.h"
#include "Livro.h"
#include "Emprestimo.h"
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;


Usuario* buscarUsuario(vector<Usuario*>& usuarios, int mat) {
    for (size_t i = 0; i < usuarios.size(); i++)
        if (usuarios[i]->getMatricula() == mat)
            return usuarios[i];
    return NULL;
}

Livro* buscarLivro(vector<Livro*>& acervo, string isbn) {
    for (size_t i = 0; i < acervo.size(); i++)
        if (acervo[i]->getIsbn() == isbn)
            return acervo[i];
    return NULL;
}

Bibliotecario* buscarBibliotecario(vector<Bibliotecario*>& bibs, int codigo) {
    for (size_t i = 0; i < bibs.size(); i++)
        if (bibs[i]->getCodigo() == codigo)
            return bibs[i];
    return NULL;
}

string calcularDataPrevista(string dataEmp) {
    tm data = {};
    istringstream ss(dataEmp);
    ss >> get_time(&data, "%Y/%m/%d");
    time_t tempo = mktime(&data);
    tempo += 7 * 24 * 60 * 60;
    tm* novaData = localtime(&tempo);
    ostringstream saida;
    saida << put_time(novaData, "%Y/%m/%d");
    return saida.str();
}


void criarTabelas(sqlite3* db) {
    char* errMsg = 0;

    sqlite3_exec(db,
        "CREATE TABLE IF NOT EXISTS livros ("
        "isbn TEXT PRIMARY KEY,"
        "titulo TEXT NOT NULL,"
        "autor TEXT NOT NULL,"
        "disponivel INTEGER NOT NULL DEFAULT 1);",
        0, 0, &errMsg);

    sqlite3_exec(db,
        "CREATE TABLE IF NOT EXISTS usuarios ("
        "matricula INTEGER PRIMARY KEY,"
        "nome TEXT NOT NULL,"
        "cpf TEXT NOT NULL);",
        0, 0, &errMsg);

    sqlite3_exec(db,
        "CREATE TABLE IF NOT EXISTS bibliotecarios ("
        "codigo INTEGER PRIMARY KEY,"
        "nome TEXT NOT NULL,"
        "cpf TEXT NOT NULL);",
        0, 0, &errMsg);

    sqlite3_exec(db,
        "CREATE TABLE IF NOT EXISTS emprestimos ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "matricula INTEGER NOT NULL,"
        "isbn TEXT NOT NULL,"
        "dataEmprestimo TEXT NOT NULL,"
        "dataPrevista TEXT NOT NULL,"
        "dataDevolucao TEXT,"
        "devolvido INTEGER DEFAULT 0);",
        0, 0, &errMsg);
}


void carregarDados(sqlite3* db,
                   vector<Livro*>& acervo,
                   vector<Usuario*>& usuarios,
                   vector<Bibliotecario*>& bibliotecarios) {
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, "SELECT isbn, titulo, autor, disponivel FROM livros;", -1, &stmt, 0);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string isbn  = (char*)sqlite3_column_text(stmt, 0);
        string titulo = (char*)sqlite3_column_text(stmt, 1);
        string autor  = (char*)sqlite3_column_text(stmt, 2);
        int    disp   = sqlite3_column_int(stmt, 3);
        Livro* l = new Livro(isbn, titulo, autor);
        l->setDisponivel(disp == 1);
        acervo.push_back(l);
    }
    sqlite3_finalize(stmt);

    sqlite3_prepare_v2(db, "SELECT matricula, nome, cpf FROM usuarios;", -1, &stmt, 0);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int    mat  = sqlite3_column_int(stmt, 0);
        string nome = (char*)sqlite3_column_text(stmt, 1);
        string cpf  = (char*)sqlite3_column_text(stmt, 2);
        usuarios.push_back(new Usuario(nome, cpf, mat));
    }
    sqlite3_finalize(stmt);

    sqlite3_prepare_v2(db, "SELECT codigo, nome, cpf FROM bibliotecarios;", -1, &stmt, 0);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int    cod  = sqlite3_column_int(stmt, 0);
        string nome = (char*)sqlite3_column_text(stmt, 1);
        string cpf  = (char*)sqlite3_column_text(stmt, 2);
        bibliotecarios.push_back(new Bibliotecario(nome, cpf, cod));
    }
    sqlite3_finalize(stmt);
}


void menuLivro(sqlite3* db, vector<Livro*>& acervo) {
    int op;
    do {
        cout << "\n=== CRUD LIVRO ===\n";
        cout << "1. Cadastrar livro\n";
        cout << "2. Listar livros\n";
        cout << "3. Atualizar livro\n";
        cout << "4. Remover livro\n";
        cout << "0. Voltar\n";
        cout << "Opcao: ";
        cin >> op; cin.ignore();

        if (op == 1) {
            string isbn, titulo, autor;
            cout << "ISBN: ";   getline(cin, isbn);
            cout << "Titulo: "; getline(cin, titulo);
            cout << "Autor: ";  getline(cin, autor);

            string sql = "INSERT INTO livros (isbn, titulo, autor, disponivel) VALUES ('"
                         + isbn + "','" + titulo + "','" + autor + "', 1);";
            char* err = 0;
            if (sqlite3_exec(db, sql.c_str(), 0, 0, &err) != SQLITE_OK) {
                cout << "Erro: " << err << "\n";
                sqlite3_free(err);
            } else {
                acervo.push_back(new Livro(isbn, titulo, autor));
                cout << "Livro cadastrado!\n";
            }

        } else if (op == 2) {
            cout << "\n--- Livros ---\n";
            for (size_t i = 0; i < acervo.size(); i++) {
                Livro* l = acervo[i];
                cout << "ISBN: " << l->getIsbn()
                     << " | Titulo: " << l->getTitulo()
                     << " | Autor: " << l->getAutor()
                     << " | Disponivel: " << (l->isDisponivel() ? "Sim" : "Nao") << "\n";
            }
            if (acervo.empty()) cout << "Nenhum livro cadastrado.\n";

        } else if (op == 3) {
            string isbn, titulo, autor;
            cout << "ISBN do livro a atualizar: "; getline(cin, isbn);
            Livro* l = buscarLivro(acervo, isbn);
            if (!l) { cout << "Livro nao encontrado.\n"; continue; }
            cout << "Novo titulo (atual: " << l->getTitulo() << "): "; getline(cin, titulo);
            cout << "Novo autor  (atual: " << l->getAutor()  << "): "; getline(cin, autor);
            if (titulo.empty()) titulo = l->getTitulo();
            if (autor.empty())  autor  = l->getAutor();

            string sql = "UPDATE livros SET titulo='" + titulo + "', autor='" + autor
                         + "' WHERE isbn='" + isbn + "';";
            char* err = 0;
            if (sqlite3_exec(db, sql.c_str(), 0, 0, &err) != SQLITE_OK) {
                cout << "Erro: " << err << "\n"; sqlite3_free(err);
            } else {
                l->setTitulo(titulo); l->setAutor(autor);
                cout << "Livro atualizado!\n";
            }

        } else if (op == 4) {
            string isbn;
            cout << "ISBN do livro a remover: "; getline(cin, isbn);
            Livro* l = buscarLivro(acervo, isbn);
            if (!l) { cout << "Livro nao encontrado.\n"; continue; }

            string sql = "DELETE FROM livros WHERE isbn='" + isbn + "';";
            char* err = 0;
            if (sqlite3_exec(db, sql.c_str(), 0, 0, &err) != SQLITE_OK) {
                cout << "Erro: " << err << "\n"; sqlite3_free(err);
            } else {
                for (size_t i = 0; i < acervo.size(); i++) {
                    if (acervo[i]->getIsbn() == isbn) {
                        delete acervo[i];
                        acervo.erase(acervo.begin() + i);
                        break;
                    }
                }
                cout << "Livro removido!\n";
            }
        }
    } while (op != 0);
}

void menuUsuario(sqlite3* db, vector<Usuario*>& usuarios) {
    int op;
    do {
        cout << "\n=== CRUD USUARIO ===\n";
        cout << "1. Cadastrar usuario\n";
        cout << "2. Listar usuarios\n";
        cout << "3. Atualizar usuario\n";
        cout << "4. Remover usuario\n";
        cout << "0. Voltar\n";
        cout << "Opcao: ";
        cin >> op; cin.ignore();

        if (op == 1) {
            string nome, cpf;
            int mat;
            cout << "Matricula: "; cin >> mat; cin.ignore();
            if (buscarUsuario(usuarios, mat)) { cout << "Matricula ja existe.\n"; continue; }
            cout << "Nome: "; getline(cin, nome);
            cout << "CPF: ";  getline(cin, cpf);

            string sql = "INSERT INTO usuarios (matricula, nome, cpf) VALUES ("
                         + to_string(mat) + ",'" + nome + "','" + cpf + "');";
            char* err = 0;
            if (sqlite3_exec(db, sql.c_str(), 0, 0, &err) != SQLITE_OK) {
                cout << "Erro: " << err << "\n"; sqlite3_free(err);
            } else {
                usuarios.push_back(new Usuario(nome, cpf, mat));
                cout << "Usuario cadastrado!\n";
            }

        } else if (op == 2) {
            cout << "\n--- Usuarios ---\n";
            for (size_t i = 0; i < usuarios.size(); i++) {
                Usuario* u = usuarios[i];
                cout << "Matricula: " << u->getMatricula()
                     << " | Nome: " << u->getNome()
                     << " | CPF: " << u->getCpf() << "\n";
            }
            if (usuarios.empty()) cout << "Nenhum usuario cadastrado.\n";

        } else if (op == 3) {
            int mat;
            cout << "Matricula do usuario a atualizar: "; cin >> mat; cin.ignore();
            Usuario* u = buscarUsuario(usuarios, mat);
            if (!u) { cout << "Usuario nao encontrado.\n"; continue; }
            string nome, cpf;
            cout << "Novo nome (atual: " << u->getNome() << "): "; getline(cin, nome);
            cout << "Novo CPF  (atual: " << u->getCpf()  << "): "; getline(cin, cpf);
            if (nome.empty()) nome = u->getNome();
            if (cpf.empty())  cpf  = u->getCpf();

            string sql = "UPDATE usuarios SET nome='" + nome + "', cpf='" + cpf
                         + "' WHERE matricula=" + to_string(mat) + ";";
            char* err = 0;
            if (sqlite3_exec(db, sql.c_str(), 0, 0, &err) != SQLITE_OK) {
                cout << "Erro: " << err << "\n"; sqlite3_free(err);
            } else {
                u->setNome(nome); u->setCpf(cpf);
                cout << "Usuario atualizado!\n";
            }

        } else if (op == 4) {
            int mat;
            cout << "Matricula do usuario a remover: "; cin >> mat; cin.ignore();
            Usuario* u = buscarUsuario(usuarios, mat);
            if (!u) { cout << "Usuario nao encontrado.\n"; continue; }

            string sql = "DELETE FROM usuarios WHERE matricula=" + to_string(mat) + ";";
            char* err = 0;
            if (sqlite3_exec(db, sql.c_str(), 0, 0, &err) != SQLITE_OK) {
                cout << "Erro: " << err << "\n"; sqlite3_free(err);
            } else {
                for (size_t i = 0; i < usuarios.size(); i++) {
                    if (usuarios[i]->getMatricula() == mat) {
                        delete usuarios[i];
                        usuarios.erase(usuarios.begin() + i);
                        break;
                    }
                }
                cout << "Usuario removido!\n";
            }
        }
    } while (op != 0);
}


void menuBibliotecario(sqlite3* db, vector<Bibliotecario*>& bibs) {
    int op;
    do {
        cout << "\n=== CRUD BIBLIOTECARIO ===\n";
        cout << "1. Cadastrar bibliotecario\n";
        cout << "2. Listar bibliotecarios\n";
        cout << "3. Atualizar bibliotecario\n";
        cout << "4. Remover bibliotecario\n";
        cout << "0. Voltar\n";
        cout << "Opcao: ";
        cin >> op; cin.ignore();

        if (op == 1) {
            string nome, cpf;
            int cod;
            cout << "Codigo: "; cin >> cod; cin.ignore();
            if (buscarBibliotecario(bibs, cod)) { cout << "Codigo ja existe.\n"; continue; }
            cout << "Nome: "; getline(cin, nome);
            cout << "CPF: ";  getline(cin, cpf);

            string sql = "INSERT INTO bibliotecarios (codigo, nome, cpf) VALUES ("
                         + to_string(cod) + ",'" + nome + "','" + cpf + "');";
            char* err = 0;
            if (sqlite3_exec(db, sql.c_str(), 0, 0, &err) != SQLITE_OK) {
                cout << "Erro: " << err << "\n"; sqlite3_free(err);
            } else {
                bibs.push_back(new Bibliotecario(nome, cpf, cod));
                cout << "Bibliotecario cadastrado!\n";
            }

        } else if (op == 2) {
            cout << "\n--- Bibliotecarios ---\n";
            for (size_t i = 0; i < bibs.size(); i++) {
                Bibliotecario* b = bibs[i];
                cout << "Codigo: " << b->getCodigo()
                     << " | Nome: " << b->getNome()
                     << " | CPF: " << b->getCpf() << "\n";
            }
            if (bibs.empty()) cout << "Nenhum bibliotecario cadastrado.\n";

        } else if (op == 3) {
            int cod;
            cout << "Codigo do bibliotecario a atualizar: "; cin >> cod; cin.ignore();
            Bibliotecario* b = buscarBibliotecario(bibs, cod);
            if (!b) { cout << "Nao encontrado.\n"; continue; }
            string nome, cpf;
            cout << "Novo nome (atual: " << b->getNome() << "): "; getline(cin, nome);
            cout << "Novo CPF  (atual: " << b->getCpf()  << "): "; getline(cin, cpf);
            if (nome.empty()) nome = b->getNome();
            if (cpf.empty())  cpf  = b->getCpf();

            string sql = "UPDATE bibliotecarios SET nome='" + nome + "', cpf='" + cpf
                         + "' WHERE codigo=" + to_string(cod) + ";";
            char* err = 0;
            if (sqlite3_exec(db, sql.c_str(), 0, 0, &err) != SQLITE_OK) {
                cout << "Erro: " << err << "\n"; sqlite3_free(err);
            } else {
                b->setNome(nome); b->setCpf(cpf);
                cout << "Bibliotecario atualizado!\n";
            }

        } else if (op == 4) {
            int cod;
            cout << "Codigo do bibliotecario a remover: "; cin >> cod; cin.ignore();
            Bibliotecario* b = buscarBibliotecario(bibs, cod);
            if (!b) { cout << "Nao encontrado.\n"; continue; }

            string sql = "DELETE FROM bibliotecarios WHERE codigo=" + to_string(cod) + ";";
            char* err = 0;
            if (sqlite3_exec(db, sql.c_str(), 0, 0, &err) != SQLITE_OK) {
                cout << "Erro: " << err << "\n"; sqlite3_free(err);
            } else {
                for (size_t i = 0; i < bibs.size(); i++) {
                    if (bibs[i]->getCodigo() == cod) {
                        delete bibs[i];
                        bibs.erase(bibs.begin() + i);
                        break;
                    }
                }
                cout << "Bibliotecario removido!\n";
            }
        }
    } while (op != 0);
}

void menuEmprestimo(sqlite3* db,
                    vector<Emprestimo*>& emprestimos,
                    vector<Usuario*>& usuarios,
                    vector<Livro*>& acervo) {
    int op;
    do {
        cout << "\n=== CRUD EMPRESTIMO ===\n";
        cout << "1. Realizar emprestimo\n";
        cout << "2. Listar emprestimos\n";
        cout << "3. Devolver livro\n";
        cout << "4. Remover emprestimo (por ID)\n";
        cout << "0. Voltar\n";
        cout << "Opcao: ";
        cin >> op; cin.ignore();

        if (op == 1) {
            int mat;
            string isbn, dataEmp;
            cout << "Matricula do usuario: "; cin >> mat; cin.ignore();
            Usuario* u = buscarUsuario(usuarios, mat);
            if (!u) { cout << "Usuario nao encontrado.\n"; continue; }
            cout << "ISBN do livro: "; getline(cin, isbn);
            Livro* l = buscarLivro(acervo, isbn);
            if (!l) { cout << "Livro nao encontrado.\n"; continue; }
            if (!l->isDisponivel()) { cout << "Livro indisponivel.\n"; continue; }
            cout << "Data do emprestimo (aaaa/mm/dd): "; getline(cin, dataEmp);
            string dataPrev = calcularDataPrevista(dataEmp);
            cout << "Data prevista de devolucao: " << dataPrev << "\n";

            char* err = 0;
            string sqlUpd = "UPDATE livros SET disponivel=0 WHERE isbn='" + isbn + "';";
            sqlite3_exec(db, sqlUpd.c_str(), 0, 0, &err);

            string sqlIns = "INSERT INTO emprestimos (matricula, isbn, dataEmprestimo, dataPrevista, dataDevolucao, devolvido)"
                            " VALUES (" + to_string(mat) + ",'" + isbn + "','" + dataEmp + "','" + dataPrev + "', NULL, 0);";
            if (sqlite3_exec(db, sqlIns.c_str(), 0, 0, &err) != SQLITE_OK) {
                cout << "Erro ao registrar emprestimo: " << err << "\n"; sqlite3_free(err);
            } else {
                l->setDisponivel(false);
                Emprestimo* emp = new Emprestimo(dataEmp, dataPrev, u, l);
                emprestimos.push_back(emp);
                u->adicionarEmprestimo(emp);
                cout << "Emprestimo realizado!\n";
            }

        } else if (op == 2) {
            cout << "\n--- Emprestimos ---\n";
            sqlite3_stmt* stmt;
            sqlite3_prepare_v2(db,
                "SELECT id, matricula, isbn, dataEmprestimo, dataPrevista, dataDevolucao, devolvido"
                " FROM emprestimos;", -1, &stmt, 0);
            bool tem = false;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                tem = true;
                int    id       = sqlite3_column_int(stmt, 0);
                int    mat      = sqlite3_column_int(stmt, 1);
                string isbn     = (char*)sqlite3_column_text(stmt, 2);
                string dataEmp  = (char*)sqlite3_column_text(stmt, 3);
                string dataPrev = (char*)sqlite3_column_text(stmt, 4);
                const unsigned char* dataDev = sqlite3_column_text(stmt, 5);
                int    dev      = sqlite3_column_int(stmt, 6);
                cout << "ID: " << id
                     << " | Matricula: " << mat
                     << " | ISBN: " << isbn
                     << " | Emprestimo: " << dataEmp
                     << " | Prevista: " << dataPrev
                     << " | Devolvido: " << (dev ? "Sim" : "Nao");
                if (dataDev) cout << " | Devolucao: " << dataDev;
                cout << "\n";
            }
            sqlite3_finalize(stmt);
            if (!tem) cout << "Nenhum emprestimo.\n";

        } else if (op == 3) {
            string isbn, dataDev;
            cout << "ISBN do livro a devolver: "; getline(cin, isbn);
            cout << "Data da devolucao (aaaa/mm/dd): "; getline(cin, dataDev);
            Livro* l = buscarLivro(acervo, isbn);
            if (!l) { cout << "Livro nao encontrado.\n"; continue; }
            if (l->isDisponivel()) { cout << "Livro ja esta disponivel.\n"; continue; }

            char* err = 0;
            string sqlL = "UPDATE livros SET disponivel=1 WHERE isbn='" + isbn + "';";
            sqlite3_exec(db, sqlL.c_str(), 0, 0, &err);

            string sqlE = "UPDATE emprestimos SET devolvido=1, dataDevolucao='" + dataDev
                          + "' WHERE isbn='" + isbn + "' AND devolvido=0;";
            sqlite3_exec(db, sqlE.c_str(), 0, 0, &err);

            l->setDisponivel(true);
            cout << "Livro devolvido!\n";

        } else if (op == 4) {
            int id;
            cout << "ID do emprestimo a remover: "; cin >> id; cin.ignore();
            string sql = "DELETE FROM emprestimos WHERE id=" + to_string(id) + ";";
            char* err = 0;
            if (sqlite3_exec(db, sql.c_str(), 0, 0, &err) != SQLITE_OK) {
                cout << "Erro: " << err << "\n"; sqlite3_free(err);
            } else {
                cout << "Emprestimo removido!\n";
            }
        }
    } while (op != 0);
}

void livrosPorUsuario(sqlite3* db) {
    int mat;
    cout << "Matricula do usuario: "; cin >> mat; cin.ignore();
    sqlite3_stmt* stmt;
    string sql = "SELECT COUNT(*) FROM emprestimos WHERE matricula=" + to_string(mat) + ";";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    int total = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        total = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    cout << "O usuario " << mat << " fez " << total << " emprestimo(s).\n";
}

void livrosPorAutor(sqlite3* db) {
    string autor;
    cout << "Nome do autor: "; getline(cin, autor);
    sqlite3_stmt* stmt;
    string sql = "SELECT COUNT(*) FROM livros WHERE autor='" + autor + "';";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    int total = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        total = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    cout << "Existem " << total << " livro(s) do autor " << autor << ".\n";
}

void livrosDisponiveis(sqlite3* db) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM livros WHERE disponivel=1;", -1, &stmt, 0);
    int total = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        total = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    cout << "Total de livros disponiveis: " << total << "\n";
}

void emprestimosAtrasados(sqlite3* db) {
    time_t agora = time(0);
    tm* dataAtual = localtime(&agora);
    char hoje[11];
    strftime(hoje, sizeof(hoje), "%Y/%m/%d", dataAtual);
    string dataHoje = hoje;

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "SELECT id, matricula, isbn, dataEmprestimo, dataPrevista FROM emprestimos"
        " WHERE devolvido=0;",
        -1, &stmt, 0);

    cout << "\n--- EMPRESTIMOS ATRASADOS (hoje: " << dataHoje << ") ---\n";
    bool achou = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string dataPrev = (char*)sqlite3_column_text(stmt, 4);
        if (dataHoje > dataPrev) {
            achou = true;
            cout << "ID: "         << sqlite3_column_int(stmt, 0)
                 << " | Matricula: " << sqlite3_column_int(stmt, 1)
                 << " | ISBN: "    << (char*)sqlite3_column_text(stmt, 2)
                 << " | Emprestimo: " << (char*)sqlite3_column_text(stmt, 3)
                 << " | Prevista: "  << dataPrev << "\n";
        }
    }
    sqlite3_finalize(stmt);
    if (!achou) cout << "Nenhum emprestimo atrasado.\n";
}

int main() {
    sqlite3* db;
    if (sqlite3_open("biblioteca.db", &db)) {
        cout << "Erro ao abrir banco: " << sqlite3_errmsg(db) << "\n";
        return 1;
    }
    cout << "Banco de dados conectado!\n";

    criarTabelas(db);

    vector<Livro*>        acervo;
    vector<Usuario*>      usuarios;
    vector<Bibliotecario*> bibliotecarios;
    vector<Emprestimo*>   emprestimos;

    carregarDados(db, acervo, usuarios, bibliotecarios);

    int op;
    do {
        cout << "\n========== SISTEMA DE BIBLIOTECA ==========\n";
        cout << "1.  CRUD Bibliotecario\n";
        cout << "2.  CRUD Usuario\n";
        cout << "3.  CRUD Livro\n";
        cout << "4.  CRUD Emprestimo\n";
        cout << "-------------------------------------------\n";
        cout << "5.  Quantos livros um usuario pegou emprestado\n";
        cout << "6.  Quantos livros de um determinado autor\n";
        cout << "7.  Quantos livros disponiveis\n";
        cout << "8.  Livros com entrega atrasada\n";
        cout << "-------------------------------------------\n";
        cout << "0.  Sair\n";
        cout << "Opcao: ";
        cin >> op; cin.ignore();

        switch (op) {
            case 1: menuBibliotecario(db, bibliotecarios);                    break;
            case 2: menuUsuario(db, usuarios);                                break;
            case 3: menuLivro(db, acervo);                                    break;
            case 4: menuEmprestimo(db, emprestimos, usuarios, acervo);        break;
            case 5: livrosPorUsuario(db);                                     break;
            case 6: livrosPorAutor(db);                                       break;
            case 7: livrosDisponiveis(db);                                    break;
            case 8: emprestimosAtrasados(db);                                 break;
            case 0: cout << "Saindo...\n";                                    break;
            default: cout << "Opcao invalida.\n";
        }
    } while (op != 0);

    for (size_t i = 0; i < emprestimos.size();    i++) delete emprestimos[i];
    for (size_t i = 0; i < acervo.size();          i++) delete acervo[i];
    for (size_t i = 0; i < usuarios.size();        i++) delete usuarios[i];
    for (size_t i = 0; i < bibliotecarios.size(); i++) delete bibliotecarios[i];

    sqlite3_close(db);
    cout << "Banco fechado.\n";
    return 0;
}
