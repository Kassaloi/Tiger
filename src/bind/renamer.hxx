/**
 ** \file bind/renamer.hxx
 ** \brief Template methods of bind::Renamer.
 */

#pragma once

#include <bind/renamer.hh>

namespace bind
{
  template <typename Def> misc::symbol Renamer::new_name_compute(const Def& e)
  {
    return e.name_get().fresh(e.name_get());
  }

  template <typename Def> misc::symbol Renamer::new_name(const Def& e)
  {
    return new_name_compute(e);
  }

  inline misc::symbol Renamer::new_name(const ast::FunctionDec& e)
  {
    if (e.body_get() && e.name_get().get() != "_main")
      return new_name_compute(e);
    else
      return e.name_get();
  }

  template <class E, class Def> void Renamer::visit(E& e, const Def* def) {}

} // namespace bind
