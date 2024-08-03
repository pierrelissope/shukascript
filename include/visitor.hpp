#ifndef VISITOR_H

#include <iostream>
#include <string>

#include "shkvariant.hpp"

class AST;

class ValueNode;
class FunctionCallNode;
class VariableNode;

class Expression;
class Assignement;
class Structure;
class Function;

class Visitor {
    public:
        Visitor() = default;
        ~Visitor() = default;

        variant_t ret_value;

        void visit_value(AST *parent_node, ValueNode *node);
        void visit_fcall(AST *parent_node, FunctionCallNode *node);
        void visit_variable(AST *parent_node, VariableNode *node);

        void visit_expression(AST *parent_node, Expression *node);
        void visit_assignement(AST *parent_node, Assignement *node);
        void visit_structure(AST *parent_node, Structure *node);
        void visit_function(AST *parent_node, Function *node);
};

#define VISITOR_H


#endif /* !VISITOR_H */