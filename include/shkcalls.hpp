#ifndef SHKCALLS_HPP_
#define SHKCALLS_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "shkvariant.hpp"

class Expression;
class AST;
class Visitor;
class Function;

typedef void (*syscallo)(std::vector<std::shared_ptr<Expression>> args, std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v);

bool is_all_digits(const std::string& str);
int call_sysfunctions(std::string name, std::vector<std::shared_ptr<Expression>> args, std::shared_ptr<AST> parent_node, std::shared_ptr<Visitor> v);

#endif /* !SHKCALLS_HPP_ */
