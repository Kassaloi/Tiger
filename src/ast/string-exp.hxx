/**
 ** \file ast/string-exp.hxx
 ** \brief Inline methods of ast::StringExp.
 */

#pragma once

#include <ast/string-exp.hh>

namespace ast
{
  const inline std::string StringExp::get_var() const { return str_; }
  inline std::string StringExp::get_var() { return str_; }

} // namespace ast
