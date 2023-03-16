/**
 ** \file bind/binder.hxx
 ** \brief Inline methods of bind::Binder.
 **/

#pragma once

#include <bind/binder.hh>

namespace bind
{
  /*-----------------.
  | Error handling.  |
  `-----------------*/

  template <typename T>
  void Binder::undeclared(const std::string& k, const T& e)
  {
    error_ << misc::error::error_type::bind << e.location_get()
           << ": undeclared " << k << ": " << e.name_get() << std::endl;
  }

  template <typename T> void Binder::redefinition(const T& e1, const T& e2)
  {
    error_ << misc::error::error_type::bind << e2.location_get()
           << "redefinition: " << e2.name_get() << std::endl;
    error_ << e1.location_get() << "first def" << std::endl;
  }

  /*----------------------------.
  | Visiting /ChunkInterface/.  |
  `----------------------------*/

  template <class D> void Binder::chunk_visit(ast::Chunk<D>& e)
  {
    auto& chunk = e.decs_get();

    size_t i = 0;
    while (i < chunk.size())
      {
        size_t j = 0;
        while (j < i)
          {
            auto a = chunk[j]->name_get().get();
            auto b = chunk[i]->name_get().get();
            if (a == b)
              {
                redefinition(*chunk[j], *chunk[i]);
                break;
              }
            j++;
          }
        visit_dec_header<D>(*chunk[i]);
        i++;
      }
    for (auto& x : e.decs_get())
      visit_dec_body(*x);
  }

  /* These specializations are in bind/binder.hxx, so that derived
     visitors can use them (otherwise, they wouldn't see them).  */

  template <>
  inline void Binder::visit_dec_header<ast::TypeDec>(ast::TypeDec& e)
  {
    t_scope_.put(e.name_get(), &e);
  }

  template <>
  inline void Binder::visit_dec_header<ast::FunctionDec>(ast::FunctionDec& e)
  {
    f_scope_.put(e.name_get(), &e);
  }

  template <> inline void Binder::visit_dec_header<ast::VarDec>(ast::VarDec& e)
  {
    v_scope_.put(e.name_get(), &e);
  }

  template <> inline void Binder::visit_dec_body<ast::TypeDec>(ast::TypeDec& e)
  {
    scope_begin();
    e.ty_get().accept(*this);
    scope_end();
  }

  template <>
  inline void Binder::visit_dec_body<ast::FunctionDec>(ast::FunctionDec& e)
  {
    scope_begin();
    auto& r = e.formals_get();
    chunk_visit<ast::VarDec>(r);
    if (e.result_get())
      e.result_get()->accept(*this);
    if (e.body_get())
      e.body_get()->accept(*this);
    scope_end();
  }

  template <> inline void Binder::visit_dec_body<ast::VarDec>(ast::VarDec& e)
  {
    scope_begin();
    if (e.type_name_get())
      e.type_name_get()->accept(*this);
    if (e.init_get())
      e.init_get()->accept(*this);
    scope_end();
  }

} // namespace bind
