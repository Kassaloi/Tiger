/** \file misc/scoped-map.hxx
 ** \brief Implementation of misc::scoped_map.
 */

#pragma once

#include <sstream>
#include <stdexcept>
#include <type_traits>

#include <misc/algorithm.hh>
#include <misc/contract.hh>
#include <misc/indent.hh>
#include <range/v3/view/reverse.hpp>

namespace misc
{
  //FIXED
  template <typename Key, typename Data>
  scoped_map<Key, Data>::scoped_map()
    : scopes_()
  {}

  template <typename Key, typename Data>
  void scoped_map<Key, Data>::put(const Key& key, const Data& data)
  {
    scopes_.back()[key] = data;
  }

  template <typename Key, typename Data>
  Data scoped_map<Key, Data>::get(const Key& key) const
  {
    for (auto i = scopes_.rbegin(); i != scopes_.rend(); i++)
      {
        if (i->find(key) != i->end())
          return i->find(key)->second;
      }
    return throw_range_error<Key, Data>();
  }

  template <typename Key, typename Data>
  std::ostream& scoped_map<Key, Data>::dump(std::ostream& os) const
  {
    for (auto i = scopes_.rbegin(); i != scopes_.rend(); i++)
      {
        os << "( ";
        for (auto j = i->begin(); j != i->end(); j++)
          {
            os << j->first << " " << j->second << " ";
          }
        os << ")\n";
      }
    return os;
  }

  template <typename Key, typename Data>
  void scoped_map<Key, Data>::scope_begin()
  {
    scopes_.push_back(scope_slot());
  }

  template <typename Key, typename Data> void scoped_map<Key, Data>::scope_end()
  {
    scopes_.pop_back();
  }

  template <typename Key, typename Data>
  inline const typename scoped_map<Key, Data>::scope_type&
  scoped_map<Key, Data>::get_scopes() const
  {
    return scopes_;
  }

  template <typename Key, typename Data>
  inline std::ostream& operator<<(std::ostream& ostr,
                                  const scoped_map<Key, Data>& tbl)
  {
    return tbl.dump(ostr);
  }

} // namespace misc
