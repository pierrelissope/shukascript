#ifndef AST_H

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

        void build(std::vector<token *> tokens);
};

class Expression : public AST {
    public:
        void accept(Visitor *v) override;

        Expression(std::vector<token *> tokens);
};

class Assignement : public AST {
    public:
        void accept(Visitor *v) override;

        Assignement(std::string var_type, std::string var_name, Expression *expression);
};

class Value : public AST{
    public:
        void accept(Visitor *v) override;
};

class Structure : public AST{
    public:

        Expression *expression = nullptr;

        void accept(Visitor *v) override;

        Structure(std::string struct_type, Expression *expression, std::vector<token *>tokens);
};

class Function : public AST {
    public:
        void accept(Visitor *v) override;
};

#define AST_H_


#endif /* !AST_H_ */
