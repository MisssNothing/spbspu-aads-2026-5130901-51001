#define BOOST_TEST_MODULE BSTreeTests
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include "bstree.hpp"

BOOST_AUTO_TEST_SUITE(BSTreeTests)

BOOST_AUTO_TEST_CASE(TestPushAndGet) {
  petrenko::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(1, "one");
  tree.push(9, "nine");

  BOOST_CHECK_EQUAL(tree.get(5), "five");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
  BOOST_CHECK_EQUAL(tree.get(7), "seven");
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(9), "nine");
  BOOST_CHECK_EQUAL(tree.size(), 5);
}

BOOST_AUTO_TEST_CASE(TestPushUpdateExisting) {
  petrenko::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(5, "FIVE");
  BOOST_CHECK_EQUAL(tree.get(5), "FIVE");
  BOOST_CHECK_EQUAL(tree.size(), 1);
}

BOOST_AUTO_TEST_CASE(TestGetNonexistent) {
  petrenko::BSTree<int, std::string> tree;
  tree.push(5, "five");
  BOOST_CHECK_THROW(tree.get(10), const char*);
}

BOOST_AUTO_TEST_CASE(TestDropLeaf) {
  petrenko::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");

  std::string val = tree.drop(3);
  BOOST_CHECK_EQUAL(val, "three");
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_THROW(tree.get(3), const char*);
  BOOST_CHECK_EQUAL(tree.get(5), "five");
  BOOST_CHECK_EQUAL(tree.get(7), "seven");
}

BOOST_AUTO_TEST_CASE(TestDropRoot) {
  petrenko::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");

  std::string val = tree.drop(5);
  BOOST_CHECK_EQUAL(val, "five");
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_THROW(tree.get(5), const char*);
  BOOST_CHECK_EQUAL(tree.get(3), "three");
  BOOST_CHECK_EQUAL(tree.get(7), "seven");
}

BOOST_AUTO_TEST_CASE(TestDropNodeWithOneChild) {
  petrenko::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(1, "one");

  std::string val = tree.drop(3);
  BOOST_CHECK_EQUAL(val, "three");
  BOOST_CHECK_EQUAL(tree.size(), 3);
  BOOST_CHECK_THROW(tree.get(3), const char*);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(5), "five");
  BOOST_CHECK_EQUAL(tree.get(7), "seven");
}

BOOST_AUTO_TEST_CASE(TestEmpty) {
  petrenko::BSTree<int, std::string> tree;
  BOOST_CHECK(tree.empty());
  tree.push(5, "five");
  BOOST_CHECK(!tree.empty());
  tree.drop(5);
  BOOST_CHECK(tree.empty());
}

BOOST_AUTO_TEST_CASE(TestSize) {
  petrenko::BSTree<int, std::string> tree;
  BOOST_CHECK_EQUAL(tree.size(), 0);
  tree.push(5, "five");
  BOOST_CHECK_EQUAL(tree.size(), 1);
  tree.push(3, "three");
  BOOST_CHECK_EQUAL(tree.size(), 2);
  tree.drop(5);
  BOOST_CHECK_EQUAL(tree.size(), 1);
  tree.clear();
  BOOST_CHECK_EQUAL(tree.size(), 0);
}

BOOST_AUTO_TEST_CASE(TestIterator) {
  petrenko::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(1, "one");
  tree.push(9, "nine");

  std::vector<int> keys;
  for (petrenko::BSTree<int, std::string>::iterator it = tree.begin(); it != tree.end(); ++it) {
    std::pair<const int&, std::string&> pair = *it;
    keys.push_back(pair.first);
  }

  std::vector<int> expected_keys = {1, 3, 5, 7, 9};
  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(), expected_keys.begin(), expected_keys.end());
}

BOOST_AUTO_TEST_CASE(TestFind) {
  petrenko::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");

  petrenko::BSTree<int, std::string>::iterator it = tree.find(3);
  BOOST_CHECK(it != tree.end());
  std::pair<const int&, std::string&> pair = *it;
  BOOST_CHECK_EQUAL(pair.first, 3);
  BOOST_CHECK_EQUAL(pair.second, "three");

  it = tree.find(10);
  BOOST_CHECK(it == tree.end());
}

