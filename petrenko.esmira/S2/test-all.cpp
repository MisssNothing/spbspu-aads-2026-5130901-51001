#define BOOST_TEST_MODULE AllTests
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include <string>
#include "parser.hpp"
#include "queue.hpp"
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(test_get_precedence) {
  BOOST_CHECK_EQUAL(petrenko::getPrecedence("+"), 1);
  BOOST_CHECK_EQUAL(petrenko::getPrecedence("*"), 2);
  BOOST_CHECK_EQUAL(petrenko::getPrecedence("gcd"), 3);
  BOOST_CHECK_EQUAL(petrenko::getPrecedence("unknown"), 0);
}

BOOST_AUTO_TEST_CASE(test_is_operator) {
  BOOST_CHECK(petrenko::isOperator("+"));
  BOOST_CHECK(petrenko::isOperator("gcd"));
  BOOST_CHECK(!petrenko::isOperator("123"));
}

BOOST_AUTO_TEST_CASE(test_is_number) {
  BOOST_CHECK(petrenko::isNumber("123"));
  BOOST_CHECK(petrenko::isNumber("-456"));
  BOOST_CHECK(!petrenko::isNumber(""));
  BOOST_CHECK(!petrenko::isNumber("12a"));
}

BOOST_AUTO_TEST_CASE(test_apply_operator) {
  BOOST_CHECK_EQUAL(petrenko::applyOperator("+", 5, 3), 8);
  BOOST_CHECK_EQUAL(petrenko::applyOperator("*", 6, 7), 42);
  BOOST_CHECK_EQUAL(petrenko::applyOperator("gcd", 12, 18), 6);
  BOOST_CHECK_THROW(petrenko::applyOperator("/", 10, 0), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_tokenize_line) {
  petrenko::Queue<std::string> tokens = petrenko::tokenizeLine("10 + 20 * 3");
  BOOST_CHECK_EQUAL(tokens.size(), 5);
  BOOST_CHECK_EQUAL(tokens.pop(), "10");
  BOOST_CHECK_EQUAL(tokens.pop(), "+");
  BOOST_CHECK_EQUAL(tokens.pop(), "20");
  BOOST_CHECK_EQUAL(tokens.pop(), "*");
  BOOST_CHECK_EQUAL(tokens.pop(), "3");
}

BOOST_AUTO_TEST_CASE(test_infix_to_postfix) {
  petrenko::Queue<std::string> infix;
  infix.push("3");
  infix.push("+");
  infix.push("4");
  infix.push("*");
  infix.push("2");
  petrenko::Queue<std::string> postfix = petrenko::infixToPostfix(infix);
  BOOST_CHECK_EQUAL(postfix.pop(), "3");
  BOOST_CHECK_EQUAL(postfix.pop(), "4");
  BOOST_CHECK_EQUAL(postfix.pop(), "2");
  BOOST_CHECK_EQUAL(postfix.pop(), "*");
  BOOST_CHECK_EQUAL(postfix.pop(), "+");
}

BOOST_AUTO_TEST_CASE(test_evaluate_postfix) {
  petrenko::Queue<std::string> postfix;
  postfix.push("3");
  postfix.push("4");
  postfix.push("2");
  postfix.push("*");
  postfix.push("+");
  BOOST_CHECK_EQUAL(petrenko::evaluatePostfix(postfix), 11);
}

BOOST_AUTO_TEST_CASE(test_integration) {
  petrenko::Queue<std::string> tokens = petrenko::tokenizeLine("( 3 + 4 ) * 2");
  petrenko::Queue<std::string> postfix = petrenko::infixToPostfix(tokens);
  long long result = petrenko::evaluatePostfix(postfix);
  BOOST_CHECK_EQUAL(result, 14);
}

BOOST_AUTO_TEST_CASE(test_queue_push_front_back) {
  petrenko::Queue<int> q;
  q.push(10);
  BOOST_CHECK_EQUAL(q.front(), 10);
  BOOST_CHECK_EQUAL(q.back(), 10);
  BOOST_CHECK_EQUAL(q.size(), 1);

  q.push(20);
  BOOST_CHECK_EQUAL(q.front(), 10);
  BOOST_CHECK_EQUAL(q.back(), 20);
  BOOST_CHECK_EQUAL(q.size(), 2);

  q.push(30);
  BOOST_CHECK_EQUAL(q.front(), 10);
  BOOST_CHECK_EQUAL(q.back(), 30);
  BOOST_CHECK_EQUAL(q.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_queue_pop_fifo) {
  petrenko::Queue<int> q;
  q.push(1);
  q.push(2);
  q.push(3);
  q.push(4);

  BOOST_CHECK_EQUAL(q.pop(), 1);
  BOOST_CHECK_EQUAL(q.pop(), 2);
  BOOST_CHECK_EQUAL(q.pop(), 3);
  BOOST_CHECK_EQUAL(q.pop(), 4);

  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(test_queue_empty) {
  petrenko::Queue<int> q;
  BOOST_CHECK(q.empty());

  q.push(42);
  BOOST_CHECK(!q.empty());

  q.pop();
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(test_queue_size) {
  petrenko::Queue<int> q;
  BOOST_CHECK_EQUAL(q.size(), 0);

  for (int i = 0; i < 100; ++i) {
    q.push(i);
  }
  BOOST_CHECK_EQUAL(q.size(), 100);

  for (int i = 0; i < 50; ++i) {
    q.pop();
  }
  BOOST_CHECK_EQUAL(q.size(), 50);
  BOOST_CHECK_EQUAL(q.front(), 50);
  BOOST_CHECK_EQUAL(q.back(), 99);
}

BOOST_AUTO_TEST_CASE(test_queue_clear) {
  petrenko::Queue<std::string> q;
  q.push("first");
  q.push("second");
  q.push("third");

  BOOST_CHECK_EQUAL(q.size(), 3);
  q.clear();
  BOOST_CHECK(q.empty());
  BOOST_CHECK_EQUAL(q.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_queue_large) {
  petrenko::Queue<int> q;
  const int N = 10000;

  for (int i = 0; i < N; ++i) {
    q.push(i);
  }

  BOOST_CHECK_EQUAL(q.size(), N);
  BOOST_CHECK_EQUAL(q.front(), 0);
  BOOST_CHECK_EQUAL(q.back(), N - 1);

  for (int i = 0; i < N; ++i) {
    BOOST_CHECK_EQUAL(q.pop(), i);
  }

  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(test_stack_push_top) {
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

BOOST_AUTO_TEST_CASE(test_stack_pop_lifo) {
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

BOOST_AUTO_TEST_CASE(test_stack_empty) {
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

BOOST_AUTO_TEST_CASE(test_stack_large) {
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

BOOST_AUTO_TEST_CASE(test_stack_exception) {
  petrenko::Stack<int> s;
  BOOST_REQUIRE_THROW(s.pop(), std::runtime_error);
  BOOST_REQUIRE_THROW(s.top(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_stack_move) {
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

BOOST_AUTO_TEST_CASE(test_stack_copy) {
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

BOOST_AUTO_TEST_CASE(test_stack_compare) {
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
