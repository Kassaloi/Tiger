/**
 ** \file ast/let-exp.hxx
 ** \brief Inline methods of ast::LetExp.
 */

#pragma once

#include <ast/let-exp.hh>

namespace ast
{
  // FIXED
  inline const ChunkList& LetExp::get_chunk() const { return *chunk_; }
  inline ChunkList& LetExp::get_chunk() { return *chunk_; }

  inline const Exp& LetExp::get_exp() const { return *exp_; }
  inline Exp& LetExp::get_exp() { return *exp_; }

} // namespace ast
