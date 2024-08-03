#ifndef VISITOR_H

#include <iostream>
#include <string>
#include <memory>

#include "shkvariant.hpp"

class AST;

class ValueNode;
class FunctionCallNode;
class VariableNode;

class Expression;
class Assignement;
class Structure;
class Function;

class Visitor : public std::enable_shared_from_this<Visitor> {
    public:
        Visitor() = default;
        ~Visitor() = default;

        variant_t ret_value;

        void visit_value(std::shared_ptr<AST> parent_node, std::shared_ptr<ValueNode> node);
        void visit_fcall(std::shared_ptr<AST> parent_node, std::shared_ptr<FunctionCallNode> node);
        void visit_variable(std::shared_ptr<AST> parent_node, std::shared_ptr<VariableNode> node);

        void visit_expression(std::shared_ptr<AST> parent_node, std::shared_ptr<Expression> node);
        void visit_assignement(std::shared_ptr<AST> parent_node, std::shared_ptr<Assignement> node);
        void visit_structure(std::shared_ptr<AST> parent_node, std::shared_ptr<Structure> node);
        void visit_function(std::shared_ptr<AST> parent_node, std::shared_ptr<Function> node);
};

#define VISITOR_H


#endif /* !VISITOR_H */