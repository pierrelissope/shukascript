#ifndef SHKCALLS_HPP_
#define SHKCALLS_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "shkvariant.hpp"

class Expression;
class AST;
class Visitor;
class Function;

typedef void (*syscallo)(std::vector<Expression *> args, AST *parent_node, Visitor *v);

bool is_all_digits(const std::string& str);
int call_sysfunctions(std::string name, std::vector<Expression *> args, AST *parent_node, Visitor *v);

#endif /* !SHKCALLS_HPP_ */
