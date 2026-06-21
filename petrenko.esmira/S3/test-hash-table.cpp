#define BOOST_TEST_MODULE HashTableTests
#include <boost/test/included/unit_test.hpp>
#include "hash-table.hpp"
#include <string>

BOOST_AUTO_TEST_CASE(testAddAndHas) {
  petrenko::HashTable<std::string, int,
                      std::hash<std::string>,
                      std::equal_to<std::string> > t;

  t.add("key1", 100);
  BOOST_CHECK(t.has("key1"));
  BOOST_CHECK(!t.has("key2"));
}

BOOST_AUTO_TEST_CASE(testAddAndGet) {
  petrenko::HashTable<std::string, int,
                      std::hash<std::string>,
                      std::equal_to<std::string> > t;

  t.add("key1", 100);
  int val;
  BOOST_CHECK(t.get("key1", val));
  BOOST_CHECK_EQUAL(val, 100);
}

BOOST_AUTO_TEST_CASE(testUpdateValue) {
  petrenko::HashTable<std::string, int,
                      std::hash<std::string>,
                      std::equal_to<std::string> > t;

  t.add("key1", 100);
  t.add("key1", 200);

  int val;
  t.get("key1", val);
  BOOST_CHECK_EQUAL(val, 200);
}

BOOST_AUTO_TEST_CASE(testDrop) {
  petrenko::HashTable<std::string, int,
                      std::hash<std::string>,
                      std::equal_to<std::string> > t;

  t.add("key1", 100);
  t.add("key2", 200);

  int dropped;
  BOOST_CHECK(t.drop("key1", dropped));
  BOOST_CHECK_EQUAL(dropped, 100);
  BOOST_CHECK(!t.has("key1"));
  BOOST_CHECK(t.has("key2"));
  BOOST_CHECK_EQUAL(t.size(), 1);
}

BOOST_AUTO_TEST_CASE(testTombstoneCount) {
  petrenko::HashTable<std::string, int,
                      std::hash<std::string>,
                      std::equal_to<std::string> > t(8);

  t.add("a", 1);
  t.add("b", 2);
  t.add("c", 3);

  int val;
  t.drop("b", val);
  BOOST_CHECK_EQUAL(t.tombstoneCount(), 1);

  t.add("d", 4);
  BOOST_CHECK_EQUAL(t.tombstoneCount(), 0);
  BOOST_CHECK_EQUAL(t.size(), 3);
}

BOOST_AUTO_TEST_CASE(testRehash) {
  petrenko::HashTable<std::string, int,
                      std::hash<std::string>,
                      std::equal_to<std::string> > t(2);

  t.add("a", 1);
  t.add("b", 2);
  t.add("c", 3);

  BOOST_CHECK(t.capacity() > 2);
  BOOST_CHECK(t.has("a"));
  BOOST_CHECK(t.has("b"));
  BOOST_CHECK(t.has("c"));
}

BOOST_AUTO_TEST_CASE(testManualRehash) {
  petrenko::HashTable<std::string, int,
                      std::hash<std::string>,
                      std::equal_to<std::string> > t(4);

  t.add("a", 1);
  t.add("b", 2);

  BOOST_CHECK_EQUAL(t.capacity(), 4);

  t.rehash(16);
  BOOST_CHECK_EQUAL(t.capacity(), 16);
  BOOST_CHECK(t.has("a"));
  BOOST_CHECK(t.has("b"));
}

BOOST_AUTO_TEST_CASE(testCleanUp) {
  petrenko::HashTable<std::string, int,
                      std::hash<std::string>,
                      std::equal_to<std::string> > t(8);

  t.add("a", 1);
  t.add("b", 2);
  t.add("c", 3);

  int val;
  t.drop("a", val);
  t.drop("b", val);

  BOOST_CHECK_EQUAL(t.tombstoneCount(), 2);
  BOOST_CHECK_EQUAL(t.size(), 1);

  t.cleanUp();
  BOOST_CHECK_EQUAL(t.tombstoneCount(), 0);
  BOOST_CHECK_EQUAL(t.size(), 1);
  BOOST_CHECK(t.has("c"));
}

BOOST_AUTO_TEST_CASE(testIterator) {
  petrenko::HashTable<std::string, int,
                      std::hash<std::string>,
                      std::equal_to<std::string> > t;

  t.add("a", 1);
  t.add("b", 2);
  t.add("c", 3);

  int sum = 0;
  for (petrenko::HashTable<std::string, int,
                           std::hash<std::string>,
                           std::equal_to<std::string> >::Iterator it = t.begin();
       it != t.end(); ++it) {
    std::pair<std::string, int> p = *it;
    sum += p.second;
  }
  BOOST_CHECK_EQUAL(sum, 6);
}

BOOST_AUTO_TEST_CASE(testEmptyTable) {
  petrenko::HashTable<std::string, int,
                      std::hash<std::string>,
                      std::equal_to<std::string> > t;

  BOOST_CHECK_EQUAL(t.size(), 0);
  BOOST_CHECK(!t.has("anything"));
}

BOOST_AUTO_TEST_CASE(testManyElements) {
  petrenko::HashTable<std::string, int,
                      std::hash<std::string>,
                      std::equal_to<std::string> > t(4);

  for (int i = 0; i < 100; ++i) {
    std::string key = "key" + std::to_string(i);
    t.add(key, i);
  }

  BOOST_CHECK_EQUAL(t.size(), 100);

  for (int i = 0; i < 100; ++i) {
    std::string key = "key" + std::to_string(i);
    BOOST_CHECK(t.has(key));
  }
}

BOOST_AUTO_TEST_CASE(testCollisionResolution) {
  petrenko::HashTable<int, std::string,
                      std::hash<int>,
                      std::equal_to<int> > t(10);

  t.add(0, "zero");
  t.add(10, "ten");
  t.add(20, "twenty");

  BOOST_CHECK(t.has(0));
  BOOST_CHECK(t.has(10));
  BOOST_CHECK(t.has(20));

  std::string val;
  t.get(10, val);
  BOOST_CHECK_EQUAL(val, "ten");
}
