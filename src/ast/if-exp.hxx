/**
 ** \file ast/if-exp.hxx
 ** \brief Inline methods of ast::IfExp.
 */

#pragma once

#include <ast/if-exp.hh>

namespace ast
{
  // FIXED
  inline const Exp& IfExp::get_exp() const { return *exp_; }
  inline Exp& IfExp::get_exp() { return *exp_; }

  inline const Exp& IfExp::get_then() const { return *Ethen_; }
  inline Exp& IfExp::get_then() { return *Ethen_; }

  inline const Exp* IfExp::get_else() const { return Eelse_; }
  inline Exp* IfExp::get_else() { return Eelse_; }
} // namespace ast
