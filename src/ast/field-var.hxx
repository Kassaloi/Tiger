/**
 ** \file ast/field-var.hxx
 ** \brief Inline methods of ast::FieldVar.
 */

#pragma once

#include <ast/field-var.hh>

namespace ast
{

  inline const Var& FieldVar::get_var() const { return *var_; }
  inline Var& FieldVar::get_var() { return *var_; }

  inline const misc::symbol FieldVar::get_name() const { return name_; }
  inline misc::symbol FieldVar::get_name() { return name_; }

} // namespace ast
