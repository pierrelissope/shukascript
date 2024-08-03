#include "visitor.hpp"
#include "ast.hpp"

using namespace std;

void Visitor::visit_assignement(AST *parent_node, Assignement *node) {
    if (node == nullptr)
        return;
    parent_node->variables[node->var_name] = node->expression->evaluate(parent_node->variables, this);
}

void Visitor::visit_expression(AST *parent_node, Expression *node) {
    if (node == nullptr)
        return;
    node->evaluate(parent_node->variables, this);
}

void Visitor::visit_value(AST *parent_node, ValueNode *node) {

    if (node == nullptr)
        return;
    node->evaluate(parent_node->variables, this);
}

void Visitor::visit_fcall(AST *parent_node, FunctionCallNode *node) {
    if (node == nullptr)
        return;
    node->evaluate(parent_node->variables, this);

}

void Visitor::visit_variable(AST *parent_node, VariableNode *node) {
    if (node == nullptr)
        return;
    node->evaluate(parent_node->variables, this);
}

void Visitor::visit_structure(AST *parent_node, Structure *node) {
    if (node == nullptr)
        return;    
    node->variables = parent_node->variables;

    if (node->type == "if") {
        variant_t exp_value = node->expression->evaluate(node->variables, this);
        if ((std::holds_alternative<int>(exp_value) && std::get<int>(exp_value)) ||
            (std::holds_alternative<double>(exp_value) && std::get<double>(exp_value))) {
            for (auto structline : node->nodes)
                structline->accept(node, this);
            parent_node->variables = node->variables;
        }
    } else if (node->type == "while") {

        variant_t exp_value = node->expression->evaluate(node->variables, this);
        while ((std::holds_alternative<int>(exp_value) && std::get<int>(exp_value)) ||
            (std::holds_alternative<double>(exp_value) && std::get<double>(exp_value))) {
            for (auto structline : node->nodes)
                structline->accept(node, this);
            parent_node->variables = node->variables;
            exp_value = node->expression->evaluate(node->variables, this);
        }

    } else {
        throw std::runtime_error("Error: Unknown structure type");
    }
}

void Visitor::visit_function(AST *parent_node, Function *node) {
    if (node == nullptr)
        return;
    parent_node->variables[node->name] = node;
}
