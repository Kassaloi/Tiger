/**
 ** \file ast/record-ty.hh
 ** \brief Declaration of ast::RecordTy.
 */

#pragma once

#include <ast/field.hh>
#include <ast/ty.hh>

namespace ast
{
  /// RecordTy.
  class RecordTy : public Ty
  {
    // FIXED
  public:
    RecordTy(const Location& location, fields_type* field);
    virtual ~RecordTy();

    void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;

    const fields_type& get_field() const;
    fields_type& get_field();

  protected:
    fields_type* field_;
  };
} // namespace ast
#include <ast/record-ty.hxx>
