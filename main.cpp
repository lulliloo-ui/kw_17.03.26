#include "top-it-vector.hpp"
#include <iostream>
#include <iomanip>


bool testEmptyVecror()  // можно добавить std::map
{
  // std::cout << __func__ << "\n";
  topit::Vector< int > v;
  return v.isEmpty();
}

int main()
{
  using test_t = std::pair< const char *, bool(*)() >;
  test_t tests[] = {
    { "Empty vector", testEmptyVecror}
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
