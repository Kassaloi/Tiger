/**
 ** \file ast/string-exp.hh
 ** \brief Declaration of ast::StringExp.
 */

#pragma once

#include <string>
#include <ast/exp.hh>

namespace ast
{
  /// StringExp.
  class StringExp : public Exp
  {
  public:
    StringExp(const Location& location, std::string str);
    virtual ~StringExp() = default;

    void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;

    const std::string get_var() const;
    std::string get_var();

  protected:
    std::string str_;
  };
} // namespace ast
#include <ast/string-exp.hxx>
