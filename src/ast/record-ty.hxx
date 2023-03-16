/**
 ** \file ast/record-ty.hxx
 ** \brief Inline methods of ast::RecordTy.
 */

#pragma once

#include <ast/record-ty.hh>

namespace ast
{
  // FIXED
  inline const fields_type& RecordTy::get_field() const { return *field_; }
  inline fields_type& RecordTy::get_field() { return *field_; }
} // namespace ast
