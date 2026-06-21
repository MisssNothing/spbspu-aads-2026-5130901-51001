#define BOOST_TEST_MODULE CalculatorTests
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include <string>

int runCalculator(std::istream& input, std::ostream& output);

std::string runProgram(const std::string& input) {
  std::istringstream fakeCin(input);
  std::ostringstream fakeCout;
  runCalculator(fakeCin, fakeCout);
  return fakeCout.str();
}

BOOST_AUTO_TEST_CASE(test_simple_addition) {
  std::string output = runProgram("2 + 3\n");
  BOOST_CHECK_EQUAL(output, "5\n");
}

BOOST_AUTO_TEST_CASE(test_simple_subtraction) {
  std::string output = runProgram("10 - 4\n");
  BOOST_CHECK_EQUAL(output, "6\n");
}

BOOST_AUTO_TEST_CASE(test_simple_multiplication) {
  std::string output = runProgram("3 * 5\n");
  BOOST_CHECK_EQUAL(output, "15\n");
}

BOOST_AUTO_TEST_CASE(test_simple_division) {
  std::string output = runProgram("20 / 4\n");
  BOOST_CHECK_EQUAL(output, "5\n");
}

BOOST_AUTO_TEST_CASE(test_operator_precedence) {
  std::string output = runProgram("2 + 3 * 4\n");
  BOOST_CHECK_EQUAL(output, "14\n");
}

BOOST_AUTO_TEST_CASE(test_parentheses) {
  std::string output = runProgram("( 2 + 3 ) * 4\n");
  BOOST_CHECK_EQUAL(output, "20\n");
}

BOOST_AUTO_TEST_CASE(test_complex_expression) {
  std::string output = runProgram("10 - 2 * 3 + 1\n");
  BOOST_CHECK_EQUAL(output, "5\n");
}

BOOST_AUTO_TEST_CASE(test_gcd) {
  std::string output = runProgram("gcd 12 18\n");
  BOOST_CHECK_EQUAL(output, "6\n");
}

BOOST_AUTO_TEST_CASE(test_gcd_large) {
  std::string output = runProgram("gcd 100 25\n");
  BOOST_CHECK_EQUAL(output, "25\n");
}

BOOST_AUTO_TEST_CASE(test_multiple_expressions) {
  std::string input = "2 + 3\n5 * 6\n10 - 4\n";
  std::string output = runProgram(input);
  BOOST_CHECK_EQUAL(output, "5 30 6\n");
}

BOOST_AUTO_TEST_CASE(test_overflow_addition) {
  std::string output = runProgram("9223372036854775807 + 1\n");
  BOOST_CHECK_EQUAL(output, "");
}

BOOST_AUTO_TEST_CASE(test_overflow_multiplication) {
  std::string output = runProgram("9223372036854775807 * 2\n");
  BOOST_CHECK_EQUAL(output, "");
}

BOOST_AUTO_TEST_CASE(test_division_by_zero) {
  std::string output = runProgram("10 / 0\n");
  BOOST_CHECK_EQUAL(output, "");
}

BOOST_AUTO_TEST_CASE(test_invalid_expression) {
  std::string output = runProgram("2 + \n");
  BOOST_CHECK_EQUAL(output, "");
}

BOOST_AUTO_TEST_CASE(test_mismatched_parentheses) {
  std::string output = runProgram("( 2 + 3\n");
  BOOST_CHECK_EQUAL(output, "");
}
