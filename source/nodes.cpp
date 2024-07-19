#include "ast.hpp"
#include "visitor.hpp"

Assignement::Assignement(std::string var_type, std::string var_name, Expression *expression)
{

}

Expression::Expression(std::vector<token *> tokens)
{

}

Structure::Structure(std::string struct_type, Expression *expression, std::vector<token *>tokens)
{
    this->expression = expression;
}





void Assignement::accept(Visitor *v) {
    v->visit_assignement(this);
}

void Expression::accept(Visitor *v) {
    v->visit_expression(this);
}

void Value::accept(Visitor *v) {
    v->visit_value(this);
}

void Structure::accept(Visitor *v) {
    v->visit_structure(this);
}

void Function::accept(Visitor *v) {
    v->visit_function(this);
}
