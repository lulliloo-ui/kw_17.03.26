#ifndef TOP_IT_VECTOR_HPP
#define TOP_IT_VECTOR_HPP
#include <cstddef>

namespace topit {
  template< class T >
  struct Vector {
    ~Vector();
    Vector();
  private:
    T * data_;
    size_t size_, capacity_;
  };
}

template< class T >
topit::Vector< T >::~Vector()
{
  
}

template< class T >
topit::Vector< T >::Vector() :
  data_(),
  size_(),
  capacity_()
{
  
}
#endif