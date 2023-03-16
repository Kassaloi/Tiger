/**
 ** \file ast/break-exp.hh
 ** \brief Declaration of ast::BreakExp.
 */

#pragma once

#include <ast/exp.hh>

namespace ast
{
  class BreakExp : public Exp
  {
  public:
    BreakExp(const Location& location);
    virtual ~BreakExp();

    void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;

    const Exp* get_loop() const;
    Exp* get_loop();
    void set_loop(Exp* loop);

  private:
    Exp* loop_;
  };
} // namespace ast
#include <ast/break-exp.hxx>
