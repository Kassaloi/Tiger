/**
 ** \file bind/libbind.hh
 ** \brief Interface of the bind module.
 */

#pragma once


#include <misc/error.hh>
#include <ast/fwd.hh>

namespace bind
{
  misc::error bind(ast::Ast& tree);
  void rename(ast::Ast& tree);
}