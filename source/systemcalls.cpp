#include <map>
#include <random>
#include <cstdlib>
#include <ctime>

#include "shkcalls.hpp"
#include "ast.hpp"
#include "visitor.hpp"

void print(std::vector<std::shared_ptr<Expression>> args, std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v)
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

void returnsyscall(std::vector<std::shared_ptr<Expression>> args, std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v)
{
    for (auto element : args) {
        v->ret_value = element->evaluate(parent_node->variables, v);
        return;
    }
}

void randomsyscall(std::vector<std::shared_ptr<Expression>> args, std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v)
{
    if (args.size() != 2) {
        throw std::runtime_error("Funtion: Random require 2 arguments but " + std::to_string(args.size()) + " are given");
    }

    variant_t val1 = args[0]->evaluate(parent_node->variables, v);
    variant_t val2 = args[1]->evaluate(parent_node->variables, v);

    if (std::holds_alternative<int>(val1) && std::holds_alternative<int>(val2)) {
            int int_value1 = std::get<int>(val1);
            int int_value2 = std::get<int>(val2);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(int_value1, int_value2);

            int b = distrib(gen);;
            v->ret_value = b;
    } else {
        throw std::runtime_error("Funtion: Random require 2 numeric values");
    }
}

void exitsyscall(std::vector<std::shared_ptr<Expression>> args, std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v)
{
    for (auto element : args) {
        variant_t vvalue = element->evaluate(parent_node->variables, v);
        if (std::holds_alternative<int>(vvalue)) {
            int int_value = std::get<int>(vvalue);
            exit(int_value);
        }
    }
    exit(0);
}

int call_sysfunctions(std::string name, std::vector<std::shared_ptr<Expression>> args, std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v)
{
    static const std::map<std::string, syscallo> sys_fct_map = {
        {"print", print},
        {"return", returnsyscall},
        {"random", randomsyscall},
        {"exit", exitsyscall},
    };

    for (auto it = sys_fct_map.begin(); it != sys_fct_map.end(); ++it) {
        if ((*it).first == name) {
            (*it).second(args, parent_node, v);
            return 1;
        }
    }
    return 0;
}
