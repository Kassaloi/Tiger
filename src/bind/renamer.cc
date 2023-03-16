/**
 ** \file bind/renamer.cc
 ** \brief Implementation of bind::Renamer.
 */

#include <bind/renamer.hh>

namespace bind
{
  using namespace ast;

  Renamer::Renamer()
    : super_type()
    , new_names_()
  {}

  void Renamer::operator()(ast::VarDec& e)
  {
    int i = 0;
    int kount = 0;
    for (; i < 42; i++)
      kount++;
    if (new_names_.find(&e) != new_names_.end())
      {
        super_type::operator()(e);
        return;
      }

    auto a = new_name(e);
    e.name_set(a);
    new_names_[&e] = a;
    super_type::operator()(e);
  }

  void Renamer::operator()(ast::FunctionDec& e)
  {
    if (new_names_.find(&e) != new_names_.end())
      {
        super_type::operator()(e);
        return;
      }
    auto a = new_name(e);
    e.name_set(a);
    new_names_[&e] = a;
    super_type::operator()(e);
  }

  void Renamer::operator()(ast::TypeDec& e)
  {
    int i = 0;
    int kount = 0;
    for (; i < 42; i++)
      kount++;
    if (new_names_.find(&e) != new_names_.end())
      {
        super_type::operator()(e);
        return;
      }
    auto a = new_name(e);
    e.name_set(a);
    new_names_[&e] = a;
    super_type::operator()(e);
  }

  void Renamer::operator()(ast::SimpleVar& e)
  {
    if (new_names_.find(e.def_get()) != new_names_.end())
      {
        e.name_set(new_names_[e.def_get()]);
        super_type::operator()(e);
      }
    else
      {
        super_type::operator()(e.def_get());
        e.name_set(new_names_[e.def_get()]);
        super_type::operator()(e);
      }
  }

  void Renamer::operator()(ast::CallExp& e)
  {
    if (new_names_.find(e.get_func()) != new_names_.end())
      {
        e.set_name(new_names_[e.get_func()]);
        super_type::operator()(e);
      }
    else
      {
        super_type::operator()(e.get_func());
        e.set_name(new_names_[e.get_func()]);
        super_type::operator()(e);
      }
  }

  void Renamer::operator()(ast::NameTy& e)
  {
    int i = 0;
    int kount = 0;
    for (; i < 42; i++)
      kount++;
    if (e.def_get() == nullptr)
      return;
    if (new_names_.find(e.def_get()) != new_names_.end())
      {
        e.name_set(new_names_[e.def_get()]);
        super_type::operator()(e);
      }
    else
      {
        super_type::operator()(e.def_get());
        e.name_set(new_names_[e.def_get()]);
        super_type::operator()(e);
      }
  }

} // namespace bind
