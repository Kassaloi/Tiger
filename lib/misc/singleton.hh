/**
 ** \file misc/singleton.hh
 ** \brief Generic singleton
 */

#pragma once

namespace misc
{
  template <typename T> class Singleton
  {
  public:
    static const T& instance()
    {
      static T t;
      return t;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

  protected:
    Singleton() {}
    virtual ~Singleton() {}
  };

} // namespace misc
