#include <sstream>
#include <locale>

#include "ast.hpp"
#include "visitor.hpp"

using namespace std;

bool is_float(const std::string& str)
{
    std::stringstream ss(str);
    float f;
    char c;

    ss >> f;
    return !(ss.fail() || ss.get(c) || str.empty());
}

bool is_all_digits(const std::string& str)
{
    if (str.empty()) return false;
    
    for (char c : str) {
        if (!std::isdigit(static_cast<unsigned char>(c)))
            return false;
    }
    return true;
}

shared_ptr<Expression> Expression_Parser::parse_cmp(void)
{
    shared_ptr<Expression> node = parse_add_min();
    shared_ptr<Expression> right = nullptr;
    
    if (node == nullptr)
        return nullptr;
    while (current_token_index < tokens.size() && tokens[current_token_index]->type == COMPARATOR) {
        string op = tokens[current_token_index++]->value;
        right = parse_add_min();

        if (right != nullptr) {
            shared_ptr<Expression> new_node = make_shared<Expression>(); 
            new_node->left = node;
            new_node->right = right;
            new_node->op = op;
            node = new_node;
        }
    }
    return node;
}

shared_ptr<Expression> Expression_Parser::parse_add_min(void)
{
    shared_ptr<Expression> node = parse_mul_div();
    shared_ptr<Expression> right = nullptr;
    
    if (node == nullptr)
        return nullptr;
    while (current_token_index < tokens.size() && tokens[current_token_index]->type == NOPRIO_OPERATOR) {
        string op = tokens[current_token_index++]->value;
        right = parse_mul_div();

        if (right != nullptr) {
            shared_ptr<Expression> new_node = make_shared<Expression>(); 
            new_node->left = node;
            new_node->right = right;
            new_node->op = op;
            node = new_node;
        }
    }
    return node;
}

shared_ptr<Expression> Expression_Parser::parse_mul_div(void)
{
    shared_ptr<Expression> node = parse_parentheses();
    shared_ptr<Expression> right = nullptr;
    
    if (node == nullptr)
        return nullptr;
    while (current_token_index < tokens.size() && tokens[current_token_index]->type == PRIO_OPERATOR) {
        string op = tokens[current_token_index++]->value;
        right = parse_parentheses();
        if (right != nullptr) {
            shared_ptr<Expression> new_node = make_shared<Expression>(); 
            new_node->left = node;
            new_node->right = right;
            new_node->op = op;
            node = new_node;
        }
    }
    return node;
}

shared_ptr<Expression> Expression_Parser::parse_parentheses(void)
{
    if (current_token_index >= tokens.size())
        return nullptr;
    if (tokens[current_token_index]->type != O_PARENTHESE)
        return (parse_value());
    vector<shared_ptr<token>> private_tokens;

    int open_p = 0;
    ++current_token_index;
    ++open_p;
    while (current_token_index < tokens.size() && open_p != 0) {
        if (tokens[current_token_index]->type == C_PARENTHESE)
            --open_p;
        if (tokens[current_token_index]->type == O_PARENTHESE)
            ++open_p;
        private_tokens.push_back(tokens[current_token_index]);
        ++current_token_index;
    }
    
    shared_ptr<Expression> node = create_expression(private_tokens);
    private_tokens.clear();
    return node;
}

shared_ptr<Expression> Expression_Parser::parse_value(void)
{
    if (is_all_digits(tokens[current_token_index]->value)) {
        // FULLNOMBRE
        shared_ptr<ValueNode> value_node = make_shared<ValueNode> ();
        value_node->value = atoi(tokens[current_token_index++]->value.c_str());
        return value_node;
        
    } else if (is_float(tokens[current_token_index]->value)) {
        // FULLFLOAT

        shared_ptr<ValueNode> value_node = make_shared<ValueNode> ();
        value_node->value = atof(tokens[current_token_index++]->value.c_str());
        return value_node;

    } else if (tokens[current_token_index]->type == STRING_DELIMITER) {
        // FULLSTRING

        ++current_token_index;
        shared_ptr<ValueNode> value_node = make_shared<ValueNode> ();
        string content = "";

        while (current_token_index < tokens.size() && tokens[current_token_index]->type == IDENTIFIER)
            content += (tokens[current_token_index++]->value + " ");
        value_node->value = content;
        if (current_token_index < tokens.size() && tokens[current_token_index]->type == STRING_DELIMITER)
            ++current_token_index;
        return value_node;

    } else if (tokens[current_token_index]->type == IDENTIFIER) {
        // FCALL
        if (current_token_index + 1 < tokens.size() && tokens[current_token_index + 1]->type == O_PARENTHESE) {
            shared_ptr<FunctionCallNode> fcall_node = make_shared<FunctionCallNode>();

            fcall_node->name = tokens[current_token_index]->value;
            current_token_index += 2;
            int o_count = 1;
            while (current_token_index < tokens.size() && o_count != 0) {
                
                vector<shared_ptr<token>> new_tokens;
                while (current_token_index < tokens.size() &&
                   (tokens[current_token_index]->type == IDENTIFIER ||
                    tokens[current_token_index]->type == STRING_DELIMITER ||
                    tokens[current_token_index]->type == O_PARENTHESE ||
                    tokens[current_token_index]->type == C_PARENTHESE ||
                    tokens[current_token_index]->type == COMPARATOR ||
                    tokens[current_token_index]->type == PRIO_OPERATOR ||
                    (tokens[current_token_index]->type == COMMA && o_count != 1) ||
                    tokens[current_token_index]->type == NOPRIO_OPERATOR) &&
                    o_count != 0) {

                    if (tokens[current_token_index]->type == C_PARENTHESE)
                        --o_count;
                    if (tokens[current_token_index]->type == O_PARENTHESE)
                        ++o_count;
                    if (o_count == 0)
                        break;
                    new_tokens.push_back(tokens[current_token_index]);
                    ++current_token_index;
                }
                shared_ptr<Expression> expression = create_expression(new_tokens);
                if (expression != nullptr)
                    fcall_node->args.push_back(expression);
                new_tokens.clear();

                if (current_token_index < tokens.size() && tokens[current_token_index]->type == COMMA){
                    ++current_token_index;
                }
            }
            ++current_token_index;
            return fcall_node;
        } else {
            shared_ptr<VariableNode> variable_node = make_shared<VariableNode>();

            variable_node->name = tokens[current_token_index++]->value;
            return variable_node;
        }
    } else {
        cout << "ERROR: expression tokens missing" << endl;
        return nullptr;
    }
    cout << "ERROR: expression tokens missing" << endl;
    return nullptr;
}
