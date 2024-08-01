#include "ast.hpp"
#include "visitor.hpp"

Assignement::Assignement(std::string var_type, std::string var_name, Expression *expression)
{
    this->var_type = var_type;
    this->var_name = var_name;
    this->expression = expression;
}

Structure::Structure(std::string struct_type, Expression *expression, std::vector<token *>tokens)
{
    this->type = struct_type;
    this->expression = expression;
    this->tokens = tokens;
    this->build(tokens);
}

Function::Function(std::vector<token *>tokens, std::string return_type, std::string name, std::vector<std::string> args, std::vector<std::string> args_types) {

    this->name = name;
    this->tokens = tokens;
    this->return_type = return_type;
    this->args = args;
    this->args_types = args_types;
    this->build(tokens);
}

void ValueNode::accept(Visitor *v) {
    v->visit_value(this);
}

void VariableNode::accept(Visitor *v) {
    v->visit_variable(this);
}

void FunctionCallNode::accept(Visitor *v) {
    v->visit_fcall(this);
}


void Assignement::accept(Visitor *v) {
    v->visit_assignement(this);
}

void Expression::accept(Visitor *v) {
    v->visit_expression(this);

    this->left->accept(v);
    this->right->accept(v);
}

void Structure::accept(Visitor *v) {
    v->visit_structure(this);
}

void Function::accept(Visitor *v) {
    v->visit_function(this);
}
