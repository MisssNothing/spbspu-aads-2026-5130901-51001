#define BOOST_TEST_MODULE OueueTest
#include <boost/test/included/unit_test.hpp>
#include "queue.hpp"
#include <string>

BOOST_AUTO_TEST_CASE(test_push_front_back) {
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

BOOST_AUTO_TEST_CASE(test_drop_fifo) {
  petrenko::Queue<int> q;

  q.push(1);
  q.push(2);
  q.push(3);
  q.push(4);

  BOOST_CHECK_EQUAL(q.drop(), 1);
  BOOST_CHECK_EQUAL(q.drop(), 2);
  BOOST_CHECK_EQUAL(q.drop(), 3);
  BOOST_CHECK_EQUAL(q.drop(), 4);
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(test_empty_queue) {
  petrenko::Queue<int> q;

  BOOST_CHECK(q.empty());

  q.push(42);
  BOOST_CHECK(!q.empty());

  q.drop();
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
    q.drop();
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


BOOST_AUTO_TEST_CASE(test_large_queue) {
  petrenko::Queue<int> q; const int N = 10000;

  for (int i = 0; i < N; ++i) {
    q.push(i);
  }

  BOOST_CHECK_EQUAL(q.size(), N);
  BOOST_CHECK_EQUAL(q.front(), 0);
  BOOST_CHECK_EQUAL(q.back(), N - 1);

  for (int i = 0; i < N; ++i) {
    BOOST_CHECK_EQUAL(q.drop(), i);
  }

  BOOST_CHECK(q.empty());
}
