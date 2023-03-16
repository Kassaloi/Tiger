/**
 ** \file ast/break-exp.hxx
 ** \brief Inline methods of ast::BreakExp.
 */

#pragma once

#include <ast/break-exp.hh>

// Hint: this needs to be done at TC-3.

namespace ast
{
  inline const Exp* BreakExp::get_loop() const { return loop_; }
  inline Exp* BreakExp::get_loop() { return loop_; }
  inline void BreakExp::set_loop(Exp* loop) { this->loop_ = loop; }

} // namespace ast
