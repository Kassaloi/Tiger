/**
 ** \file ast/method-call-exp.cc
 ** \brief Implementation of ast::MethodCallExp.
 */

#include <ast/method-call-exp.hh>
#include <ast/visitor.hh>

namespace ast
{
  // FIXED
  MethodCallExp::MethodCallExp(const Location& location,
                               const misc::symbol& name,
                               ast::exps_type* seq,
                               Exp* var)
    : CallExp(location, name, seq)
    , var_(var)
  {}

  MethodCallExp::~MethodCallExp() { delete var_; }

  void MethodCallExp::accept(ConstVisitor& v) const { v(*this); }
  void MethodCallExp::accept(Visitor& v) { v(*this); }

} // namespace ast
