#include "top-it-vector.hpp"
#include <iostream>
#include <iomanip>


bool testEmptyVecror()  // можно добавить std::map
{
  // std::cout << __func__ << "\n";
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

bool testCopyConstructor()
{
  topit::Vector< int > v;
  topit::Vector< int > yav = v;
  return v == yav;
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
  topit::Vector< int > yav = v;
  try {
  return yav.getSize() == v.getSize() && yav.at(0) == v.at(0);
  } catch (...) {
    return false;
  }
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
    { "Copy empty vector", testCopyConstructor},
    { "Copy empty vector", testCopyConstructorForEmpty},
    { "Copy non-empty vector", testCopyConstructorForNonEmpty},
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
