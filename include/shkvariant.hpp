#ifndef SHKVARIANT_HPP_
#define SHKVARIANT_HPP_

#include <variant>
#include <string>
#include <memory>

class Function;

typedef std::variant<int, double, std::string, std::shared_ptr<Function>> variant_t;

#endif /* !SHKVARIANT_HPP_ */
