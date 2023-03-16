/**
 ** \file ast/field-var.hh
 ** \brief Declaration of ast::FieldVar.
 */

#pragma once

#include <ast/var.hh>
#include <misc/symbol.hh>

namespace ast
{
  /// FieldVar.
  class FieldVar : public Var
  {

  public:
    FieldVar(const Location& location, Var* var, misc::symbol name);
    virtual ~FieldVar();

    void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;

    const Var& get_var() const;
    Var& get_var();

    const misc::symbol get_name() const;
    misc::symbol get_name();

  protected:
    Var* var_;
    misc::symbol name_;
  };

} // namespace ast
#include <ast/field-var.hxx>
