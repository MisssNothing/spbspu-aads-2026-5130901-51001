#define BOOST_TEST_MODULE StackTest
#include <boost/test/included/unit_test.hpp>
#include "stack.hpp"
#include <string>

BOOST_AUTO_TEST_CASE(test_push_top) {
  petrenko::Stack<int> s;

  s.push(10);
  BOOST_CHECK_EQUAL(s.top(), 10);
  BOOST_CHECK_EQUAL(s.size(), 1);

  s.push(20);
  BOOST_CHECK_EQUAL(s.top(), 20);
  BOOST_CHECK_EQUAL(s.size(), 2);

  s.push(30);
  BOOST_CHECK_EQUAL(s.top(), 30);
  BOOST_CHECK_EQUAL(s.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_pop_lifo) {
  petrenko::Stack<int> s;

  s.push(1);
  s.push(2);
  s.push(3);
  s.push(4);

  BOOST_CHECK_EQUAL(s.pop(), 4);
  BOOST_CHECK_EQUAL(s.pop(), 3);
  BOOST_CHECK_EQUAL(s.pop(), 2);
  BOOST_CHECK_EQUAL(s.pop(), 1);
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(test_empty_stack) {
  petrenko::Stack<int> s;

  BOOST_CHECK(s.empty());

  s.push(42);
  BOOST_CHECK(!s.empty());

  s.pop();
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(test_stack_size) {
  petrenko::Stack<int> s;

  BOOST_CHECK_EQUAL(s.size(), 0);

  for (int i = 0; i < 100; ++i) {
    s.push(i);
  }
  BOOST_CHECK_EQUAL(s.size(), 100);

  for (int i = 0; i < 50; ++i) {
    s.pop();
  }
  BOOST_CHECK_EQUAL(s.size(), 50);
  BOOST_CHECK_EQUAL(s.top(), 49);
}

BOOST_AUTO_TEST_CASE(test_stack_clear) {
  petrenko::Stack<std::string> s;

  s.push("first");
  s.push("second");
  s.push("third");

  BOOST_CHECK_EQUAL(s.size(), 3);
  s.clear();
  BOOST_CHECK(s.empty());
  BOOST_CHECK_EQUAL(s.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_large_stack) {
  petrenko::Stack<int> s;
  const int N = 10000;

  for (int i = 0; i < N; ++i) {
    s.push(i);
  }

  BOOST_CHECK_EQUAL(s.size(), N);
  BOOST_CHECK_EQUAL(s.top(), N - 1);

  for (int i = N - 1; i >= 0; --i) {
    BOOST_CHECK_EQUAL(s.pop(), i);
  }

  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(test_exception_on_empty_pop) {
  petrenko::Stack<int> s;

  BOOST_REQUIRE_THROW(s.pop(), std::runtime_error);
  BOOST_REQUIRE_THROW(s.top(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_move_semantics) {
  petrenko::Stack<std::string> s;

  std::string str1 = "hello";
  std::string str2 = "world";

  s.push(std::move(str1));
  s.push(std::move(str2));

  BOOST_CHECK_EQUAL(s.size(), 2);
  BOOST_CHECK_EQUAL(s.top(), "world");
  s.pop();
  BOOST_CHECK_EQUAL(s.top(), "hello");
}

BOOST_AUTO_TEST_CASE(test_copy_assignment) {
  petrenko::Stack<int> s1;
  s1.push(1);
  s1.push(2);
  s1.push(3);

  petrenko::Stack<int> s2;
  s2 = s1;

  BOOST_CHECK_EQUAL(s2.size(), 3);
  BOOST_CHECK_EQUAL(s2.pop(), 3);
  BOOST_CHECK_EQUAL(s2.pop(), 2);
  BOOST_CHECK_EQUAL(s2.pop(), 1);
}

BOOST_AUTO_TEST_CASE(test_comparison_operators) {
  petrenko::Stack<int> s1, s2, s3;

  s1.push(1);
  s1.push(2);
  s1.push(3);

  s2.push(1);
  s2.push(2);
  s2.push(3);

  s3.push(1);
  s3.push(2);

  BOOST_CHECK(s1 == s2);
  BOOST_CHECK(s1 != s3);
  BOOST_CHECK(s2 != s3);
}
