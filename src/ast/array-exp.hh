/**
 ** \file ast/array-exp.hh
 ** \brief Declaration of ast::ArrayExp.
 */

#pragma once

#include <ast/exp.hh>
#include <ast/name-ty.hh>

namespace ast
{
  /// ArrayExp.
  class ArrayExp : public Exp
  {
  public:
    ArrayExp(const Location& location, NameTy* type, Exp* lhs, Exp* rhs);
    virtual ~ArrayExp();

    void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;

    const NameTy& get_type() const;
    NameTy& get_type();

    const Exp& get_lhs() const;
    Exp& get_lhs();

    const Exp& get_rhs() const;
    Exp& get_rhs();

    const VarDec* get_def() const;
    VarDec* get_def();

    void set_def(VarDec*);

  protected:
    NameTy* type_;
    Exp* lhs_;
    Exp* rhs_;
    VarDec* def_ = nullptr;
  };
} // namespace ast
#include <ast/array-exp.hxx>
