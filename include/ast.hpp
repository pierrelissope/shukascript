#ifndef AST_H

#include <iostream>
#include <memory>

#include "shkvariant.hpp"
#include "lexer.hpp"

class Visitor;
class AST;
class Function;

class AST : public std::enable_shared_from_this<AST> {
    public:
        std::vector<std::shared_ptr<AST>> nodes;
        AST() = default;

        std::map<std::string, variant_t> variables;

        virtual ~AST() {nodes.clear();};
        std::size_t current_token_index = 0;

        virtual void accept(std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v);

        std::vector<std::shared_ptr<token>> get_expression_tokens(std::vector<std::shared_ptr<token>> tokens);
        std::vector<std::shared_ptr<token>> get_structure_tokens(std::vector<std::shared_ptr<token>> tokens);
        std::vector<std::string> get_args(std::vector<std::shared_ptr<token>> tokens, std::vector<std::string> &types);

        void build(std::vector<std::shared_ptr<token>> tokens);
};


class Expression : public AST {
    public:

        std::string op;

        std::shared_ptr<Expression> left = nullptr;
        std::shared_ptr<Expression> right = nullptr;

        virtual void accept(std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v) override;
        virtual variant_t evaluate(std::map<std::string, variant_t> variables, std::shared_ptr<Visitor> v);
};

class Expression_Parser {

    public:
        std::size_t current_token_index = 0;
        std::vector<std::shared_ptr<token>> tokens;


        Expression_Parser(std::vector<std::shared_ptr<token>> tokens) {this->tokens = tokens;};
        ~Expression_Parser() {tokens.clear();};
        std::shared_ptr<Expression> parse_cmp(void);
        std::shared_ptr<Expression> parse_add_min(void);
        std::shared_ptr<Expression> parse_mul_div(void);
        std::shared_ptr<Expression> parse_parentheses(void);
        std::shared_ptr<Expression> parse_value(void);
};

class ValueNode : public Expression {

    public:

        variant_t value;
    
        void accept(std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v) override;
        variant_t evaluate(std::map<std::string, variant_t> variables, std::shared_ptr<Visitor> v) override;

};

class VariableNode : public Expression {
    public:
    
        std::string name;

        void accept(std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v) override;
        variant_t evaluate(std::map<std::string, variant_t> variables, std::shared_ptr<Visitor> v) override;
};

class FunctionCallNode : public Expression {

    public:

        std::string name;
        std::vector<std::shared_ptr<Expression>> args;
    
        ~FunctionCallNode() {nodes.clear(); args.clear();};
        void accept(std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v) override;
        variant_t evaluate(std::map<std::string, variant_t> variables, std::shared_ptr<Visitor> v) override;

};

class Assignement : public AST {
    public:
        void accept(std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v) override;

        std::string var_type;
        std::string var_name;
        std::shared_ptr<Expression> expression;

        Assignement(std::string var_type, std::string var_name, std::shared_ptr<Expression> expression);
};

class Structure : public AST{
    public:

        std::string type;
        std::vector<std::shared_ptr<token>> tokens;
        std::shared_ptr<Expression> expression;

        void accept(std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v) override;

        ~Structure() {nodes.clear(); tokens.clear();};
        Structure(std::string struct_type, std::shared_ptr<Expression> expression, std::vector<std::shared_ptr<token>>tokens);
};

class Function : public AST {
    public:
        void accept(std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v) override;

        std::string name;
        std::string return_type;
        std::vector<std::shared_ptr<token>> tokens;
        std::vector<std::string> args;
        std::vector<std::string> args_types;

        ~Function() {nodes.clear(); tokens.clear(); args.clear(); args_types.clear();};
        Function(std::vector<std::shared_ptr<token>>tokens, std::string return_type, std::string name, std::vector<std::string> args, std::vector<std::string> args_types);
};

std::shared_ptr<Expression> create_expression(std::vector<std::shared_ptr<token>> tokens);

#define AST_H_


#endif /* !AST_H_ */
