#include "ast.hpp"
#include "visitor.hpp"

typedef void (*syscallo)(std::string name, std::vector<Expression *> args, AST *parent_node, Visitor *v);

void print(std::string name, std::vector<Expression *> args, AST *parent_node, Visitor *v)
{
    for (auto element : args) {
        variant_t vvalue = element->evaluate(parent_node->variables, v);
        if (std::holds_alternative<int>(vvalue)) {
            int int_value = std::get<int>(vvalue);
            std::cout << int_value;
        } else if (std::holds_alternative<double>(vvalue)) {
            double double_value = std::get<double>(vvalue);
            std::cout << double_value;
        } else if (std::holds_alternative<std::string>(vvalue)) {
            std::string string_value = std::get<std::string>(vvalue);
            std::cout << string_value;
        } else {
            std::cout << "Unrecognized value" << std::endl;
        }
    }
    std::cout << std::endl;
}

void returnsyscall(std::string name, std::vector<Expression *> args, AST *parent_node, Visitor *v)
{
    for (auto element : args) {
        v->ret_value = element->evaluate(parent_node->variables, v);
        return;
    }
}

int call_sysfunctions(std::string name, std::vector<Expression *> args, AST *parent_node, Visitor *v)
{
    static const std::map<std::string, syscallo> sys_fct_map = {
        {"print", print},
        {"return", returnsyscall}
    };

    for (auto it = sys_fct_map.begin(); it != sys_fct_map.end(); ++it) {
        if ((*it).first == name) {
            (*it).second(name, args, parent_node, v);
            return 1;
        }
    }
    return 0;
}


variant_t Expression::evaluate(std::map<std::string, variant_t> variables, Visitor *v)
{    
    if (!op.empty() && left && right) {

        auto right_val = left->evaluate(variables, v);
        auto left_val = right->evaluate(variables, v);

        variant_t res = std::visit([this](auto&& l, auto&& r) -> variant_t {
            using LType = std::decay_t<decltype(l)>;
            using RType = std::decay_t<decltype(r)>;

            if constexpr (std::is_arithmetic_v<LType> && std::is_arithmetic_v<RType>) {
                if (this->op == "/") return l / r;
                if (this->op == "*") return l * r;
                if (this->op == "-") return l - r;
                if (this->op == "+") return l + r;
                if (this->op == ">") return l > r;
                if (this->op == "<") return l < r;
                if (this->op == ">=") return l >= r;
                if (this->op == "<=") return l <= r;
                if (this->op == "==") return l == r;
            } else {
                throw std::runtime_error("Invalid operation on non-arithmetic types");
            }
            throw std::runtime_error("Unhandled case in expression evaluation");
        }, left_val, right_val);
        return res;
    } else {
        throw std::runtime_error("Invalid operation on arithmetic types");
        return 1;
    }
}

variant_t ValueNode::evaluate(std::map<std::string, variant_t> variables, Visitor *v)
{
    return value;
}

variant_t FunctionCallNode::evaluate(std::map<std::string, variant_t> variables, Visitor *v) {

    if (variables.find(this->name) != variables.end()) {
        if (std::holds_alternative<Function *>(variables[this->name])) {
            auto functionNode = std::get<Function *>(variables[this->name]);
            if (functionNode->args.size() == this->args.size()) {
                functionNode->variables = variables;
                for (std::size_t i = 0; i < functionNode->args.size(); ++i)
                    functionNode->variables[functionNode->args[i]] = this->args[i]->evaluate(variables, v);
                
                for (auto fcallnode : functionNode->nodes)
                    fcallnode->accept(functionNode, v);
                return v->ret_value;
            } else {
                throw std::runtime_error("Function: " + this->name + " has an incorrect number of arguments");
            }
        } else {
            throw std::runtime_error("Function: " + this->name + " is incorrect");
        }
    } else {

        this->variables = variables;
        if (call_sysfunctions(this->name, this->args, this, v))
            return v->ret_value;

        throw std::runtime_error("Function: " + this->name + " is not defined");
        return {};
    }
}

variant_t VariableNode::evaluate(std::map<std::string, variant_t> variables, Visitor *v)
{
    if (variables.find(this->name) != variables.end())
        return variables[this->name];
    else {
        throw std::runtime_error("Variable: " + this->name + " is not defined");
        return 0;
    }
    return 0;
}
