/**
 ** \file ast/if-exp.cc
 ** \brief Implementation of ast::IfExp.
 */

#include <ast/if-exp.hh>
#include <ast/visitor.hh>

namespace ast
{
  // FIXED
  IfExp::IfExp(const Location& location, Exp* exp, Exp* Ethen, Exp* Eelse)
    : Exp(location)
    , exp_(exp)
    , Ethen_(Ethen)
    , Eelse_(Eelse)
  {}

  IfExp::~IfExp()
  {
    delete exp_;
    delete Ethen_;
    if (Eelse_)
      delete Eelse_;
  }

  void IfExp::accept(ConstVisitor& v) const { v(*this); }
  void IfExp::accept(Visitor& v) { v(*this); }

} // namespace ast
