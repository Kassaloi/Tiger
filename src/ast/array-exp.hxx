/**
 ** \file ast/array-exp.hxx
 ** \brief Inline methods of ast::ArrayExp.
 */

#pragma once

#include <ast/array-exp.hh>

namespace ast
{
  inline const NameTy& ArrayExp::get_type() const { return *type_; }
  inline NameTy& ArrayExp::get_type() { return *type_; }

  inline const Exp& ArrayExp::get_lhs() const { return *lhs_; }
  inline Exp& ArrayExp::get_lhs() { return *lhs_; }

  inline const Exp& ArrayExp::get_rhs() const { return *rhs_; }
  inline Exp& ArrayExp::get_rhs() { return *rhs_; }

  inline const VarDec* ArrayExp::get_def() const { return def_; }
  inline VarDec* ArrayExp::get_def() { return def_; }

  inline void ArrayExp::set_def(VarDec* def) { def_ = def; }
} // namespace ast
