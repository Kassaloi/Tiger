/**
 ** \file ast/record-exp.hh
 ** \brief Declaration of ast::RecordExp.
 */

#pragma once

#include <ast/exp.hh>
#include <ast/field-init.hh>
#include <ast/name-ty.hh>

namespace ast
{
  /// RecordExp.
  class RecordExp : public Exp
  {
    // FIXED
  public:
    RecordExp(const Location& location, NameTy* type, fieldinits_type* field);
    virtual ~RecordExp();

    void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;

    const NameTy& get_type() const;
    NameTy& get_type();

    const fieldinits_type& get_field() const;
    fieldinits_type& get_field();

  protected:
    NameTy* type_;
    fieldinits_type* field_;
  };
} // namespace ast
#include <ast/record-exp.hxx>
