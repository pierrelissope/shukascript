#include "lexer.h"

using namespace std;

token::token(token_type type, std::string value) {
    this->type = type;
    this->value = value;
}

static const map<token_type, std::vector<std::string>> tokens_type_map = {
    {OPERATOR, {"+", "-", "/", "*"}},
    {COMPARATOR, {"==", "<=", ">=", "<", ">"}},
    {ASSIGNATOR, {"="}},
    {O_PARENTHESE, {"("}},
    {C_PARENTHESE, {")"}},
    {O_BRACKET, {"{"}},
    {C_BRACKET, {"}"}},
    {END_EXPRESSION, {";"}}
};

vector<token *> Lexer::process(string source_code)
{
    source_code_it = source_code.begin();
    bool is_identifier = true;
    string new_identifier = "";

    while (source_code_it != source_code.end()) {
        is_identifier = true;
        if (*source_code_it == ' ' || *source_code_it == '\n' || *source_code_it == '\t') {
            ++source_code_it;
            continue;
        }
        for (auto map_it = tokens_type_map.begin(); map_it != tokens_type_map.end(); ++map_it) {
            for (auto list_it = map_it->second.begin(); list_it != map_it->second.end(); ++list_it) {
                if (source_code.substr(std::distance(source_code.begin(), source_code_it), list_it->length()) == (*list_it)) {
                    token_array.push_back(new token(map_it->first, (*list_it)));
                    is_identifier = false;
                    source_code_it += list_it->length();
                    break;
                }
            }
            if (!is_identifier)
                break;
        }
        if (is_identifier) {
            if (token_array.empty() || token_array.back()->type != IDENTIFIER) {
                new_identifier = *source_code_it;
                token_array.push_back(new token(IDENTIFIER, new_identifier));
            } else
                token_array.back()->value += *source_code_it;
            ++source_code_it;
        }
    }

    return token_array;
}