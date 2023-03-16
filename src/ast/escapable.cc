/**
 ** \file ast/escapable.cc
 ** \brief Implementation of ast::Escapable.
 */

#include <ast/escapable.hh>
#include <ast/visitor.hh>

namespace ast
{
  // FIXED
  Escapable::Escapable() { is_escaped_ = true; }
  Escapable::~Escapable(){};
} // namespace ast
