#include <fstream>

#include "lexer.hpp"
#include "ast.hpp"
#include "visitor.hpp"

using namespace std;

string open_file(string file_path)
{
    std::ifstream fichier(file_path);

    if (!fichier.is_open()) {
        std::cerr << "Impossible to open the file.." << std::endl;
        return "";
    }
    std::string buffer;
    std::string line;
    while (std::getline(fichier, line)) {
        buffer += line;
        buffer += '\n';
    }
    fichier.close();
    return buffer;
}

int main(int ac, char **av)
{
    string source_code = open_file("./indexs.shk");
    Lexer *lexer = new Lexer();
    vector<token *> array = lexer->process(source_code);

    AST *ast = new AST();
    Visitor *v = new Visitor();

    ast->build(array);
    ast->accept(v);

    // for (auto it = array.begin(); it != array.end(); ++it) {
        // cout << "value: " << (*it)->value << " type: " <<  (*it)->type << "\n";
        // delete (*it);
    // }

    array.clear();
    delete lexer;
}
