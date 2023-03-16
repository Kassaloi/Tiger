/**
 ** \file ast/array-exp.cc
 ** \brief Implementation of ast::ArrayExp.
 */

#include <ast/array-exp.hh>
#include <ast/visitor.hh>

namespace ast
{
  ArrayExp::ArrayExp(const Location& location, NameTy* type, Exp* lhs, Exp* rhs)
    : Exp(location)
    , type_(type)
    , lhs_(lhs)
    , rhs_(rhs)
  {}

  ArrayExp::~ArrayExp()
  {
    delete type_;
    delete lhs_;
    delete rhs_;
  }

  void ArrayExp::accept(ConstVisitor& v) const { v(*this); }
  void ArrayExp::accept(Visitor& v) { v(*this); }

} // namespace ast
