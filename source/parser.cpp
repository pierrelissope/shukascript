#include "ast.hpp"

using namespace std;

void AST::accept(AST *parent_node, Visitor *v) {
    for (AST *node : nodes)
        node->accept(this, v);
}

vector<token *> AST::get_expression_tokens(vector<token *> tokens) {
    vector<token *> new_tokens;

    while (current_token_index < tokens.size() &&
           (tokens[current_token_index]->type == IDENTIFIER ||
            tokens[current_token_index]->type == STRING_DELIMITER ||
            tokens[current_token_index]->type == O_PARENTHESE ||
            tokens[current_token_index]->type == C_PARENTHESE ||
            tokens[current_token_index]->type == COMMA ||
            tokens[current_token_index]->type == COMPARATOR ||
            tokens[current_token_index]->type == PRIO_OPERATOR ||
            tokens[current_token_index]->type == NOPRIO_OPERATOR)) {
        new_tokens.push_back(tokens[current_token_index]);
        ++current_token_index;
    }
    return new_tokens;
}

Expression *create_expression(std::vector<token *> tokens)
{
    Expression_Parser *parser = new Expression_Parser(tokens);
    Expression *expression = parser->parse_cmp();

    delete parser;
    return expression;
}

vector<string> AST::get_args(vector<token *> tokens, vector<string> &types) {
    vector<string> new_tokens;
    string next_arg_type = "int";

    ++current_token_index;
    while (current_token_index < tokens.size() && tokens[current_token_index]->type != C_PARENTHESE) {
        if (tokens[current_token_index]->type == IDENTIFIER) {
            new_tokens.push_back(tokens[current_token_index]->value);
            types.push_back(next_arg_type);
            next_arg_type = "int";
        }
        if (tokens[current_token_index]->type == TYPE_IDENTIFIER)
            next_arg_type = tokens[current_token_index]->value;
        ++current_token_index;
    }
    ++current_token_index;
    return new_tokens;
}

vector<token *> AST::get_structure_tokens(vector<token *> tokens) {
    vector<token *> new_tokens;
    ino64_t count = 1;

    ++current_token_index;
    while (current_token_index < tokens.size() && count != 0) {
        if (tokens[current_token_index]->type == O_BRACKET) ++count;
        if (tokens[current_token_index]->type == C_BRACKET) --count;
        if (count != 0)
            new_tokens.push_back(tokens[current_token_index]);
        ++current_token_index;
    }
    return new_tokens;
}

void AST::build(vector<token *> tokens)
{
    string next_val_type = "int";
    string next_func_type = "int";
    string next_val_name = "unknown";
    string next_struct_type = "if";
    string next_func_name = "uknown_func";

    while (current_token_index < tokens.size()) {
        // Detect type identifier and set the next var type
        if (tokens[current_token_index]->type == TYPE_IDENTIFIER) {
            next_val_type = tokens[current_token_index]->value;
            ++current_token_index;
        }

        // Detect Simple Identifier
        else if (tokens[current_token_index]->type == IDENTIFIER) {
            if (current_token_index + 1 < tokens.size() && tokens[current_token_index + 1]->type == ASSIGNATOR) {
                ++current_token_index;
                next_val_name = tokens[current_token_index - 1]->value;
                ++current_token_index;
                if (current_token_index >= tokens.size()) break;
                vector<token *> exp_tokens = this->get_expression_tokens(tokens);
                // Check if the expression is not empty
                if (exp_tokens.size() != 0) {
                    Expression *new_expression = create_expression(exp_tokens);
                    if (new_expression != nullptr) {
                        Assignement *new_assignement = new Assignement(next_val_type, next_val_name, new_expression);
                        nodes.push_back(new_assignement);
                    }
                }
                exp_tokens.clear();
            } else {

                vector<token *> exp_tokens = this->get_expression_tokens(tokens);
                Expression *new_expression = create_expression(exp_tokens);
                exp_tokens.clear();
                if (new_expression != nullptr) {
                    nodes.push_back(new_expression);
                }
            }
        }


        // Detect Structure Identifier
        else if (tokens[current_token_index]->type == STRUCTURE_IDENTIFIER) {
            next_struct_type = tokens[current_token_index]->value;
            ++current_token_index;
            if (current_token_index >= tokens.size()) break;
            vector<token *> exp_tokens = this->get_expression_tokens(tokens);
            // Check if the expression is not empty
            Expression *new_expression = nullptr;
            if (exp_tokens.size() != 0) {
                new_expression = create_expression(exp_tokens);
                if (new_expression != nullptr) {
                    // Check if There is a content inside the expression
                    if (tokens[current_token_index]->type == O_BRACKET) {
                        vector<token *> struct_content_tokens = this->get_structure_tokens(tokens);
                        Structure *new_structure = new Structure(next_struct_type, new_expression, struct_content_tokens);
                        nodes.push_back(new_structure);
                        struct_content_tokens.clear();
                    } else {
                        delete new_expression;
                        exp_tokens.clear();
                        cerr << "Error: you must add content inside your structure";
                        break;
                    }
                }
            } else {
                exp_tokens.clear();
                cerr << "Error: you must enter an expression";
                break;
            }
            exp_tokens.clear();
        }


        // Detect Function identifier
        else if (tokens[current_token_index]->type == FUNCTION_IDENTIFIER) {
            ++current_token_index;
            if (current_token_index >= tokens.size()) break;
            if (tokens[current_token_index]->type == TYPE_IDENTIFIER) {
                next_func_type = tokens[current_token_index]->value;
                ++current_token_index;
                if (current_token_index >= tokens.size()) break;
                // Check if the function has a name
                if (tokens[current_token_index]->type == IDENTIFIER) {
                    next_func_name = tokens[current_token_index]->value;
                    ++current_token_index;
                    if (current_token_index >= tokens.size()) break;
                    // Check ifthe args parentheses are here 
                    if (tokens[current_token_index]->type == O_PARENTHESE) {
                        vector<string> args_types;
                        vector<string> args = this->get_args(tokens, args_types);
                        // Check if There is a content inside the expression
                        if (tokens[current_token_index]->type == O_BRACKET) {
                            vector<token *> func_content_tokens = this->get_structure_tokens(tokens);
                            Function *new_function = new Function(func_content_tokens, next_func_type, next_func_name,
                                args, args_types);
                            nodes.push_back(new_function);
                            func_content_tokens.clear();
                        } else {
                            args_types.clear();
                            args.clear();
                            cerr << "Error: you must add content inside your structure" << endl;
                            break;
                        }
                    } else {
                        cerr << "Error: Invalid token missing (" << endl;
                        break;
                    }
                } else {
                    cerr << "Error: each function must have a name" << endl;
                    break;
                }
            } else {
                cerr << "Error: each function must have a type identifier" << endl;
                break;
            }
            
        }


        else {
            ++current_token_index;
        }

    }
}
