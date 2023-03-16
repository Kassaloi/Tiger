/**
 ** \file type/builtin-types.hh
 ** \brief The classes Int, String, Void.
 */
#pragma once

#include <misc/singleton.hh>
#include <type/fwd.hh>
#include <type/type.hh>

namespace type
{
  class String
    : public Type
    , public misc::Singleton<type::String>
  {
  public:
    String() = default;
    ~String() {}

    void accept(ConstVisitor& v) const;
    void accept(Visitor& v);

    const Type& actual() const;
  };

  class Int
    : public type::Type
    , public misc::Singleton<type::Int>
  {
  public:
    Int() = default;
    ~Int() {}
    void accept(ConstVisitor& v) const;
    void accept(Visitor& v);

    const Type& actual() const;
  };

  class Void
    : public type::Type
    , public misc::Singleton<type::Void>
  {
  public:
    Void() = default;
    ~Void() {}

    void accept(ConstVisitor& v) const;
    void accept(Visitor& v);

    const Type& actual() const;
  };
} // namespace type
