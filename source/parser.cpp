#include "ast.hpp"

using namespace std;

void AST::accept(Visitor *v) {
    for (AST *node : nodes) node->accept(v);
}

void AST::safe_increment(int val, vector<token *> tokens) {
    for (int i = 0; current_token_index < tokens.size() && i < val; ++i) {
        ++current_token_index;
    }
}

vector<token *> AST::get_expression_tokens(vector<token *> tokens) {
    vector<token *> new_tokens;

    while (current_token_index < tokens.size() &&
           (tokens[current_token_index]->type == IDENTIFIER ||
            tokens[current_token_index]->type == O_PARENTHESE ||
            tokens[current_token_index]->type == C_PARENTHESE ||
            tokens[current_token_index]->type == COMPARATOR ||
            tokens[current_token_index]->type == OPERATOR)) {
        new_tokens.push_back(tokens[current_token_index]);
        ++current_token_index;
    }
    return new_tokens;
}

vector<token *> AST::get_structure_tokens(vector<token *> tokens) {
    vector<token *> new_tokens;
    ino64_t count = 1;

    ++current_token_index;
    while (current_token_index < tokens.size() && count != 0) {
        if (tokens[current_token_index]->type == O_BRACKET) ++count;
        if (tokens[current_token_index]->type == C_BRACKET) --count;
        new_tokens.push_back(tokens[current_token_index]);
        ++current_token_index;
    }
    return new_tokens;
}

void AST::build(vector<token *> tokens)
{
    string next_val_type = "int";
    string next_val_name = "unknown";
    string next_struct_type = "if";

    while (current_token_index < tokens.size()) {
        // Detect type identifier and set the next var type
        if (tokens[current_token_index]->type == TYPE_IDENTIFIER)
            next_val_type = tokens[current_token_index]->value;


        // Detect Simple Identifier
        else if (tokens[current_token_index]->type == IDENTIFIER) {
            safe_increment(1, tokens);
            // Create a new Expression and a new Assignement if an Assignator is
            // detected
            if (tokens[current_token_index]->type == ASSIGNATOR) {
                next_val_name = tokens[current_token_index - 1]->value;
                safe_increment(1, tokens);
                vector<token *> exp_tokens =
                    this->get_expression_tokens(tokens);
                // Check if the expression is not empty
                if (exp_tokens.size() != 0) {
                    Expression *new_expression = new Expression(exp_tokens);
                    Assignement *new_assignement = new Assignement(
                        next_val_type, next_val_name, new_expression);
                    nodes.push_back(new_assignement);
                }
                exp_tokens.clear();
            }
        }


        // Detect Structure Identifier
        else if (tokens[current_token_index]->type == STRUCTURE_IDENTIFIER) {
            next_struct_type = tokens[current_token_index]->value;
            safe_increment(1, tokens);
            vector<token *> exp_tokens = this->get_expression_tokens(tokens);
            // Check if the expression is not empty
            Expression *new_expression = nullptr;
            if (exp_tokens.size() != 0) {
                new_expression = new Expression(exp_tokens);
                // Check if There is a content inside the expression
                if (tokens[current_token_index]->type == O_BRACKET) {
                    vector<token *> struct_content_tokens =
                        this->get_structure_tokens(tokens);
                    Structure *new_structure =
                        new Structure(next_struct_type, new_expression,
                                      struct_content_tokens);
                    nodes.push_back(new_structure);
                    struct_content_tokens.clear();
                } else {
                    delete new_expression;
                    exp_tokens.clear();
                    cerr << "Error: you must add content inside your structure";
                    break;
                }
            } else {
                exp_tokens.clear();
                cerr << "Error: you must enter an expression";
                break;
            }
            exp_tokens.clear();
        }
    }
}
