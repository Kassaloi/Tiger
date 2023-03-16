/**
 ** \file ast/type-constructor.hh
 ** \brief Declaration of ast::TypeConstructor.
 */

#pragma once

#include <ast/fwd.hh>
#include <type/fwd.hh>

namespace ast
{
  /** \class ast::TypeConstructor
   ** \brief Create a new type.
   */

  class TypeConstructor
  {
  public:
    TypeConstructor();
    TypeConstructor(const type::Type* e);
    virtual ~TypeConstructor();

    void set_created_type(const type::Type*);
    const type::Type* get_created_type() const;

    virtual void accept(ConstVisitor& v) const = 0;
    virtual void accept(Visitor& v) = 0;

  private:
    const type::Type* type_ = nullptr;
  };
} // namespace ast
#include <ast/type-constructor.hxx>
