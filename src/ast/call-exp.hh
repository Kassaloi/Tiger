/**
 ** \file ast/call-exp.hh
 ** \brief Declaration of ast::CallExp.
 */

#pragma once

#include <ast/exp.hh>
#include <ast/function-dec.hh>
#include <misc/symbol.hh>

namespace ast
{
  /// CallExp.
  class CallExp : public Exp
  {
  public:
    CallExp(const Location& location, misc::symbol name, exps_type* seq);
    virtual ~CallExp();

    void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;

    const exps_type& get_seq() const;
    exps_type& get_seq();

    const misc::symbol& name_get() const;
    misc::symbol& name_get();
    void set_name(const misc::symbol& symbol);

    const FunctionDec* get_func() const;
    FunctionDec* get_func();
    void set_func(FunctionDec*);

  protected:
    exps_type* seq_;
    misc::symbol name_;
    FunctionDec* func_;
  };
} // namespace ast
#include <ast/call-exp.hxx>
