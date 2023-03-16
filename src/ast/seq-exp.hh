/**
 ** \file ast/seq-exp.hh
 ** \brief Declaration of ast::SeqExp.
 */

#pragma once

#include <ast/exp.hh>

namespace ast
{
  /// SeqExp.
  class SeqExp : public Exp
  {
    // FIXED
  public:
    SeqExp(const Location& location, exps_type* seq);
    virtual ~SeqExp();

    void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;

    const exps_type& get_seq() const;
    exps_type& get_seq();

  protected:
    exps_type* seq_;
  };
} // namespace ast
#include <ast/seq-exp.hxx>
