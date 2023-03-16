/**
 ** \file ast/object-exp.cc
 ** \brief Implementation of ast::ObjectExp.
 */

#include <ast/object-exp.hh>
#include <ast/visitor.hh>

namespace ast
{
  // FIXED
  ObjectExp::ObjectExp(const Location& location, NameTy* type)
    : Exp(location)
    , type_(type)
  {}

  ObjectExp::~ObjectExp() { delete type_; }

  void ObjectExp::accept(ConstVisitor& v) const { v(*this); }
  void ObjectExp::accept(Visitor& v) { v(*this); }
} // namespace ast
