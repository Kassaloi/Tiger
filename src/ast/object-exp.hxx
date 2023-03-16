/**
 ** \file ast/object-exp.hxx
 ** \brief Inline methods of ast::ObjectExp.
 */

#pragma once

#include <ast/object-exp.hh>

namespace ast
{
  // FIXED
  inline const NameTy& ObjectExp::get_type() const { return *type_; }
  inline NameTy& ObjectExp::get_type() { return *type_; }
} // namespace ast
