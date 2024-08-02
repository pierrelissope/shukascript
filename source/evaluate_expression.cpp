#include "syscalls.hpp"
#include "ast.hpp"
#include "visitor.hpp"

variant_t Expression::evaluate(std::map<std::string, variant_t> variables, Visitor *v)
{
    if (!op.empty() && left && right) {
        auto left_val = left->evaluate(variables, v);
        auto right_val = right->evaluate(variables, v);

        return std::visit([this](auto&& l, auto&& r) -> variant_t {
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

        if (call_sysfunctions(this->name, this->args, v))
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
