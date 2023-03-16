/**
 ** \file ast/call-exp.hxx
 ** \brief Inline methods of ast::CallExp.
 */

#pragma once

#include <ast/call-exp.hh>

namespace ast
{
  inline const exps_type& CallExp::get_seq() const { return *seq_; }
  inline exps_type& CallExp::get_seq() { return *seq_; }

  inline const misc::symbol& CallExp::name_get() const { return name_; }
  inline misc::symbol& CallExp::name_get() { return name_; }
  inline void CallExp::set_name(const misc::symbol& symbol) { name_ = symbol; }

  inline const FunctionDec* CallExp::get_func() const { return func_; }
  inline FunctionDec* CallExp::get_func() { return func_; }
  inline void CallExp::set_func(FunctionDec* def) { func_ = def; }
} // namespace ast
