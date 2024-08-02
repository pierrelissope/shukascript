#ifndef AST_H

#include <variant>

#include "lexer.hpp"

class Visitor;
class AST;
class Function;

typedef std::variant<int, double, std::string, Function *> variant_t;

class AST {
    public:
        AST() = default;
        virtual ~AST() = default;

        std::map<std::string, variant_t> variables;

        std::vector<AST *> nodes;
        std::size_t current_token_index = 0;

        virtual void accept(AST *parent_node, Visitor *v);

        std::vector<token *> get_expression_tokens(std::vector<token *> tokens);
        std::vector<token *> get_structure_tokens(std::vector<token *> tokens);
        std::vector<std::string> get_args(std::vector<token *> tokens, std::vector<std::string> &types);

        void build(std::vector<token *> tokens);
};


class Expression : public AST {
    public:

        std::string op;

        Expression *left = nullptr;
        Expression *right = nullptr;

        virtual void accept(AST *parent_node, Visitor *v) override;
        virtual variant_t evaluate(std::map<std::string, variant_t> variables, Visitor *v);
};

class Expression_Parser {

    public:
        std::size_t current_token_index = 0;
        std::vector<token *> tokens;

        Expression_Parser(std::vector<token *> tokens) {this->tokens = tokens;};
        Expression *parse_cmp(void);
        Expression *parse_add_min(void);
        Expression *parse_mul_div(void);
        Expression *parse_parentheses(void);
        Expression *parse_value(void);
};

class ValueNode : public Expression {

    public:

        variant_t value;
    
        void accept(AST *parent_node, Visitor *v) override;
        variant_t evaluate(std::map<std::string, variant_t> variables, Visitor *v) override;

};

class VariableNode : public Expression {
    public:
    
        std::string name;

        void accept(AST *parent_node, Visitor *v) override;
        variant_t evaluate(std::map<std::string, variant_t> variables, Visitor *v) override;
};

class FunctionCallNode : public Expression {

    public:

        std::string name;
        std::vector<Expression *> args;
    
        void accept(AST *parent_node, Visitor *v) override;
        variant_t evaluate(std::map<std::string, variant_t> variables, Visitor *v) override;

};

class Assignement : public AST {
    public:
        void accept(AST *parent_node, Visitor *v) override;

        std::string var_type;
        std::string var_name;
        Expression *expression;

        Assignement(std::string var_type, std::string var_name, Expression *expression);
};

class Structure : public AST{
    public:

        std::string type;
        std::vector<token *> tokens;
        Expression *expression;

        void accept(AST *parent_node, Visitor *v) override;

        Structure(std::string struct_type, Expression *expression, std::vector<token *>tokens);
};

class Function : public AST {
    public:
        void accept(AST *parent_node, Visitor *v) override;

        std::string name;
        std::string return_type;
        std::vector<token *> tokens;
        std::vector<std::string> args;
        std::vector<std::string> args_types;

        Function(std::vector<token *>tokens, std::string return_type, std::string name, std::vector<std::string> args, std::vector<std::string> args_types);
};

Expression *create_expression(std::vector<token *> tokens);

#define AST_H_


#endif /* !AST_H_ */
