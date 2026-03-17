#ifndef TOP_IT_VECTOR_HPP
#define TOP_IT_VECTOR_HPP
#include <cstddef>

namespace topit {
  template< class T >
  struct Vector {
    ~Vector();
    Vector();
    Vector(const Vector&);
    Vector& operator=(const Vector&);
    Vector& operator=(Vector&&);

    bool isEmpty() const noexcept;  //
    bool getSize() const noexcept;  //
    bool getCapacity() const noexcept;  //

    void pushBack(const T& v);
    void popBack();
    void insert(size_t i, const T& v);
    void erase(size_t i);

  private:
    T * data_;
    size_t size_, capacity_;
  };
}

template< class T >
bool topit::Vector< T >::isEmpty() const noexcept
{
  return !size;
}

template< class T >
topit::Vector< T >::~Vector()
{
  delete [] data_;
}

template< class T >
topit::Vector< T >::Vector() :
  data_(nullptr),
  size_(0),
  capacity_(0)
{}
#endif