/**
 ** \file ast/escapable.hh
 ** \brief Declaration of ast::Escapable.
 */

#pragma once

#include <ast/fwd.hh>

namespace ast
{
  /// Escapable.
  class Escapable
  {
    // FIXED
  public:
    Escapable();
    virtual ~Escapable();

    bool get_is_escaped() const;
    bool get_is_escaped();
    void set_is_escaped(bool b);

  private:
    bool is_escaped_;
  };
} // namespace ast
#include <ast/escapable.hxx>
