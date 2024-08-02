#include <iostream>

#include "visitor.hpp"
#include "ast.hpp"
#include "syscalls.hpp"

using namespace std;

void print(std::string name, std::vector<Expression *> args, Visitor *v)
{
    for (auto element : args)
        cout << element;
    cout << endl;
}

int call_sysfunctions(string name, vector<Expression *> args, Visitor *v)
{
    static const map<string, syscall> sys_fct_map = {
        {"print", print}
    };

    for (auto it = sys_fct_map.begin(); it != sys_fct_map.end(); ++it) {
        if ((*it).first == name) {
            (*it).second(name, args, v);
            return 1;
        }
    }
}