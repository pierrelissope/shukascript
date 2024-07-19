#ifndef VISITOR_H

class Value;
class Expression;
class Assignement;
class Structure;
class Function;

class Visitor {
    public:
        Visitor() = default;
        ~Visitor() = default;



        void visit_value(Value *node) {
            
        }

        void visit_expression(Expression *node) {

        }

        void visit_assignement(Assignement *node) {
            
        }

        void visit_structure(Structure *node) {
            
        }

        void visit_function(Function *node) {

        }
};

#define VISITOR_H


#endif /* !VISITOR_H */