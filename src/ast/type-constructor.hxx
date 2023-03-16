/**
 ** \file ast/type-constructor.hxx
 ** \brief Inline methods of ast::TypeConstructor.
 */

#pragma once

#include <ast/type-constructor.hh>
#include <type/types.hh>

namespace ast
{
  // FIXED
  inline void TypeConstructor::set_created_type(const type::Type* e)
  {
    type_ = e;
  }
  inline const type::Type* TypeConstructor::get_created_type() const
  {
    return type_;
  }
} // namespace ast
