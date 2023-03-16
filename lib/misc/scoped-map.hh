/**
 ** \file misc/scoped-map.hh
 ** \brief Declaration of misc::scoped_map.
 **
 ** This implements a stack of dictionnaries.  Each time a scope is
 ** opened, a new dictionnary is added on the top of the stack; the
 ** dictionary is removed when the scope is closed.  Lookup of keys
 ** is done in the last added dictionnary first (LIFO).
 **
 ** In particular this class is used to implement symbol tables.
 **/

#pragma once

#include <map>
#include <vector>

namespace misc
{
  //FIXED
  template <typename Key, typename Data> class scoped_map
  {
  public:
    using scope_slot = std::map<Key, Data>;
    using scope_type = std::vector<scope_slot>;

    scoped_map();
    ~scoped_map() = default;

    void put(const Key& key, const Data& data);
    Data get(const Key& key) const;
    std::ostream& dump(std::ostream& os) const;
    void scope_begin();
    void scope_end();

    const scope_type& get_scopes() const;

  private:
    scope_type scopes_;
  };

  template <typename Key, typename Data>
  std::ostream& operator<<(std::ostream& ostr,
                           const scoped_map<Key, Data>& tbl);

  template <typename Key, typename Data>
  std::enable_if_t<std::is_pointer<Data>::value, Data> throw_range_error()
  {
    return nullptr;
  }

  template <typename Key, typename Data>
  std::enable_if_t<!std::is_pointer<Data>::value, Data> throw_range_error()
  {
    throw(std::range_error("Element not found"));
  }

} // namespace misc

#include <misc/scoped-map.hxx>
