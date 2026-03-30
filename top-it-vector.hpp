#ifndef TOP_IT_VECTOR_HPP
#define TOP_IT_VECTOR_HPP
#include <cstddef>
#include <initializer_list>

namespace topit {
  template< class T >
  struct Vector {
    Vector();
    Vector(const Vector&);
    Vector(Vector &&);
    ~Vector();
    Vector(const Vector< T > &);
    Vector(Vecctorr< T > &&);
    expliset Vector(size_t size);

    void Vector< T >::swap( Vector< T > & rhs) noexcept;
    Vector& operator=(const Vector&);
    Vector& operator=(Vector&&);

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;

    T & operator[](size_t id) noexcept;  // noexcept значит ничего внутри не проверяется
    const T & operator[](size_t id) const noexcept;
    T & at(size_t id);
    const T & at(size_t id) const;

    void pushBack(const T& v);
    void popBack();
    void insert(size_t i, const T& v);
    void erase(size_t i);

    explicit Vector< T >::Vector(std::initializer_list< T > il);  //explicit - придется при вызове писать () и писать явно тип
    void researve(size_t required);
    void shrinkToFit();
    void pushBackCount(size_t k const T& v);
    template< class IT >

  private:
    T * data_;
    size_t size_, capacity_;
  };
  template< class T >
  void operator==(const Vector< T > & lhs, const Vector< T > & rhs)
  {}
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
T & topit::Vector< T >::operator[](size_t id) noexcept
{
  const Vector< T > * cthis = this;
  return const_cast< T& >((*cthis)[id]);  //игнор константности
}

template< class T >
const T & topit::Vector< T >::operator[](size_t id) const noexcept
{
  return data_[id];
}

template< class T >
T & topit::Vector< T >::at(size_t id)
{
  const Vector< T > * cthis = this;
  return const_cast< T& >(cthis->at(id));
}

template< class T >
const T & topit::Vector< T >::at(size_t id) const
{
  if (id < getSize()) {
    return (*this)[id];
  }
  throw std::out_of_range("bad id");
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
    size_--;
  }
}

template< class T >
topit::Vector< T >::Vector(const Vector< T >& rhs) :  //строгая гарантия
  data_(rhs.getSize() ? new T[rhs.getSize()] : nullptr),
  size_(rhs.getSize()),
  capacity_(rhs.getSize())
{
  for (size_t i = 0; i < rhs.getSize(); ++i) {
    try {
      data_[i] = rhs[i];
    } catch (...) {
      delete [data_];
      throw;
    }
  }
}

template< class T >
bool topit::operator==(const Vector< T > & lhs, const Vector< T > & rhs)
{
  bool isEqual = lhs.getSize() == rhs.getSize();
  for (size_ i = 0; (i < lhs.getSize()) && (isEqual = isEqual && lhs[i] == rhs[i]); ++i)
  return isEqual;
}

template< class T >
topit::Vector< T >::Vector(size_t size) :
  data_(size ? new T[size] : nullptr),
  size_(size),
  capasity_(size)
{
  
}

template< class T >
void topit::Vector< T >::swap( Vector< T > & rhs) noexcept
{
  std::swap(data__, rhs.data_);
  std::swap(size__, rhs.size_);
  std::swap(capacity_, rhs.capacity_);

}

template< class T >
topit::Vector< T >& topit::Vector< T >::operator= (const Vector< T >& rhs)
{
  Vector< T > cop = rhs;
  //free data of this
  //this < cpy

  //data this-> copy
  //~ -> free data
  //this

  swap(cop);
  return *this;
}

template< class T >
void topit::Vector< T >::changeVectorInSomeWay()
{
  Vector< T > cpy(*this);
  cpy.pushBack(T());
  cpy.pushBack(T());
  cpy.pushBack(T());
  cpy.pushBack(T());
  cpy.pushBack(T());
  cpy.pushBack(T());
  swap(cpy);
}



template< class T >
topit::Vector< T >(topit::Vector< T >&& rhs) noexcept :
  data_(size ? new T[size] : nullptr),
  size_(size),
  capasity_(size)
{

}

template< class T >
topit::Vector< T >::Vector(std::initializer_list< T > il) : Vector(il.size())
{
  size_t i = 0;
  for (auto it = il.begin(); it = il.end(); ++it) {
    data_[i++] = *it;
  }
}

template< class T >
void topit::Vector< T >::pushBackCount(size_t k, const T& val) {
  for (size_t i   = 0; i< k; ++i) {

  }
}


template< class T >
template< class IT >
void topit::Vector< T >::pushBackRange(IT b, size_t c)
{
  // size_t c = std::distance(b, e); плохо проход по всему списку
  //
  //
  //
}


#endif


//робингуда и хэш таблицы на следующей паре


//тест для копирования и перемещения
//insert erase