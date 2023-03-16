/**
 ** \file ast/record-exp.cc
 ** \brief Implementation of ast::RecordExp.
 */

#include <ast/record-exp.hh>
#include <ast/visitor.hh>
#include <misc/algorithm.hh>

namespace ast
{
  // FIXED
  RecordExp::RecordExp(const Location& location,
                       NameTy* type,
                       fieldinits_type* field)
    : Exp(location)
    , type_(type)
    , field_(field)
  {}

  RecordExp::~RecordExp() {}

  void RecordExp::accept(ConstVisitor& v) const { v(*this); }
  void RecordExp::accept(Visitor& v) { v(*this); }

} // namespace ast
