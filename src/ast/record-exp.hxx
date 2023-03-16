/**
 ** \file ast/record-exp.hxx
 ** \brief Inline methods of ast::RecordExp.
 */

#pragma once

#include <ast/record-exp.hh>

namespace ast
{
  // FIXED
  inline const NameTy& RecordExp::get_type() const { return *type_; }
  inline NameTy& RecordExp::get_type() { return *type_; }

  inline const fieldinits_type& RecordExp::get_field() const { return *field_; }
  inline fieldinits_type& RecordExp::get_field() { return *field_; }
} // namespace ast
