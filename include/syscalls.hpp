#ifndef SYSCALLS_HPP_
#define SYSCALLS_HPP_

#include <string>
#include <vector>
#include <map>

class Expression;
class Visitor;

typedef void (*syscall)(std::string name, std::vector<Expression *> args, Visitor *v);
 
int call_sysfunctions(std::string name, std::vector<Expression *> args, Visitor *v)

#endif /* !SYSCALLS_HPP_ */
