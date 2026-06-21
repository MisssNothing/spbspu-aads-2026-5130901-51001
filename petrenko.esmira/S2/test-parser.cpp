#define BOOST_TEST_MODULE ParserTests
#include <boost/test/included/unit_test.hpp>
#include "parser.hpp"
#include <string>

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
