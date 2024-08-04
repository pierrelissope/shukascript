#include "visitor.hpp"
#include "ast.hpp"

using namespace std;

void Visitor::visit_assignement(shared_ptr<AST> parent_node, shared_ptr<Assignement> node) {
    if (node == nullptr)
        return;
    std::shared_ptr<Visitor> self = std::dynamic_pointer_cast<Visitor>(shared_from_this());
    parent_node->variables[node->var_name] = node->expression->evaluate(parent_node->variables, self);
}

void Visitor::visit_expression(shared_ptr<AST> parent_node, shared_ptr<Expression> node) {
    if (node == nullptr)
        return;
    std::shared_ptr<Visitor> self = std::dynamic_pointer_cast<Visitor>(shared_from_this());
    node->evaluate(parent_node->variables, self);
}

void Visitor::visit_value(shared_ptr<AST> parent_node, shared_ptr<ValueNode> node) {
    if (node == nullptr)
        return;
    std::shared_ptr<Visitor> self = std::dynamic_pointer_cast<Visitor>(shared_from_this());
    node->evaluate(parent_node->variables, self);
}

void Visitor::visit_fcall(shared_ptr<AST> parent_node, shared_ptr<FunctionCallNode> node) {
    if (node == nullptr)
        return;
    std::shared_ptr<Visitor> self = std::dynamic_pointer_cast<Visitor>(shared_from_this());
    node->evaluate(parent_node->variables, self);

}

void Visitor::visit_variable(shared_ptr<AST> parent_node, shared_ptr<VariableNode> node) {
    if (node == nullptr)
        return;
    std::shared_ptr<Visitor> self = std::dynamic_pointer_cast<Visitor>(shared_from_this());
    node->evaluate(parent_node->variables, self);
}

void Visitor::visit_structure(shared_ptr<AST> parent_node, shared_ptr<Structure>  node) {
    if (node == nullptr)
        return;    
    std::shared_ptr<Visitor> self = std::dynamic_pointer_cast<Visitor>(shared_from_this());
    node->variables = parent_node->variables;

    if (node->type == "if") {
        variant_t exp_value = node->expression->evaluate(node->variables, self);
        if ((std::holds_alternative<int>(exp_value) && std::get<int>(exp_value)) ||
            (std::holds_alternative<double>(exp_value) && std::get<double>(exp_value))) {
            for (auto structline : node->nodes)
                structline->accept(node, self);
            parent_node->variables = node->variables;
        }
    } else if (node->type == "while") {

        variant_t exp_value = node->expression->evaluate(node->variables, self);
        while ((std::holds_alternative<int>(exp_value) && std::get<int>(exp_value)) ||
            (std::holds_alternative<double>(exp_value) && std::get<double>(exp_value))) {
            for (auto structline : node->nodes)
                structline->accept(node, self);
            parent_node->variables = node->variables;
            exp_value = node->expression->evaluate(node->variables, self);
        }

    } else {
        throw std::runtime_error("Error: Unknown structure type");
    }
}

void Visitor::visit_function(shared_ptr<AST> parent_node, shared_ptr<Function> node) {
    if (node == nullptr)
        return;
    parent_node->variables[node->name] = node;
}
