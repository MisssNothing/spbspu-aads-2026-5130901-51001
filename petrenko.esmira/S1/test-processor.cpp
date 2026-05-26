#include <boost/test/unit_test.hpp>
#include <sstream>
#include "processor.hpp"

BOOST_AUTO_TEST_SUITE(ProcessorTests)

BOOST_AUTO_TEST_CASE(test_single_line) {
  std::istringstream input("title1 1 2 3\n");
  petrenko::ProcessResult result = petrenko::processInput(input);

  BOOST_TEST(result.titles.getSize() == 1);
  BOOST_TEST(result.titles[0] == "title1");
  BOOST_TEST(result.numbers.getSize() == 1);
  BOOST_TEST(result.numbers[0].getSize() == 3);
  BOOST_TEST(result.numbers[0][0] == 1);
  BOOST_TEST(result.numbers[0][1] == 2);
  BOOST_TEST(result.numbers[0][2] == 3);
  BOOST_TEST(!result.overflow);
}

BOOST_AUTO_TEST_CASE(test_multiple_lines) {
  std::istringstream input("title1 1 2\n title2 3 4 5\n title3 6\n");
  petrenko::ProcessResult result = petrenko::processInput(input);

  BOOST_TEST(result.titles.getSize() == 3);
  BOOST_TEST(result.titles[0] == "title1");
  BOOST_TEST(result.titles[1] == "title2");
  BOOST_TEST(result.titles[2] == "title3");

  BOOST_TEST(result.numbers[0].getSize() == 2);
  BOOST_TEST(result.numbers[0][0] == 1);
  BOOST_TEST(result.numbers[0][1] == 2);

  BOOST_TEST(result.numbers[1].getSize() == 3);
  BOOST_TEST(result.numbers[1][0] == 3);
  BOOST_TEST(result.numbers[1][1] == 4);
  BOOST_TEST(result.numbers[1][2] == 5);

  BOOST_TEST(result.numbers[2].getSize() == 1);
  BOOST_TEST(result.numbers[2][0] == 6);
}

BOOST_AUTO_TEST_CASE(test_empty_lines) {
  std::istringstream input("\n\ntitle1 1 2\n\n");
  petrenko::ProcessResult result = petrenko::processInput(input);

  BOOST_TEST(result.titles.getSize() == 1);
  BOOST_TEST(result.titles[0] == "title1");
}

BOOST_AUTO_TEST_CASE(test_print_results) {
  std::istringstream input("A 10 20\nB 30\nC 40 50 60\n");
  petrenko::ProcessResult result = petrenko::processInput(input);

  std::ostringstream output;
  std::ostringstream error;
  petrenko::printResults(output, error, result);

  std::string expectedOutput = "A B C\n10 30 40\n20 50\n60\n80 70 60\n";
  BOOST_TEST(output.str() == expectedOutput);
  BOOST_TEST(error.str().empty());
  BOOST_TEST(!result.overflow);
}

BOOST_AUTO_TEST_CASE(test_empty_input) {
  std::istringstream input("");
  petrenko::ProcessResult result = petrenko::processInput(input);

  std::ostringstream output;
  std::ostringstream error;
  petrenko::printResults(output, error, result);

  BOOST_TEST(output.str() == "0\n");
  BOOST_TEST(error.str().empty());
}

BOOST_AUTO_TEST_CASE(test_numbers_with_spaces) {
  std::istringstream input("title  1   2    3  4\n");
  petrenko::ProcessResult result = petrenko::processInput(input);

  BOOST_TEST(result.numbers[0].getSize() == 4);
  BOOST_TEST(result.numbers[0][0] == 1);
  BOOST_TEST(result.numbers[0][1] == 2);
  BOOST_TEST(result.numbers[0][2] == 3);
  BOOST_TEST(result.numbers[0][3] == 4);
}

BOOST_AUTO_TEST_SUITE_END()
