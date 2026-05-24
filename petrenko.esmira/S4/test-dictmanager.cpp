#define BOOST_TEST_MODULE DictionaryManagerTests
#include <boost/test/included/unit_test.hpp>
#include "dictmanager.hpp"

BOOST_AUTO_TEST_SUITE(DictionaryManagerTests)

BOOST_AUTO_TEST_CASE(TestComplement) {
  petrenko::BSTree<int, std::string> a;
  a.push(1, "name");
  a.push(2, "surname");

  petrenko::BSTree<int, std::string> b;
  b.push(1, "name");
  b.push(4, "mouse");

  petrenko::DictionaryManager manager;
  petrenko::BSTree<int, std::string> result = manager.complement(a, b);

  BOOST_CHECK_EQUAL(result.size(), 1);
  BOOST_CHECK_EQUAL(result.get(2), "surname");
  BOOST_CHECK_THROW(result.get(1), const char*);
  BOOST_CHECK_THROW(result.get(4), const char*);
}

BOOST_AUTO_TEST_CASE(TestIntersect) {
  petrenko::BSTree<int, std::string> a;
  a.push(1, "name");
  a.push(2, "surname");

  petrenko::BSTree<int, std::string> b;
  b.push(1, "name");
  b.push(2, "keyboard");
  b.push(4, "mouse");

  petrenko::DictionaryManager manager;
  petrenko::BSTree<int, std::string> result = manager.intersect(a, b);

  BOOST_CHECK_EQUAL(result.size(), 2);
  BOOST_CHECK_EQUAL(result.get(1), "name");
  BOOST_CHECK_EQUAL(result.get(2), "surname");
  BOOST_CHECK_THROW(result.get(4), const char*);
}

BOOST_AUTO_TEST_CASE(TestUnion) {
  petrenko::BSTree<int, std::string> a;
  a.push(1, "name");
  a.push(2, "surname");

  petrenko::BSTree<int, std::string> b;
  b.push(1, "NAME");
  b.push(4, "mouse");

  petrenko::DictionaryManager manager;
  petrenko::BSTree<int, std::string> result = manager.setUnion(a, b);

  BOOST_CHECK_EQUAL(result.size(), 3);
  BOOST_CHECK_EQUAL(result.get(1), "name");
  BOOST_CHECK_EQUAL(result.get(2), "surname");
  BOOST_CHECK_EQUAL(result.get(4), "mouse");
}

BOOST_AUTO_TEST_CASE(TestComplementEmpty) {
  petrenko::BSTree<int, std::string> a;
  petrenko::BSTree<int, std::string> b;
  b.push(1, "name");

  petrenko::DictionaryManager manager;
  petrenko::BSTree<int, std::string> result = manager.complement(a, b);

  BOOST_CHECK_EQUAL(result.size(), 0);
  BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(TestIntersectEmpty) {
  petrenko::BSTree<int, std::string> a;
  a.push(1, "name");

  petrenko::BSTree<int, std::string> b;

  petrenko::DictionaryManager manager;
  petrenko::BSTree<int, std::string> result = manager.intersect(a, b);

  BOOST_CHECK_EQUAL(result.size(), 0);
  BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(TestUnionEmpty) {
  petrenko::BSTree<int, std::string> a;
  a.push(1, "name");

  petrenko::BSTree<int, std::string> b;

  petrenko::DictionaryManager manager;
  petrenko::BSTree<int, std::string> result = manager.setUnion(a, b);

  BOOST_CHECK_EQUAL(result.size(), 1);
  BOOST_CHECK_EQUAL(result.get(1), "name");
}

BOOST_AUTO_TEST_SUITE_END()
