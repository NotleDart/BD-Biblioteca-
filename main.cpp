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

string calcularDataPrevista(string dataEmp) {

    tm data = {};

    istringstream ss(dataEmp);

    ss >> get_time(&data, "%Y-%m-%d");

    time_t tempo = mktime(&data);

    tempo += 7 * 24 * 60 * 60;

    tm* novaData = localtime(&tempo);

    ostringstream saida;
    saida << put_time(novaData, "%Y-%m-%d");

    return saida.str(); 
}

int main() {

    sqlite3* db;
    int rc = sqlite3_open("biblioteca.db", &db);
    if (rc) {
        cout << "Erro ao abrir banco: " << sqlite3_errmsg(db) << endl;
        return 1;
    }
    cout << "Banco de dados conectado!\n";

    const char* sqlCreate = "CREATE TABLE IF NOT EXISTS livros ("
                            "isbn TEXT PRIMARY KEY, "
                            "titulo TEXT NOT NULL, "
                            "autor TEXT NOT NULL, "
                            "disponivel INTEGER NOT NULL DEFAULT 1);";
    char* errMsg = 0;
    sqlite3_exec(db, sqlCreate, 0, 0, &errMsg);

    const char* sqlCreateEmprestimos =
    "CREATE TABLE IF NOT EXISTS emprestimos ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "matricula INTEGER NOT NULL,"
        "isbn TEXT NOT NULL,"
        "dataEmprestimo TEXT NOT NULL,"
        "dataPrevista TEXT NOT NULL,"
        "devolvido INTEGER DEFAULT 0"
        ");";
        sqlite3_exec(db, sqlCreateEmprestimos, 0, 0, &errMsg);

    vector<Livro*> acervo;
    vector<Usuario*> usuarios;
    vector<Emprestimo*> emprestimos;

    Bibliotecario* bib = new Bibliotecario("Carlos", "123.456.789-00", 1001);

    string sqlSelect = "SELECT isbn, titulo, autor, disponivel FROM livros;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sqlSelect.c_str(), -1, &stmt, 0);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string isbn = (char*)sqlite3_column_text(stmt, 0);
        string titulo = (char*)sqlite3_column_text(stmt, 1);
        string autor = (char*)sqlite3_column_text(stmt, 2);
        int disp = sqlite3_column_int(stmt, 3);
        Livro* l = new Livro(isbn, titulo, autor);
        l->setDisponivel(disp == 1);
        acervo.push_back(l);
    }
    sqlite3_finalize(stmt);

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
        cout << "8. Quantos livros de um autor?\n";   
        cout << "9. Quantos livros disponíveis?\n";
        cout << "10. Emprestimos atrasados\n";        
        cout << "Opção: ";
        cin >> op;
        cin.ignore();

        switch (op) {
  
            case 1: {
                string isbn, titulo, autor;
                cout << "ISBN: "; getline(cin, isbn);
                cout << "Título: "; getline(cin, titulo);
                cout << "Autor: "; getline(cin, autor);
                
        
                string sqlInsert = "INSERT INTO livros (isbn, titulo, autor, disponivel) VALUES ('" 
                                   + isbn + "', '" + titulo + "', '" + autor + "', 1);";
                rc = sqlite3_exec(db, sqlInsert.c_str(), 0, 0, &errMsg);
                if (rc != SQLITE_OK) {
                    cout << "Erro: " << errMsg << " (ISBN pode estar duplicado)\n";
                    sqlite3_free(errMsg);
                } else {
       
                    Livro* l = new Livro(isbn, titulo, autor);
                    acervo.push_back(l);
                    cout << "Livro cadastrado com sucesso!\n";
                }
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
                if (!l->isDisponivel()) {
                    cout << "Livro indisponível.\n";
                    break;
                }
                cout << "Data empréstimo (yyyy/mm/dd): "; getline(cin, dataEmp);
                dataPrev = calcularDataPrevista(dataEmp);
                
       
                string sqlEmp = "UPDATE livros SET disponivel = 0 WHERE isbn = '" + isbn + "';";
                sqlite3_exec(db, sqlEmp.c_str(), 0, 0, &errMsg);
                
        
                l->setDisponivel(false);
                string sqlInsertEmp =
                "INSERT INTO emprestimos "
                "(matricula, isbn, dataEmprestimo, dataPrevista, devolvido) VALUES ("
                + to_string(mat) +
                ", '" + isbn + 
                "', '" + dataEmp +
                "', '" + dataPrev +
                "', 0);"; 
                
                sqlite3_exec(db, sqlInsertEmp.c_str(), 0, 0, &errMsg);

                Emprestimo* emp = new Emprestimo(dataEmp, dataPrev, u, l);
                emprestimos.push_back(emp);
                u->adicionarEmprestimo(emp);
                cout << "Empréstimo realizado.\n";
                break;
            }

      
            case 4: {
                string isbn;
                cout << "ISBN do livro: "; getline(cin, isbn);
                Livro* l = buscarLivro(acervo, isbn);
                if (!l) {
                    cout << "Livro não encontrado.\n";
                    break;
                }
                if (l->isDisponivel()) {
                    cout << "Livro já está disponível.\n";
                    break;
                }
                
    
                string sqlDev = "UPDATE livros SET disponivel = 1 WHERE isbn = '" + isbn + "';";
                sqlite3_exec(db, sqlDev.c_str(), 0, 0, &errMsg);

                string sqlDevEmp =
                "UPDATE emprestimos" 
                "SET devolvido = 1"
                "WHERE isbn = '" + isbn + "' AND devolvido = 0;";
                sqlite3_exec(db, sqlDevEmp.c_str(), 0, 0, &errMsg);

                
    
                l->setDisponivel(true);
                cout << "Livro devolvido.\n";
                break;
            }

      
            case 5: {
                cout << "\n--- ACERVO ---\n";
                
     
                string sqlBusca = "SELECT isbn, titulo, autor, disponivel FROM livros;";
                sqlite3_stmt* stmt2;
                sqlite3_prepare_v2(db, sqlBusca.c_str(), -1, &stmt2, 0);
                bool temLivro = false;
                while (sqlite3_step(stmt2) == SQLITE_ROW) {
                    temLivro = true;
                    string isbn = (char*)sqlite3_column_text(stmt2, 0);
                    string titulo = (char*)sqlite3_column_text(stmt2, 1);
                    string autor = (char*)sqlite3_column_text(stmt2, 2);
                    int disp = sqlite3_column_int(stmt2, 3);
                    cout << "Título: " << titulo 
                         << ", Autor: " << autor 
                         << ", ISBN: " << isbn 
                         << ", Disponível: " << (disp ? "Sim" : "Não") << "\n";
                }
                sqlite3_finalize(stmt2);
                if (!temLivro) cout << "Nenhum livro cadastrado.\n";
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

            case 8: {
                string autor;
                cout << "Digite o nome do autor: ";
                getline(cin, autor);
                string sql = "SELECT COUNT(*) FROM livros WHERE autor = '" + autor + "';";
                sqlite3_stmt* stmt3;
                sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt3, 0);
                int total = 0;
                if (sqlite3_step(stmt3) == SQLITE_ROW) {
                    total = sqlite3_column_int(stmt3, 0);
                }
                sqlite3_finalize(stmt3);
                cout << "Existem " << total << " livros do autor " << autor << " no acervo.\n";
                break;
            }

            case 9: {
                string sql = "SELECT COUNT(*) FROM livros WHERE disponivel = 1;";
                sqlite3_stmt* stmt4;
                sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt4, 0);
                int total = 0;
                if (sqlite3_step(stmt4) == SQLITE_ROW) {
                    total = sqlite3_column_int(stmt4, 0);
                }
                sqlite3_finalize(stmt4);
                cout << "Total de livros disponíveis: " << total << "\n";
                break;
            }
                

            case 10: {

    string sql =
    "SELECT matricula, isbn, dataEmprestimo, dataPrevista "
    "FROM emprestimos "
    "WHERE devolvido = 0;";

    sqlite3_stmt* stmt5;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt5, 0);

    time_t agora = time(0);
    tm* dataAtual = localtime(&agora);

    char hoje[11];
    strftime(hoje, sizeof(hoje), "%Y-%m-%d", dataAtual);
    string dataHoje = hoje;

    bool atrasado = false;

    cout << "\n--- EMPRÉSTIMOS ATRASADOS ---\n";

    while (sqlite3_step(stmt5) == SQLITE_ROW) {

        string dataPrevista = (char*)sqlite3_column_text(stmt5, 3);

        if (dataHoje > dataPrevista) {

            atrasado = true;

            cout << "Matrícula: "
                 << sqlite3_column_int(stmt5, 0)
                 << "\nISBN: "
                 << (char*)sqlite3_column_text(stmt5, 1)
                 << "\nData do empréstimo: "
                 << (char*)sqlite3_column_text(stmt5, 2)
                 << "\nData prevista: "
                 << dataPrevista
                 << "\n--------------------------\n";
        }
    }

    if (!atrasado)
        cout << "Nenhum empréstimo atrasado.\n";

    sqlite3_finalize(stmt5);

    break;
}

            default:
                cout << "Opção inválida.\n";
        }
    } while (op != 7);

    for (size_t i = 0; i < emprestimos.size(); i++) delete emprestimos[i];
    for (size_t i = 0; i < acervo.size(); i++) delete acervo[i];
    for (size_t i = 0; i < usuarios.size(); i++) delete usuarios[i];
    delete bib;

    sqlite3_close(db);
    cout << "Banco fechado.\n";

    return 0;
}
            
