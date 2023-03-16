/**
 ** \file ast/type-constructor.cc
 ** \brief Implementation of ast::TypeConstructor.
 */

#include <ast/type-constructor.hh>
#include <ast/visitor.hh>

namespace ast
{
  // FIXED
  TypeConstructor::TypeConstructor()
    : type_(nullptr)
  {}
  TypeConstructor::TypeConstructor(const type::Type* e)
    : type_(e)
  {}

  TypeConstructor::~TypeConstructor()
  {
    if (type_)
      delete type_;
  }
} // namespace ast
