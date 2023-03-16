/**
 ** \file ast/let-exp.hh
 ** \brief Declaration of ast::LetExp.
 */

#pragma once

#include <ast/chunk-list.hh>
#include <ast/exp.hh>
#include <misc/contract.hh>

namespace ast
{
  /// LetExp.
  class LetExp : public Exp
  {
    // FIXED
  public:
    LetExp(const Location& location, ChunkList* chunk, Exp* exp);
    virtual ~LetExp();

    void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;

    const ChunkList& get_chunk() const;
    ChunkList& get_chunk();

    const Exp& get_exp() const;
    Exp& get_exp();

  protected:
    ChunkList* chunk_;
    Exp* exp_;
  };
} // namespace ast
#include <ast/let-exp.hxx>
