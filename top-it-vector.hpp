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

    void insert(size_t i, const T & v);
    Iter< T > insert(Iter< T > i, const T & v);
    void insert(size_t i, const Vector< T > & rhs, size_t start, size_t end);
    Iter< T > insert(Iter< T > i, const Vector< T > & rhs, Iter< T > start, Iter< T > end);
    void erase(size_t i);
    //+ erase (iter)
    void erase(size_t start, size_t end);
    //+ erase (iter)

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
  data_(nullptr),
  size_(rhs.getSize()),
  capacity_(rhs.getSize())
{
  if (size_ == 0) return;
  data_ = static_cast<T*>(::operator new(sizeof(T) * size_));
  size_t i = 0;
  try {
    for (size_t j = 0; j < rhs.getSize(); ++j) {
      new (&data_[j]) T(rhs[j]);
      i++;
    }
  } catch (...) {
    for (size_t j = 0; j < i; ++j) {
      data_[j].~T();
    }
    ::operator delete(data_);
    throw;
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
  size_t i = 0;
  try {
    for (size_t j = 0; j < size; ++j) {
      new (&data_[j]) T(init);
      i++;
    }
  } catch (...) {
    for (size_t j = 0; j < i; ++j) {
      data_[j].~T();
    }
    ::operator delete(data_);
    throw;
  }
}

template< class T >
topit::Vector< T >::~Vector()
{
  ::operator delete(data_);
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
    new (& data_[size_]) T(v);  
    size_++;
  } else {
    size_t new_cap = 2 * capacity_ + 1;
    T * new_data = static_cast< T * >(::operator new(sizeof(T) * new_cap));
    size_t i = 0;
    try {
      for (size_t j = 0; j < size_; ++j) {
        new (& new_data[j]) T(data_[j]);
        i++;
      }
      new (& new_data[i]) T(v);
      i++;
      for (size_t j = 0; j < size_; ++j) {
        data_[j].~T();
      }
      ::operator delete(data_);
      data_ = new_data;
      new_data = nullptr;
      size_++;
      capacity_ = new_cap;
    } catch (...) {
      for (size_t j = 0; j < i; ++j) {
        new_data[j].~T();
      }
      ::operator delete(new_data);
      throw;
    }
  }
}

template< class T >
void topit::Vector< T >::popBack()
{
  if (size_) {
    size_--;

    data_[size_].~T();
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
  return const_cast< T & >(cthis -> at(id));  //избегаем дублирования с неconst at()
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
    new_data = static_cast< T * >(::operator new(sizeof(T) * new_cap));
  } else {
    new_cap = getCapacity() * 2 + 1;
    new_data = static_cast< T * >(::operator new(sizeof(T) * new_cap));
  }
  size_t count = 0;
  size_t j = 0;
  try {
    while (j < i) {
      new (& new_data[j]) T(data_[j]);
      j++;
      count++;
    }
    new (& new_data[i]) T(v);
    j++;
    count++;
    while (j < size_) {
      new (& new_data[j + 1]) T(data_[j]);
      count++;
    }
    for (size_t k = 0; k < size_; ++k) {
      data_[k].~T();
    }
    ::operator delete(data_);
    data_ = new_data;
    capacity_ = new_cap;
    size_++;
  } catch (...) {
    for (size_t k = 0; k < count; ++k) {
      new_data[k].~T();
    }
    ::operator delete(new_data);
    throw;
  }
}

template< class T >
topit::Iter< T > topit::Vector< T >::insert(Iter< T > i, const T & v)
{
  size_t index = i - begin();
  insert(index, value);
  return begin() + index;
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
topit::Iter< T > topit::Vector< T >::insert(Iter< T > i, const Vector< T > & rhs, Iter< T > start, Iter< T > end)
{
  size_t index = i - begin();
  size_t s = start - rhs.begin();
  size_t e = end - rhs.begin();
  insert(index, rhs, s, e);
  return begin() + index;
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
void topit::Vector< T >::unsafePushback(const T & v)
{
  assert(size_ < capacity_);
  data_[size_] = v;
  size_++;
}

template< class T>
void topit::Vector< T >::reserve(size_t required)
{
  if (required <= capacity_) {
    return;
  }
  T* new_data = new T[required];
  try {
    for (size_t i = 0; i < size_; ++i) {
      new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = required;
  } catch (...) {
    delete[] new_data;
    throw;
  }
}

template< class T>
void topit::Vector< T >::shrinkToFit()
{
  if (size_ == capacity_) {
    return;
  }
  
  if (size_ == 0) {
    delete[] data_;
    data_ = nullptr;
    capacity_ = 0;
    return;
  }
  T* new_data = new T[size_];
  try {
    for (size_t i = 0; i < size_; ++i) {
      new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = size_;
  } catch (...) {
    delete[] new_data;
    throw;
  }
}

template< class T >
template< class IT >
void topit::Vector< T >::pushbackRange(IT b, size_t c)
{
  //size_t c = std::distance(b, e); //плохо проход по всему списку
  // Если памяти не хватает на с
  // - делаем так, чтоб хватало на k*
  // Добавляем в конец*
  if (c == 0) {
    return;
  }
  T * new_vec = new T[size_ + c];
  try {
    for (size_t i = 0; i < size_; ++i) {
      new_vec[i] = data_[i];
    }
    for (size_t i = 0; i < c; ++i) {
      new_vec[size_ + i] = *b;
      ++b;
    }
    delete [] data_;
    data_ = new_vec;
    size_ += c;
    capacity_ = size_;
    catch (...) {
      detete []new_vec;
      throw;
    }
  }
}

template< class T >
void topit::Vector< T >::pushBackCount(size_t k, const T& val) {
//   for (size_t i   = 0; i< k; ++i) {
//     pushBack(val);
//   }
  //если памяти не хватает на k
  //- делаем так, чтобы хватало k
  // добавляем в конец*

  if (k == 0) {
    return;
  }
  T * new_vec = new T[size_ + k];
  try {
    for (size_t i = 0; i < size_; ++i) {
      new_vec[i] = data_[i];
    }
    for (size_t i = 0; i < k; ++i) {
      new_vec[size_ + i] = val;
    }
    delete [] data_;
    data_ = new_vec;
    size_ += k;
    capacity_ = size_;
    catch (...) {
      detete []new_vec;
      throw;
    }
  }
}

template< class T >
topit::Vector< T >::Vector(size_t size) :
  data_(nullptr),
  size_(size),
  capacity_(size)
{
  if (!size) {
    return;
  }
  data_ = static_cast< T* >(::operator new(sizeof(T) * size));
}

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
