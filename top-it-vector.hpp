#ifndef TOP_IT_VECTOR_HPP
#define TOP_IT_VECTOR_HPP
#include <cstddef>
#include <memory>
#include <utility>
#include <stdexcept>
#include <initializer_list>

namespace topit {
  template< class T >
  struct Iter;

  template< class T >
  struct Vector {
    Vector();
    Vector(const Vector &);  //конструктор копирования
    Vector(Vector &&) noexcept;  //конструктор перемещения
    explicit Vector(size_t size, const T & init);
    ~Vector();

    Vector& operator=(const Vector &);
    Vector& operator=(Vector &&);

    bool isEmpty() const noexcept;  //дз + тест
    size_t getSize() const noexcept;  //дз + тест
    size_t getCapacity() const noexcept;  //дз + тест

    void pushBack(const T & v);  //дз + тест
    void popBack();  //дз + тест

    T & operator[](size_t id) noexcept;  // noexcept значит ничего внутри не проверяется
    const T & operator[](size_t id) const noexcept;
    T & at(size_t id);
    const T & at(size_t id) const;

    void swap( Vector< T > & rhs) noexcept;

    void insert(size_t i, const T& v);
    void insert(size_t i, const Vector< T > & rhs, size_t start, size_t end);
    void erase(size_t i);
    void erase(size_t start, size_t end);

    explicit Vector(std::initializer_list< T > il);  //explicit - придется при вызове писать () и писать явно тип
    void unsafePushback(const T&);
    void reserve(size_t required);
    void shrinkToFit();
    template< class IT >
    void pushbackRange(IT b, size_t c);

    Iter< T > begin() noexcept;
    Iter< T > end() noexcept;
    Iter< const T > begin() const noexcept;
    Iter< const T > end() const noexcept;

  private:
    T * data_;
    size_t size_, capacity_;
    void pushBackCount(size_t k, const T& v);
    explicit Vector(size_t size);
  };

  template< class T >
  bool operator==(const Vector< T > & lhs, const Vector< T > & rhs);


  template< class T >
  struct Iter {
    explicit Iter(T * a);
    Iter& operator++() noexcept;
    Iter& operator--() noexcept;
    bool operator==(const Iter< T > & a) const noexcept;
    bool operator!=(const Iter< T > & a) const noexcept;
    T& operator*() noexcept;
  private:
    friend class Vector< T >;
    T * it;
  };
}


template< class T >
topit::Vector< T >::Vector() :
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

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
      delete []data_;
      throw;
    }
  }
}

template< class T >
topit::Vector< T >::Vector(Vector< T > && rhs) noexcept:
  data_(rhs.data_),
  size_(rhs.size_),
  capacity_(rhs.capacity_)
{
  rhs.data_ = nullptr;
  rhs.size_ = 0;
  rhs.capacity_ = 0;
}

template< class T >
topit::Vector< T >::Vector(size_t size, const T& init) :
  Vector(size)
{
  for (size_t i = 0; i < size; ++i) {
    data_[i] = init;
  }
}

template< class T >
topit::Vector< T >::~Vector()
{
  delete [] data_;
}

template< class T >
topit::Vector< T >& topit::Vector< T >::operator=(const Vector< T > & rhs)
{
  Vector< T > cpy = rhs;  // cpy{rhs}
  //free data of this
  //this < cpy

  //data this-> copy
  //~ -> free data
  //this

  swap(cpy);
  return *this;
}

template< class T >
topit::Vector< T >& topit::Vector< T >::operator=(Vector< T > && rhs)
{
  if (this == std::addressof(rhs)) {
    return * this;
  }
  Vector< T > cpy(std::move(rhs));
  swap(cpy);
  return *this;
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
    size_--;
  }
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
  const Vector< T >* cthis = this;
  //return const_cast< T& >(static_cast< const topit::Vector<T>* >(this)->at(id));
  return const_cast< T& >(cthis -> at(id));  //избегаем дублирования с неconst at()
}

template< class T >
const T & topit::Vector< T >::at(size_t id) const
{
  if( id < getSize())
  {
    return (*this)[id];
  }
  throw std::out_of_range("bad id");
}

template< class T >
void topit::Vector< T >::swap( Vector< T > & rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
}

template< class T >
void topit::Vector< T >::insert(size_t i, const T& v)
{
  if (i > size_) {
    throw std::out_of_range("insert index out of range");
  }
  T * new_data = nullptr;
  size_t new_cap = 0;
  if (size_ < capacity_) {
    new_cap = getCapacity();
    new_data = new T[new_cap];
  } else {
    new_cap = getCapacity() * 2 + 1;
    new_data = new T[new_cap];
  }
  size_t j = 0;
  try {
    while (j < i) {
      new_data[j] = data_[j];
      j++;
    }
    new_data[j++] = v;
    while (j < getSize() + 1) {
      new_data[j] = data_[j - 1];
      j++;
    }
  } catch (...) {
    delete []new_data;
    throw;
  }
  delete[] data_;
  data_ = new_data;
  capacity_ = new_cap;
  size_++;
}

