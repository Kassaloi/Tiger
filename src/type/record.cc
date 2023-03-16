/**
 ** \file type/record.cc
 ** \brief Implementation for type/record.hh.
 */

#include <ostream>

#include <type/builtin-types.hh>
#include <type/nil.hh>
#include <type/record.hh>
#include <type/visitor.hh>

namespace type
{
  void Record::accept(ConstVisitor& v) const { v(*this); }

  void Record::accept(Visitor& v) { v(*this); }

  const Type* Record::field_type(misc::symbol key) const
  {
    for (auto elm : fields_)
      {
        if (elm.name_get() == key)
          {
            return &elm.type_get();
          }
      }
    return nullptr;
  }

  int Record::field_index(misc::symbol key) const
  {
    size_t i = 0;
    while (i < fields_.size() && fields_[i].name_get() != key)
      {
        ++i;
      }
    if (i >= fields_.size())
      return -1;
    return i;
  }

  bool Record::comp_with(const Type& other) const
  {
    if (&other == this)
      {
        return true;
      }
    if (dynamic_cast<const Nil*>(&other))
      {
        return true;
      }
    return false;
  }

} // namespace type
