#include "ast.hpp"
#include "visitor.hpp"

Assignement::Assignement(std::string var_type, std::string var_name, std::shared_ptr<Expression> expression)
{
    this->var_type = var_type;
    this->var_name = var_name;
    this->expression = expression;
}

Structure::Structure(std::string struct_type, std::shared_ptr<Expression> expression, std::vector<std::shared_ptr<token>>tokens)
{
    this->type = struct_type;
    this->expression = expression;
    this->tokens = tokens;
    this->build(tokens);
}

Function::Function(std::vector<std::shared_ptr<token>>tokens, std::string return_type, std::string name, std::vector<std::string> args, std::vector<std::string> args_types) {

    this->name = name;
    this->tokens = tokens;
    this->return_type = return_type;
    this->args = args;
    this->args_types = args_types;
    this->build(tokens);
}

void ValueNode::accept(std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v) {

    std::shared_ptr<ValueNode> self = std::dynamic_pointer_cast<ValueNode>(shared_from_this());
    v->visit_value(parent_node, self);
}

void VariableNode::accept(std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v) {
    std::shared_ptr<VariableNode> self = std::dynamic_pointer_cast<VariableNode>(shared_from_this());
    v->visit_variable(parent_node, self);
}

void FunctionCallNode::accept(std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v) {
    std::shared_ptr<FunctionCallNode> self = std::dynamic_pointer_cast<FunctionCallNode>(shared_from_this());
    v->visit_fcall(parent_node, self);
}




void Assignement::accept(std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v) {
    std::shared_ptr<Assignement> self = std::dynamic_pointer_cast<Assignement>(shared_from_this());
    v->visit_assignement(parent_node, self);
}

void Expression::accept(std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v) {
    std::shared_ptr<Expression> self = std::dynamic_pointer_cast<Expression>(shared_from_this());
    v->visit_expression(parent_node, self);
}

void Structure::accept(std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v) {
    std::shared_ptr<Structure> self = std::dynamic_pointer_cast<Structure>(shared_from_this());
    v->visit_structure(parent_node, self);
}

void Function::accept(std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v) {
    std::shared_ptr<Function> self = std::dynamic_pointer_cast<Function>(shared_from_this());
    v->visit_function(parent_node, self);
}
