#ifndef VISITOR_H

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

        void visit_value(ValueNode *node);
        void visit_fcall(FunctionCallNode *node);
        void visit_variable(VariableNode *node);

        void visit_expression(Expression *node);
        void visit_assignement(Assignement *node);
        void visit_structure(Structure *node);
        void visit_function(Function *node);
};

#define VISITOR_H


#endif /* !VISITOR_H */