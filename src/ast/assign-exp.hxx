/**
 ** \file ast/assign-exp.hxx
 ** \brief Inline methods of ast::AssignExp.
 */

#pragma once

#include <ast/assign-exp.hh>

namespace ast
{
  inline const Var& AssignExp::get_var() const { return *var_; }
  inline Var& AssignExp::get_var() { return *var_; }

  inline const Exp& AssignExp::get_exp() const { return *exp_; }
  inline Exp& AssignExp::get_exp() { return *exp_; }

} // namespace ast
