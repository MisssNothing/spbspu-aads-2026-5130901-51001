#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(test_insert_and_access) {
  petrenko::List<int> list;
  list.insert(10, 0);
  list.insert(20, 1);
  list.insert(30, 2);

  BOOST_TEST(list.getSize() == 3);
  BOOST_TEST(list[0] == 10);
  BOOST_TEST(list[1] == 20);
  BOOST_TEST(list[2] == 30);
}

BOOST_AUTO_TEST_CASE(test_insert_at_beginning) {
  petrenko::List<int> list;
  list.insert(10, 0);
  list.insert(20, 0);
  list.insert(30, 0);

  BOOST_TEST(list.getSize() == 3);
  BOOST_TEST(list[0] == 30);
  BOOST_TEST(list[1] == 20);
  BOOST_TEST(list[2] == 10);
}

BOOST_AUTO_TEST_CASE(test_remove) {
  petrenko::List<int> list;
  list.insert(10, 0);
  list.insert(20, 1);
  list.insert(30, 2);

  list.removeAt(1);
  BOOST_TEST(list.getSize() == 2);
  BOOST_TEST(list[0] == 10);
  BOOST_TEST(list[1] == 30);

  list.removeAt(0);
  BOOST_TEST(list.getSize() == 1);
  BOOST_TEST(list[0] == 30);
}

BOOST_AUTO_TEST_CASE(test_clear) {
  petrenko::List<int> list;
  list.insert(10, 0);
  list.insert(20, 1);
  list.clear();

  BOOST_TEST(list.getSize() == 0);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor) {
  petrenko::List<int> list1;
  list1.insert(10, 0);
  list1.insert(20, 1);

  petrenko::List<int> list2(list1);
  BOOST_TEST(list2.getSize() == 2);
  BOOST_TEST(list2[0] == 10);
  BOOST_TEST(list2[1] == 20);
}

BOOST_AUTO_TEST_CASE(test_assignment_operator) {
  petrenko::List<int> list1;
  list1.insert(10, 0);
  list1.insert(20, 1);

  petrenko::List<int> list2;
  list2 = list1;

  BOOST_TEST(list2.getSize() == 2);
  BOOST_TEST(list2[0] == 10);
  BOOST_TEST(list2[1] == 20);
}

BOOST_AUTO_TEST_CASE(test_iterator) {
  petrenko::List<int> list;
  list.insert(10, 0);
  list.insert(20, 1);
  list.insert(30, 2);

  int expected[] = {10, 20, 30};
  int index = 0;

  for (petrenko::LIter<int> it = list.begin(); it != list.end(); ++it) {
    BOOST_TEST(*it == expected[index++]);
  }
}

BOOST_AUTO_TEST_SUITE_END()
