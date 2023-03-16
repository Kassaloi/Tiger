/**
 ** \file ast/assign-exp.hh
 ** \brief Declaration of ast::AssignExp.
 */

#pragma once

#include <ast/exp.hh>
#include <ast/var.hh>

namespace ast
{
  /// AssignExp.
  class AssignExp : public Exp
  {
    public:
      AssignExp(const Location& location, Var* var, Exp* exp);
      virtual ~AssignExp();

      void accept(ConstVisitor& v) const override;
      void accept(Visitor& v) override;

      const Var& get_var() const;
      Var& get_var();

      const Exp& get_exp() const;
      Exp& get_exp();

  protected:
      Var* var_;
      Exp* exp_;
  };
} // namespace ast
#include <ast/assign-exp.hxx>
