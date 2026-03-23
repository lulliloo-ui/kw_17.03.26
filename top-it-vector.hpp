#ifndef TOP_IT_VECTOR_HPP
#define TOP_IT_VECTOR_HPP
#include <cstddef>

namespace topit {
  template< class T >
  struct Vector {
    Vector();
    Vector(const Vector&);
    Vector(Vector &&);
    ~Vector();
    Vector& operator=(const Vector&);
    Vector& operator=(Vector&&);

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;

    void pushBack(const T& v);  //
    void popBack();  //
    void insert(size_t i, const T& v);
    void erase(size_t i);

  private:
    T * data_;
    size_t size_, capacity_;
  };
}

template< class T >
topit::Vector< T >::Vector() :
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template< class T >
topit::Vector< T >::~Vector()
{
  delete [] data_;
}

template< class T >
bool topit::Vector< T >::isEmpty() const noexcept
{
  return !size_;
}

template< class T >
size_t topit::Vector< T >::getSize() const noexcept
{
  return size_;
}

template< class T >
size_t topit::Vector< T >::getCapacity() const noexcept
{
  return capacity_;
}

template< class T >
void topit::Vector< T >::pushBack(const T & v)
{
  if (size_ < capacity_) {
    data_[size_] = v;
    size_++;
  } else {
    T * new_vec = new T[2 * capacity_ + 1];
    try {
      for (size_t i = 0; i < size_; ++i) {
        new_vec[i] = data_[i];
      }
    new_vec[size_] = v;
    delete [] data_;
    data_ = new_vec;
    size_++;
    capacity_ = capacity_ * 2 + 1;
    } catch (...) {
      delete [] new_vec;
      throw;
    }
  }
}

template< class T >
void topit::Vector< T >::popBack()
{
  if (size_) {
    data_[size_ - 1].~T();
    size_--;
  }
}

#endif