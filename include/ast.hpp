#ifndef AST_H

#include <variant>
#include "lexer.hpp"

class Visitor;

class AST {
    public:
        AST() = default;
        virtual ~AST() = default;

        std::vector<AST *> nodes;
        std::size_t current_token_index = 0;

        virtual void accept(Visitor *v);
        void safe_increment(int val, std::vector<token *> tokens);

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

        virtual void accept(Visitor *v) override;
        virtual void evaluate(void);
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

        std::variant<int, double, std::string> value;
    
        void accept(Visitor *v) override;
        void evaluate(void) override;

};

class VariableNode : public Expression {
    public:
    
        std::string name;

        void accept(Visitor *v) override;
        void evaluate(void) override;
};

class FunctionCallNode : public Expression {

    public:

        std::string name;
        std::vector<Expression *> args;
    
        void accept(Visitor *v) override;
        void evaluate(void) override;

};

class Assignement : public AST {
    public:
        void accept(Visitor *v) override;

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

        void accept(Visitor *v) override;

        Structure(std::string struct_type, Expression *expression, std::vector<token *>tokens);
};

class Function : public AST {
    public:
        void accept(Visitor *v) override;

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
