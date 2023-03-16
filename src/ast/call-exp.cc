/**
 ** \file ast/call-exp.cc
 ** \brief Implementation of ast::CallExp.
 */

#include <ast/call-exp.hh>
#include <ast/visitor.hh>
#include <misc/algorithm.hh>

namespace ast
{
  CallExp::CallExp(const Location& location, misc::symbol name, exps_type* seq)
    : Exp(location)
    , seq_(seq)
    , name_(name)
  {}

  CallExp::~CallExp()
  {
    for (auto a : *seq_)
      {
        delete a;
      }
    delete seq_;
  }

  void CallExp::accept(ConstVisitor& v) const { v(*this); }
  void CallExp::accept(Visitor& v) { v(*this); }

} // namespace ast
