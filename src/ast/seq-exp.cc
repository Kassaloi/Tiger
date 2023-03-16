/**
 ** \file ast/seq-exp.cc
 ** \brief Implementation of ast::SeqExp.
 */

#include <ast/seq-exp.hh>
#include <ast/visitor.hh>
#include <misc/algorithm.hh>

namespace ast
{
  // FIXED
  SeqExp::SeqExp(const Location& location, exps_type* seq)
    : Exp(location)
    , seq_(seq)
  {}

  SeqExp::~SeqExp()
  {
    for (auto s : *seq_)
      delete s;
    delete seq_;
  }

  void SeqExp::accept(ConstVisitor& v) const { v(*this); }
  void SeqExp::accept(Visitor& v) { v(*this); }

} // namespace ast
