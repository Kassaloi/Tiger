/**
 ** \file type/named.cc
 ** \brief Implementation for type/named.hh.
 */

#include <set>

#include <type/named.hh>
#include <type/visitor.hh>

namespace type
{
  Named::Named(misc::symbol name)
    : name_(name)
    , type_(nullptr)
  {}

  Named::Named(misc::symbol name, const Type* type)
    : name_(name)
    , type_(type)
  {}

  void Named::accept(ConstVisitor& v) const { v(*this); }

  void Named::accept(Visitor& v) { v(*this); }

  bool Named::sound() const
  {
    std::set<const Type*> type_set;
    for (auto curs = this; curs ;curs = dynamic_cast<const Named*>(curs->type_))
      {
        if (!type_set.insert(curs).second)
          {
            return false;
          }
      }
    return true;
  }

  bool Named::compatible_with(const Type& other) const
  {
    return type_->compatible_with(other);
  }

} // namespace type
