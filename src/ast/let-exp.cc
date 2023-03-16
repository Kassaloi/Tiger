/**
 ** \file ast/let-exp.cc
 ** \brief Implementation of ast::LetExp.
 */

#include <ast/let-exp.hh>
#include <ast/visitor.hh>

namespace ast
{
  // FIXED
  LetExp::LetExp(const Location& location, ChunkList* chunk, Exp* exp)
    : Exp(location)
    , chunk_(chunk)
    , exp_(exp)
  {}

  LetExp::~LetExp()
  {
    delete chunk_;
    delete exp_;
  }
  void LetExp::accept(ConstVisitor& v) const { v(*this); }
  void LetExp::accept(Visitor& v) { v(*this); }

} // namespace ast
