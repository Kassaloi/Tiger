/**
 ** \file ast/method-call-exp.hxx
 ** \brief Inline methods of ast::MethodCallExp.
 */

#pragma once

#include <ast/method-call-exp.hh>

namespace ast
{
  // FIXED
  inline const Exp& MethodCallExp::get_var() const { return *var_; }
  inline Exp& MethodCallExp::get_var() { return *var_; }

} // namespace ast
