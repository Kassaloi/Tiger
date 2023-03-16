/**
 ** \file ast/seq-exp.hxx
 ** \brief Inline methods of ast::SeqExp.
 */

#pragma once

#include <ast/seq-exp.hh>

namespace ast
{
  // FIXED
  inline const exps_type& SeqExp::get_seq() const { return *seq_; }
  inline exps_type& SeqExp::get_seq() { return *seq_; }
} // namespace ast
