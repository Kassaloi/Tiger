/**
 ** \file type/class.cc
 ** \brief Implementation for type/class.hh.
 */

#include <ostream>

#include <range/v3/algorithm/find.hpp>
#include <type/class.hh>
#include <type/visitor.hh>

namespace type
{
  Class::Class(const Class* super)
    : Type()
    , id_(fresh_id())
    , super_(super)
    , subclasses_()
  {}

  void Class::accept(ConstVisitor& v) const { v(*this); }

  void Class::accept(Visitor& v) { v(*this); }

  const Type* Class::attr_type(misc::symbol key) const
  {
    // FIXED
  }

  const Type* Class::meth_type(misc::symbol key) const
  {
    // FIXED
  }

  const Class& Class::object_instance()
  {
    // FIXEd
  }

  unsigned Class::fresh_id()
  {
    static unsigned counter_ = 0;
    return counter_++;
  }

} // namespace type
