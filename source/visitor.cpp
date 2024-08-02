#include "visitor.hpp"
#include "ast.hpp"

using namespace std;

void Visitor::visit_assignement(AST *parent_node, Assignement *node) {
    if (node == nullptr)
        return;
    cout << "Visiting Assignement: " << node->var_name << " <- Expression juste apres: \n\n" ;

    parent_node->variables[node->var_name] = node->expression->evaluate(parent_node->variables, this);
}

void Visitor::visit_expression(AST *parent_node, Expression *node) {
    if (node == nullptr)
        return;
    cout << "Visiting Expression: ";
    cout << "\n";
    node->evaluate(parent_node->variables, this);
}

void Visitor::visit_value(AST *parent_node, ValueNode *node) {

    if (node == nullptr)
        return;
    if (holds_alternative<int>(node->value)) {
        int value = get<int>(node->value);
        std::cout << "The variant contains an int with value: " << value << '\n';
    }
    if (holds_alternative<double>(node->value)) {
        double value = get<double>(node->value);
        std::cout << "The variant contains an float with value: " << value << '\n';
    }
    if (holds_alternative<string>(node->value)) {
        string value = get<string>(node->value);
        std::cout << "The variant contains an string with value: " << value << '\n';
    }
    node->evaluate(parent_node->variables, this);
}

void Visitor::visit_fcall(AST *parent_node, FunctionCallNode *node) {
    if (node == nullptr)
        return;
    cout << "Visiting fcall: " << node->name << endl;
    node->evaluate(parent_node->variables, this);

}

void Visitor::visit_variable(AST *parent_node, VariableNode *node) {
    if (node == nullptr)
        return;
    cout << "Visiting variable: " << node->name << endl;
    node->evaluate(parent_node->variables, this);
}




void Visitor::visit_structure(AST *parent_node, Structure *node) {
    if (node == nullptr)
        return;
    cout << "Visiting Structure, type: " << node->type << " Expression: ";
    
    node->variables = parent_node->variables;

    if (node->type == "if") {
        variant_t exp_value = node->expression->evaluate(node->variables, this);
        if ((std::holds_alternative<int>(exp_value) && std::get<int>(exp_value)) ||
            (std::holds_alternative<double>(exp_value) && std::get<double>(exp_value))) {
            for (auto structline : node->nodes)
                structline->accept(node, this);
            parent_node->variables = node->variables;
            printf("End IF");
        }
    } else if (node->type == "while") {

        variant_t exp_value = node->expression->evaluate(node->variables, this);
        while ((std::holds_alternative<int>(exp_value) && std::get<int>(exp_value)) ||
            (std::holds_alternative<double>(exp_value) && std::get<double>(exp_value))) {
            for (auto structline : node->nodes)
                structline->accept(node, this);
            parent_node->variables = node->variables;
            exp_value = node->expression->evaluate(node->variables, this);
            printf("Loop cycle\n");
        }

    } else {
        throw std::runtime_error("Error: Unknown structure type");
    }
}

void Visitor::visit_function(AST *parent_node, Function *node) {
    if (node == nullptr)
        return;
    cout << "Visiting Function, name: " << node->name << endl;

    parent_node->variables[node->name] = node;
}