template< class T >
void topit::Vector< T >::insert(size_t i, const Vector< T >& rhs, size_t start, size_t end)
{
  size_t delta = (end - start);
  if (delta == 0) {
    return;
  }

  if (i > size_) {
    throw std::out_of_range("insert index out of range");
  }
  if (end < start) {
    throw std::invalid_argument("start > end");
  }
  if (end > rhs.size_) {
    throw std::out_of_range("rhs end out of range");
  }

  T * new_data = nullptr;
  size_t new_cap = 0;
  if ((size_ + delta) <= capacity_) {
    new_cap = getCapacity();
    new_data = new T[new_cap];
  } else {
    new_cap = size_ + delta;
    new_data = new T[new_cap];
  }
  size_t j = 0;
  try {
    while (j < i) {
      new_data[j] = data_[j];
      j++;
    }
    for (size_t k = start; k < end; ++k) {
      new_data[j] = rhs[k];
      j++;
    }
    for (size_t k = i; k < size_; ++k) {
      new_data[j] = data_[k];
      j++;
    }
  } catch (...) {
    delete []new_data;
    throw;
  }
  delete[] data_;
  data_ = new_data;
  capacity_ = new_cap;
  size_ += delta;
}

template< class T >
void topit::Vector< T >::erase(size_t i)
{
  if (i >= size_) {
    throw std::out_of_range("erase index out of range");
  }
  T * new_data = new T[capacity_];
  size_t j = 0;
  try {
    while (j < i) {
      new_data[j] = data_[j];
      j++;
    }
    while (j < size_ - 1) {
      new_data[j] = data_[j + 1];
      j++;
    }
  } catch (...) {
    delete []new_data;
    throw;
  }
  delete[] data_;
  data_ = new_data;
  size_--;
}

template< class T >
void topit::Vector< T >::erase(size_t start, size_t end)
{
  size_t delta = (end - start);
  if (delta == 0) return;
  if (end < start) {
    throw std::invalid_argument("start > end");
  }
  if (end > size_) {
    throw std::out_of_range("end out of range");
  }
  T * new_data = new T[capacity_];
  size_t j = 0;
  try {
    while (j < start) {
      new_data[j] = data_[j];
      j++;
    }
    while (j < size_ - delta) {
      new_data[j] = data_[j + delta];
      j++;
    }
  } catch (...) {
    delete []new_data;
    throw;
  }
  delete[] data_;
  data_ = new_data;
  size_ -= delta;
}

template< class T >
topit::Vector< T >::Vector(std::initializer_list< T > il) :
  Vector(il.size())
{
  size_t i = 0;
  for (auto it = il.begin(); it != il.end(); ++it) {
    data_[i++] = *it;
  }
}

template< class T>
void topit::Vector< T >::unsafePushback(const T&)
{
  assert(size_ < capacity_);
}

//    void reserve(size_t required);
//    void shrinkToFit();

template< class T >
template< class IT >
void topit::Vector< T >::pushbackRange(IT b, size_t c)
{
  //size_t c = std::distance(b, e); //плохо проход по всему списку
  // Если памяти не хватает на с
  // - делаем так, чтоб хватало на k*
  // Добавляем в конец*
}

template< class T >
void topit::Vector< T >::pushBackCount(size_t k, const T& val) {
  for (size_t i   = 0; i< k; ++i) {
    pushback(val);
  }
  //если памяти не хватает на k
  //- делаем так, чтобы хватало k
  // добавляем в конец*
}

template< class T >
topit::Vector< T >::Vector(size_t size) :
  data_(size ? new T[size] : nullptr),
  size_(size),
  capacity_(size)
{}

template< class T >
bool topit::operator==(const Vector< T > & lhs, const Vector< T > & rhs)
{
  bool isEqual = lhs.getSize() == rhs.getSize();
  for (size_t i = 0; (i < lhs.getSize()) && (isEqual = isEqual && lhs[i] == rhs[i]); ++i);
  return isEqual;
}

template< class T >
topit::Iter< T > topit::Vector< T >::begin() noexcept
{
    return Iter<T>(data_);
}

template< class T >
topit::Iter< T > topit::Vector< T >::end() noexcept
{
    return Iter<T>(data_ + size_);
}

template< class T >
topit::Iter< const T > topit::Vector< T >::begin() const noexcept
{
    return Iter<const T>(data_);
}

template< class T >
topit::Iter< const T > topit::Vector< T >::end() const noexcept
{
    return Iter<const T>(data_ + size_);
}

template< class T >
topit::Iter< T >::Iter(T * a) :
 it(a)
{}

template< class T >
topit::Iter< T > & topit::Iter< T >::operator++() noexcept
{
  ++it;
  return *this; 
}

template< class T >
topit::Iter< T > & topit::Iter< T >::operator--() noexcept
{
  --it;
  return *this; 
}

template< class T >
bool topit::Iter< T >::operator==(const Iter& other) const noexcept { 
  return it == other.it; 
}

template< class T >
bool topit::Iter< T >::operator!=(const Iter& other) const noexcept { 
  return it != other.it; 
}

template< class T >
T& topit::Iter< T >::operator*() noexcept {
  return *it;
}

#endif
