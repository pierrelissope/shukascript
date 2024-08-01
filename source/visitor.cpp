#include "visitor.hpp"
#include "ast.hpp"

using namespace std;

void Visitor::visit_value(ValueNode *node) {

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
}

void Visitor::visit_fcall(FunctionCallNode *node) {
    if (node == nullptr)
        return;
    cout << "Visiting fcall: " << node->name << endl;
}

void Visitor::visit_variable(VariableNode *node) {
    if (node == nullptr)
        return;
    cout << "Visiting variable: " << node->name << endl;
}

void Visitor::visit_expression(Expression *node) {
    if (node == nullptr)
        return;
    cout << "Visiting Expression: ";
    cout << "\n";
    node->left->accept(this);
    node->right->accept(this);
}

void Visitor::visit_assignement(Assignement *node) {
    if (node == nullptr)
        return;
    cout << "Visiting Assignement: " << node->var_name << " <- Expression juste apres: " ;
    node->expression->accept(this);
}

void Visitor::visit_structure(Structure *node) {
    if (node == nullptr)
        return;
    cout << "Visiting Structure, type: " << node->type << " Expression: ";
    node->expression->accept(this);
    for (auto chose : node->nodes)
       chose->accept(this);
}

void Visitor::visit_function(Function *node) {
    if (node == nullptr)
        return;
    cout << "Visiting Function, name: " << node->name << endl;
}