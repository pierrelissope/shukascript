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

void ValueNode::accept(AST *parent_node, Visitor *v) {
    v->visit_value(parent_node, this);
}

void VariableNode::accept(AST *parent_node, Visitor *v) {
    v->visit_variable(parent_node, this);
}

void FunctionCallNode::accept(AST *parent_node, Visitor *v) {
    v->visit_fcall(parent_node, this);
}




void Assignement::accept(AST *parent_node, Visitor *v) {
    v->visit_assignement(parent_node, this);
}

void Expression::accept(AST *parent_node, Visitor *v) {
    v->visit_expression(parent_node, this);
}

void Structure::accept(AST *parent_node, Visitor *v) {
    v->visit_structure(parent_node, this);
}

void Function::accept(AST *parent_node, Visitor *v) {
    v->visit_function(parent_node, this);
}
