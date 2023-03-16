/**
 ** \file ast/object-visitor.hxx
 ** \brief Implementation for ast/object-visitor.hh.
 */

#pragma once

#include <ast/all.hh>
#include <ast/object-visitor.hh>
#include <misc/contract.hh>

namespace ast
{
  template <template <typename> class Const>
  GenObjectVisitor<Const>::GenObjectVisitor()
    : GenVisitor<Const>()
  {}

  template <template <typename> class Const>
  GenObjectVisitor<Const>::~GenObjectVisitor()
  {}

  /*-------------------------------.
  | Object-related visit methods.  |
  `-------------------------------*/

  template <template <typename> class Const>
  void GenObjectVisitor<Const>::operator()(const_t<ClassTy>& e)
  {
    e.super_get().accept(*this);
    e.chunks_get().accept(*this);
  }

  template <template <typename> class Const>
  void GenObjectVisitor<Const>::operator()(const_t<MethodChunk>& e)
  {
    for (auto& c : e.decs_get())
      c->accept(*this);
  }

  template <template <typename> class Const>
  void GenObjectVisitor<Const>::operator()(const_t<MethodDec>& e)
  {
    for (auto& v : e.formals_get().decs_get())
      v->accept(*this);
    if (e.result_get())
      e.result_get()->accept(*this);
    if (e.body_get())
      e.body_get()->accept(*this);
  }

  template <template <typename> class Const>
  void GenObjectVisitor<Const>::operator()(const_t<MethodCallExp>& e)
  {
    e.get_var().accept(*this);
    for (auto& v : e.get_seq())
      v->accept(*this);
  }

  template <template <typename> class Const>
  void GenObjectVisitor<Const>::operator()(const_t<ObjectExp>& e)
  {
    e.get_type().accept(*this);
  }

} // namespace ast
