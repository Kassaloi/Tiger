/**
 ** \file ast/string-exp.cc
 ** \brief Implementation of ast::StringExp.
 */

#include <ast/string-exp.hh>
#include <ast/visitor.hh>

namespace ast
{
  StringExp::StringExp(const Location& location, std::string str)
    : Exp(location)
    , str_(str)
  {}

  void StringExp::accept(ConstVisitor& v) const { v(*this); }
  void StringExp::accept(Visitor& v) { v(*this); }
} // namespace ast