BOOST_AUTO_TEST_CASE(TestRotateLeft) {
  petrenko::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(6, "six");
  tree.push(8, "eight");

  petrenko::BSTree<int, std::string>::iterator it = tree.find(5);
  petrenko::BSTree<int, std::string>::const_iterator cit = it;
  tree.rotateLeft(cit);

  std::vector<int> keys;
  for (petrenko::BSTree<int, std::string>::iterator iter = tree.begin(); iter != tree.end(); ++iter) {
    std::pair<const int&, std::string&> pair = *iter;
    keys.push_back(pair.first);
  }

  std::vector<int> expected_keys = {3, 5, 6, 7, 8};
  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(), expected_keys.begin(), expected_keys.end());
}

BOOST_AUTO_TEST_CASE(TestRotateRight) {
  petrenko::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(2, "two");
  tree.push(4, "four");

  petrenko::BSTree<int, std::string>::iterator it = tree.find(5);
  petrenko::BSTree<int, std::string>::const_iterator cit = it;
  tree.rotateRight(cit);

  std::vector<int> keys;
  for (petrenko::BSTree<int, std::string>::iterator iter = tree.begin(); iter != tree.end(); ++iter) {
    std::pair<const int&, std::string&> pair = *iter;
    keys.push_back(pair.first);
  }

  std::vector<int> expected_keys = {2, 3, 4, 5, 7};
  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(), expected_keys.begin(), expected_keys.end());
}

BOOST_AUTO_TEST_CASE(TestHeight) {
  petrenko::BSTree<int, std::string> tree;
  BOOST_CHECK_EQUAL(tree.height(), 0);
  tree.push(5, "five");
  BOOST_CHECK_EQUAL(tree.height(), 1);
  tree.push(3, "three");
  BOOST_CHECK_EQUAL(tree.height(), 2);
  tree.push(7, "seven");
  BOOST_CHECK_EQUAL(tree.height(), 2);
  tree.push(1, "one");
  BOOST_CHECK_EQUAL(tree.height(), 3);
  tree.push(9, "nine");
  BOOST_CHECK_EQUAL(tree.height(), 3);
}

BOOST_AUTO_TEST_CASE(TestCopyConstructor) {
  petrenko::BSTree<int, std::string> tree1;
  tree1.push(5, "five");
  tree1.push(3, "three");
  tree1.push(7, "seven");

  petrenko::BSTree<int, std::string> tree2(tree1);
  BOOST_CHECK_EQUAL(tree2.size(), 3);
  BOOST_CHECK_EQUAL(tree2.get(5), "five");
  BOOST_CHECK_EQUAL(tree2.get(3), "three");
  BOOST_CHECK_EQUAL(tree2.get(7), "seven");

  tree1.push(9, "nine");
  BOOST_CHECK_EQUAL(tree1.size(), 4);
  BOOST_CHECK_EQUAL(tree2.size(), 3);
}

BOOST_AUTO_TEST_CASE(TestAssignmentOperator) {
  petrenko::BSTree<int, std::string> tree1;
  tree1.push(5, "five");
  tree1.push(3, "three");

  petrenko::BSTree<int, std::string> tree2;
  tree2.push(7, "seven");

  tree2 = tree1;
  BOOST_CHECK_EQUAL(tree2.size(), 2);
  BOOST_CHECK_EQUAL(tree2.get(5), "five");
  BOOST_CHECK_EQUAL(tree2.get(3), "three");
  BOOST_CHECK_THROW(tree2.get(7), const char*);
}

BOOST_AUTO_TEST_CASE(TestClear) {
  petrenko::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  BOOST_CHECK_EQUAL(tree.size(), 3);
  tree.clear();
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_THROW(tree.get(5), const char*);
}

BOOST_AUTO_TEST_SUITE_END()
