/**
 ** \file ast/if-exp.hh
 ** \brief Declaration of ast::IfExp.
 */

#pragma once

#include <ast/exp.hh>
#include <ast/seq-exp.hh>

namespace ast
{
  /// IfExp.
  class IfExp : public Exp
  {
    // FIXED
  public:
    IfExp(const Location& location, Exp* exp, Exp* Ethen, Exp* Eelse = nullptr);
    virtual ~IfExp();

    void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;

    const Exp& get_exp() const;
    Exp& get_exp();

    const Exp& get_then() const;
    Exp& get_then();

    const Exp* get_else() const;
    Exp* get_else();

  protected:
    Exp* exp_;
    Exp* Ethen_;
    Exp* Eelse_;
  };
} // namespace ast
#include <ast/if-exp.hxx>
