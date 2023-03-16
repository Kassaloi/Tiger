/**
 ** \file bind/libbind.cc
 ** \brief Define exported bind functions.
 */

#include <bind/binder.hh>
#include <bind/libbind.hh>
#include <bind/renamer.hh>

namespace bind
{
  misc::error bind(ast::Ast& tree)
  {
    Binder b;
    b(tree);
    return b.error_get();
  }

  void rename(ast::Ast& tree)
  {
    Renamer renamer;
    renamer(tree);
  }

} // namespace bind