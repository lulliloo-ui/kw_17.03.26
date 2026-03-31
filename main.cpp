#include "top-it-vector.hpp"
#include <iostream>
#include <iomanip>


bool testEmptyVecror()  // можно добавить std::map
{
  // std::cout << __func__ << "\n";  выводит название исполняемой функции
  topit::Vector< int > v;
  return v.isEmpty();
}

bool testGetSize()
{
  topit::Vector< int > v;
  v.pushBack(1);
  v.pushBack(2);
  v.pushBack(3);
  return v.getSize() == 3;
}

bool testGetCapacityEmpty()
{
  topit::Vector< int > v;
  return v.getCapacity() == 0;
}

bool testGetCapacityNoOneElement()
{
  topit::Vector< int > v;
  v.pushBack(1);
  v.pushBack(2);
  return v.getCapacity() > 0;
}

bool testPushBack()
{
  topit::Vector< int > v;
  v.pushBack(1);
  v.pushBack(2);
  v.pushBack(3);
  v.pushBack(4);
  return !v.isEmpty() && v.getSize() == 4;
}

bool testPopBack()
{
  topit::Vector< int > v;
  v.pushBack(1);
  v.pushBack(2);
  v.pushBack(3);
  v.popBack();
  return v.getSize() == 2 && !v.isEmpty();
}

bool testElementInboundAccess()
{
  topit::Vector< int > v;
  v.pushBack(1);
  try {
    int & val = v.at(0);
    return val == 1;
  } catch (...) {
    return false;
  }
}

bool testElementOutOfBoundAccess()
{
  topit::Vector< int > v;
  try {
    int & val = v.at(0);
    return false;
  } catch (const std::out_of_range &) {
    return true;
  } catch (...) {
    return false;
  }
}

bool testElementInboundConstAccess()
{
  topit::Vector< int > v;
  v.pushBack(1);
  const topit::Vector< int > & rv = v;
  try {
    const int & val = rv.at(0);
    return val == 1;
  } catch (...) {
    return false;
  }
}

bool testElementOutOfBoundConstAccess()
{
  const topit::Vector< int > v;
  try {
    v.at(0);
    return false;
  } catch (const std::out_of_range &) {
    return true;
  } catch (...) {
    return false;
  }
}

bool testCopyConstructorForEmpty() 
{
  topit::Vector< int > v;
  topit::Vector< int > yav = v;
  return v == yav;
}

bool testCopyConstructorForNonEmpty() 
{
  topit::Vector< int > v;
  v.pushBack(1);
  topit::Vector< int > yav = v;
  try {
    return yav.getSize() == v.getSize() && yav.at(0) == v.at(0);
  } catch (...) {
    return false;
  }
}

bool testDisplacementConstructorEmpty()
{
  topit::Vector< int > v;
  topit::Vector< int > nv = std::move(v);
  return v.getSize() ==0 && nv.getSize() == 0 &&
    v.isEmpty() && nv.isEmpty() &&
    v.getCapacity() ==0 && nv.getCapacity() == 0;
}

bool testDisplacementConstructorNonEmpty()
{
  topit::Vector< int > v;
  v.pushBack(1);
  v.pushBack(2);
  int * add = std::addressof(v[0]);
  size_t cap = v.getCapacity();
  topit::Vector< int > nv = std::move(v);
  return (v.getSize() == 0) && (v.getCapacity() == 0) &&
    v.isEmpty() && nv.getCapacity() == cap &&
    add == std::addressof(nv[0]) && (nv.getSize() == 2);
}

bool testInitializedConstructorEmpty()
{
  topit::Vector< int > v(0, 1);
  return v.getSize() == 0 && v.getCapacity() == 0 && v.isEmpty();
}

bool testInitializedConstructor()
{
  topit::Vector< int > v(4, 1);
  bool init = true;
  for(size_t i = 0; i < v.getSize(); ++i) {
    init = init && (v[i] == 1);
  }
  return v.getSize() == 4 && v.getCapacity() == 4 && init;
}

bool testInitialaizerList() {
  topit::Vector< T > v = {1, 2};
  return v.getSize() = 2 && v[0] == 1 && v[1] == 2;
}

int main()
{
  using test_t = std::pair< const char *, bool(*)() >;
  test_t tests[] = {
    { "Empty vector", testEmptyVecror},
    { "Get size", testGetSize },
    { "Get campacity with empty vector", testGetCapacityEmpty},
    { "Get campacity with not one element", testGetCapacityNoOneElement},
    { "Push back elements", testPushBack},
    { "Pop back one element", testPopBack},
    { "Inbound access", testElementInboundAccess},
    { "Out of bound access", testElementOutOfBoundAccess},
    { "Inbound const access", testElementInboundConstAccess},
    { "Out of bound const access", testElementOutOfBoundConstAccess},
    { "Copy empty vector", testCopyConstructorForEmpty},
    { "Copy non-empty vector", testCopyConstructorForNonEmpty},
    { "Displacement constructo with empty vector", testDisplacementConstructorEmpty},
    { "Displacement constructo with non-empty vector", testDisplacementConstructorNonEmpty},
    { "Initialized constructor with empty vector", testDisplacementConstructorNonEmpty},
    { "Initialized constructor non-empty", testInitializedConstructor},
    { "Non-empty vector for non-empty initializer list", testInitialaizerList}
  };
  const size_t count = sizeof(tests) / sizeof(test_t);
  std::cout << std::boolalpha;
  bool pass = true;
  for (size_t i = 0; i < count; ++i) {
    bool res = tests[i].second();
    std::cout << tests[i].first << ":" << res << "\n";
    pass = pass && res;
  }
  std::cout << "RESULT: " << pass << "\n";
  //подсчет пройденный непройденных
  //выводить только не прошедшие тесты
}
