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

bool testInsertOneElementOutOfRange()
{
  topit::Vector< int > v;
  v.pushBack(1);
  v.pushBack(2);
  v.pushBack(3);
  try {
    v.insert(10, 4);
    return false;
  } catch (const std::out_of_range &) {
    return true;
  } catch (...) {
    return false;
  }
}

bool testSingleElementInsertion()
{
  topit::Vector< int > v;
  v.pushBack(1);
  v.pushBack(2);
  v.pushBack(4);
  try {
    v.insert(2, 3);
    return v.getSize() == 4 && v[0] == 1 && v[1] == 2 && v[2] == 3 &&
      v[3] == 4 && v.getSize() == 4;
  } catch (...) {
    return false;
  }
}

bool testInsertingMultipleElementsOutOfRange()
{
  topit::Vector< int > v;
  v.pushBack(1);
  v.pushBack(2);
  topit::Vector< int > nv;
  nv.pushBack(3);
  nv.pushBack(4);
  try {
    v.insert(10, nv, 0, 2);
    return false;
  } catch (const std::out_of_range &) {
    return true;
  } catch (...) {
    return false;
  }
}

bool testInsertingMultipleElementsStartMoreEnd()
{
  topit::Vector< int > v;
  v.pushBack(1);
  v.pushBack(2);
  topit::Vector< int > nv;
  nv.pushBack(3);
  nv.pushBack(4);
  try {
    v.insert(1, nv, 2, 0);
    return false;
  } catch (const std::invalid_argument &) {
    return true;
  } catch (...) {
    return false;
  }
}

bool testInsertingMultipleElementsStartEquallyEnd()
{
  topit::Vector< int > v;
  v.pushBack(1);
  v.pushBack(2);
  topit::Vector< int > nv;
  nv.pushBack(3);
  nv.pushBack(4);
  try {
    v.insert(1, nv, 0, 0);
    return v.getSize() == 2 && v[0] == 1 && v[1] == 2 &&
      nv.getSize() == 2 && nv[0] == 1 && nv[1] == 2;
  } catch (...) {
    return false;
  }
}

bool testInsertingMultipleElements()
{
  topit::Vector< int > v;
  v.pushBack(1);
  v.pushBack(4);
  topit::Vector< int > nv;
  nv.pushBack(2);
  nv.pushBack(3);
  try {
    v.insert(1, nv, 0, 2);
    return v.getSize() == 4 && v[0] == 1 && v[1] == 2 &&
      v[2] == 3 && v[3] == 4 &&
      nv.getSize() == 2 && nv[0] == 1 && nv[1] == 2;
  } catch (...) {
    return false;
  }
}

bool testEraseIndexOutOfRangeOneElement()
{
  topit::Vector< int > v;
  v.pushBack(1);
  v.pushBack(2);
  try {
    v.erase(5);
    return false;
  } catch (const std::out_of_range &) {
    return true;
  } catch (...) {
    return false;
  }
}

bool testEraseOneElement()
{
  topit::Vector< int > v;
  v.pushBack(1);
  v.pushBack(2);
  try {
    v.erase(0);
    return v.getSize() == 1 && v[0] == 2;
  } catch (...) {
    return false;
  }
}

bool testEraseMultipleElementsStartMoreEnd()
{
  topit::Vector< int > v;
  v.pushBack(1);
  v.pushBack(2);
  try {
    v.erase(1, 0);
    return false;
  } catch (const std::invalid_argument &) {
    return true;
  } catch (...) {
    return false;
  }
}

bool testEraseMultipleElementsStartEquallyEnd()
{
  topit::Vector< int > v;
  v.pushBack(1);
  v.pushBack(2);
  try {
    v.erase(0, 0);
    return v.getSize() == 2 && v[0] == 1 && v[1] == 2;
  } catch (...) {
    return false;
  }
}

bool testEraseMultipleElement()
{
  topit::Vector< int > v;
  v.pushBack(1);
  v.pushBack(0);
  v.pushBack(0);
  v.pushBack(2);
  try {
    v.erase(1, 3);
    return v.getSize() == 2 && v[0] == 1 && v[1] == 2;
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
    { "Copy empty vector", testCopyConstructorForEmpty},
    { "Copy non-empty vector", testCopyConstructorForNonEmpty},
    { "Displacement constructo with empty vector", testDisplacementConstructorEmpty},
    { "Displacement constructo with non-empty vector", testDisplacementConstructorNonEmpty},
    { "Initialized constructor with empty vector", testDisplacementConstructorNonEmpty},
    { "Initialized constructor non-empty", testInitializedConstructor},
    { "Insert one element out of range", testInsertOneElementOutOfRange},
    { "Inserting a single element", testSingleElementInsertion},
    { "Inserting multiple elements out of range", testInsertingMultipleElementsOutOfRange},
    { "Inserting multiple elements start more end", testInsertingMultipleElementsStartMoreEnd},
    { "Inserting multiple elements start equally end", testInsertingMultipleElementsStartEquallyEnd},
    { "Inserting multiple elements", testInsertingMultipleElements},
    { "Erase index out of rangewith one element", testEraseIndexOutOfRangeOneElement},
    { "Erase one element", testEraseOneElement},
    { "Erase multiple elements start more end", testEraseMultipleElementsStartMoreEnd},
    { "Erase multiple elements start equally end", testEraseMultipleElementsStartEquallyEnd},
    { "Erase multiple element", testEraseMultipleElement},
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
