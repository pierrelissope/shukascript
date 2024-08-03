#include <fstream>

#include "lexer.hpp"
#include "ast.hpp"
#include "visitor.hpp"

using namespace std;

string open_file(string file_path)
{

    if (file_path.substr(file_path.find(".") + 1, file_path.size() - 1) != "shk") {
        std::cerr << "Wrong file format" << std::endl;
        return "";
    }
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
    string source_code = open_file(av[1]);

    if (source_code == "")
        return 1;
    Lexer *lexer = new Lexer();
    vector<token *> array = lexer->process(source_code);
    AST *ast = new AST();
    Visitor *v = new Visitor();

    ast->build(array);
    ast->accept(nullptr, v);
    array.clear();
    delete lexer;
    return 0;
}
