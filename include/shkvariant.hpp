#ifndef SHKVARIANT_HPP_
#define SHKVARIANT_HPP_

#include <variant>
#include <string>

class Function;

typedef std::variant<int, double, std::string, Function *> variant_t;

#endif /* !SHKVARIANT_HPP_ */
