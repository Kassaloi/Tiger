/**
 ** \file ast/object-exp.hh
 ** \brief Declaration of ast::ObjectExp.
 */

#pragma once

#include <ast/exp.hh>
#include <ast/name-ty.hh>

namespace ast
{
  /// ObjectExp.
  class ObjectExp : public Exp
  {
    // FIXED
  public:
    ObjectExp(const Location& location, NameTy* type);
    virtual ~ObjectExp();

    void accept(ConstVisitor&) const override;
    void accept(Visitor&) override;

    const NameTy& get_type() const;
    NameTy& get_type();

  protected:
    NameTy* type_;
  };
} // namespace ast
#include <ast/object-exp.hxx>
