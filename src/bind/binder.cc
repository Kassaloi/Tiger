/**
 ** \file bind/binder.cc
 ** \brief Implementation for bind/binder.hh.
 */

#include <ast/all.hh>
#include <bind/binder.hh>

#include <misc/contract.hh>

namespace bind
{
  bool Binder::test = false;
  /*-----------------.
  | Error handling.  |
  `-----------------*/

  /// The error handler.
  const misc::error& Binder::error_get() const { return error_; }

  void Binder::break_outside_loop(ast::BreakExp& e)
  {
    error_ << misc::error::error_type::bind << e.location_get()
           << " bad break, outside loop" << std::endl;
  }

  void Binder::check_main(const ast::FunctionDec& e)
  {
    static int occ = 0;
    if (e.name_get() == "_main")
      {
        occ++;
      }
    if (occ >= 2)
      {
        error_ << misc::error::error_type::bind << e.location_get()
               << "main already defined" << std::endl;
      }
  }

  /*----------------.
  | Symbol tables.  |
  `----------------*/

  void Binder::scope_begin()
  {
    t_scope_.scope_begin();
    f_scope_.scope_begin();
    v_scope_.scope_begin();
  }

  void Binder::scope_end()
  {
    t_scope_.scope_end();
    f_scope_.scope_end();
    v_scope_.scope_end();
  }

  /*---------.
  | Visits.  |
  `---------*/

  Binder::Binder() { scope_begin(); }

  Binder::~Binder() { scope_end(); }

  void Binder::operator()(ast::LetExp& e)
  {
    scope_begin();
    e.get_chunk().accept(*this);
    e.get_exp().accept(*this);
    scope_end();
  }

  void Binder::operator()(ast::ForExp& e)
  {
    scope_begin();
    auto var = &(e.vardec_get());
    if (var)
      {
        v_scope_.put(var->name_get(), var);
      }
    e.vardec_get().accept(*this);
    e.hi_get().accept(*this);
    l_scope_.push(&e);
    e.body_get().accept(*this);
    l_scope_.pop();
    scope_end();
  }

  void Binder::operator()(ast::WhileExp& e)
  {
    scope_begin();
    Binder::test = true;
    e.test_get().accept(*this);
    Binder::test = false;
    l_scope_.push(&e);
    e.body_get().accept(*this);
    l_scope_.pop();
    scope_end();
  }

  void Binder::operator()(ast::BreakExp& e)
  {
    if (!Binder::test && !l_scope_.empty())
      e.set_loop(l_scope_.top());
    else
      break_outside_loop(e);
  }

  void Binder::operator()(ast::CallExp& e)
  {
    if (f_scope_.get(e.name_get()))
      {
        e.set_func(f_scope_.get(e.name_get()));
        size_t i = 0;
        while (i < e.get_seq().size())
          {
            e.get_seq()[i]->accept(*this);
            i++;
          }
        return;
      }
    undeclared("function", e);
  }

  void Binder::operator()(ast::SimpleVar& e)
  {
    if (v_scope_.get(e.name_get()))
      {
        e.def_set(v_scope_.get(e.name_get()));
        return;
      }
    undeclared("variable", e);
  }

  void Binder::operator()(ast::NameTy& e)
  {
    if (e.name_get().get() == "string" || e.name_get().get() == "int")
      {
        e.def_set(nullptr);
        return;
      }
    else if (t_scope_.get(e.name_get()))
      {
        e.def_set(t_scope_.get(e.name_get()));
        return;
      }
    undeclared("type", e);
  }

  void Binder::operator()(ast::ChunkList& e)
  {
    for (auto& i : e.chunks_get())
      i->accept(*this);
  }

  /*-------------------.
  | Visiting VarChunk. |
  `-------------------*/

  void Binder::operator()(ast::VarChunk& e)
  {
    Binder::test = true;
    chunk_visit<ast::VarDec>(e);
    Binder::test = false;
  }

  /*------------------------.
  | Visiting FunctionChunk. |
  `------------------------*/

  void Binder::operator()(ast::FunctionChunk& e)
  {
    Binder::test = true;
    chunk_visit<ast::FunctionDec>(e);
    Binder::test = false;
  }

  /*--------------------.
  | Visiting TypeChunk. |
  `--------------------*/
  void Binder::operator()(ast::TypeChunk& e) { chunk_visit<ast::TypeDec>(e); }

} // namespace bind
