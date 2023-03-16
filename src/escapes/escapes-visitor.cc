/**
 ** \file escapes/escapes-visitor.cc
 ** \brief Implementation for escapes/escapes-visitor.hh.
 */

#include <iostream>
#include <ast/all.hh>
#include <escapes/escapes-visitor.hh>
#include <misc/contract.hh>

namespace escapes
{
  void EscapesVisitor::operator()(ast::VarDec& e)
  {
    put(&e);
    e.set_is_escaped(false);
    if (e.type_name_get())
      e.type_name_get()->accept(*this);
    if (e.init_get())
      e.init_get()->accept(*this);
  }

  void EscapesVisitor::operator()(ast::FunctionDec& e)
  {
    scope_begin();
    e.formals_get().accept(*this);
    if (e.body_get())
      e.body_get()->accept(*this);
    if (e.result_get())
      e.result_get()->accept(*this);
    scope_end();
  }

  void EscapesVisitor::operator()(ast::SimpleVar& e)
  {
    auto d = get_var(e.name_get());
    if (d)
      d->set_is_escaped(true);
  }

  void EscapesVisitor::scope_begin() { var_.scope_begin(); }

  void EscapesVisitor::scope_end() { var_.scope_end(); }

  void EscapesVisitor::put(ast::VarDec* var) { var_.put(var->name_get(), var); }

} // namespace escapes