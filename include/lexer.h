#ifndef LEXER_H_

#include <iostream>
#include <string>
#include <map>
#include <vector>

enum token_type {
    IDENTIFIER = 0, // Random Words
    OPERATOR, // +, -, /, *
    COMPARATOR, // ==, <=, >=, <, >
    ASSIGNATOR, // =
    O_PARENTHESE, // (
    C_PARENTHESE, // )
    O_BRACKET, // {
    C_BRACKET, // }
    END_EXPRESSION, // ;
};

struct token {
    token_type type;
    std::string value;

    token(token_type type, std::string value);
};

class Lexer {

    private:
        std::vector<token *> token_array;
        std::string::iterator source_code_it;
    public:
        Lexer(void) {};
        std::vector<token *> process(std::string);
};

#define LEXER_H_


#endif /* !LEXER_H_ */
