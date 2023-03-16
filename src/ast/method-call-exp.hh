/**
 ** \file ast/method-call-exp.hh
 ** \brief Declaration of ast::MethodCallExp.
 */

#pragma once

#include <ast/call-exp.hh>
#include <ast/method-dec.hh>
#include <ast/var.hh>

namespace ast
{
  /** \class ast::MethodCallExp
   ** \brief Method call.
   **
   ** A method call is \em not a function call in the strict sense
   ** of object-oriented programming.  Inheritance is used as a
   ** factoring tool here.
   */

  class MethodCallExp : public CallExp
  {
    // FIXED
  public:
    MethodCallExp(const Location& location,
                  const misc::symbol& name,
                  ast::exps_type* seq,
                  Exp* var);
    virtual ~MethodCallExp();

    void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;

    const Exp& get_var() const;
    Exp& get_var();

  protected:
    Exp* var_;
  };
} // namespace ast
#include <ast/method-call-exp.hxx>
