#define BOOST_TEST_MODULE GraphTests
#include <boost/test/included/unit_test.hpp>
#include "graph.hpp"
#include <string>
#include <vector>

BOOST_AUTO_TEST_CASE(testGraphCreation) {
  petrenko::Graph g("test");

  BOOST_CHECK_EQUAL(g.getName(), "test");
  BOOST_CHECK_EQUAL(g.getVertices().size(), 0);
}

BOOST_AUTO_TEST_CASE(testAddEdge) {
  petrenko::Graph g("test");

  g.addEdge("a", "b", 40);

  std::vector<std::string> vertices = g.getVertices();
  BOOST_CHECK_EQUAL(vertices.size(), 2);
  BOOST_CHECK(vertices[0] == "a" || vertices[0] == "b");
  BOOST_CHECK(vertices[1] == "a" || vertices[1] == "b");

  BOOST_CHECK(g.hasVertex("a"));
  BOOST_CHECK(g.hasVertex("b"));
  BOOST_CHECK(g.hasEdge("a", "b", 40));
}

BOOST_AUTO_TEST_CASE(testMultipleEdges) {
  petrenko::Graph g("test");

  g.addEdge("a", "b", 40);
  g.addEdge("a", "b", 30);
  g.addEdge("a", "c", 50);

  std::vector<std::pair<std::string, std::vector<unsigned int> > > out =
      g.getOutbound("a");
  BOOST_CHECK_EQUAL(out.size(), 1);
  BOOST_CHECK_EQUAL(out[0].second.size(), 3);

  std::vector<unsigned int> expected;
  expected.push_back(30);
  expected.push_back(40);
  expected.push_back(50);
  BOOST_CHECK_EQUAL_COLLECTIONS(out[0].second.begin(), out[0].second.end(),
                                expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(testOutbound) {
  petrenko::Graph g("test");

  g.addEdge("a", "b", 40);
  g.addEdge("b", "c", 50);
  g.addEdge("c", "a", 30);
  g.addEdge("c", "b", 20);

  std::vector<std::pair<std::string, std::vector<unsigned int> > > out =
      g.getOutbound("c");
  BOOST_CHECK_EQUAL(out.size(), 1);
  BOOST_CHECK_EQUAL(out[0].first, "c");

  std::vector<unsigned int> expected;
  expected.push_back(20);
  expected.push_back(30);
  BOOST_CHECK_EQUAL_COLLECTIONS(out[0].second.begin(), out[0].second.end(),
                                expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(testInbound) {
  petrenko::Graph g("test");

  g.addEdge("a", "b", 40);
  g.addEdge("b", "c", 50);
  g.addEdge("c", "b", 20);

  std::vector<std::pair<std::string, std::vector<unsigned int> > > in =
      g.getInbound("b");
  BOOST_CHECK_EQUAL(in.size(), 1);
  BOOST_CHECK_EQUAL(in[0].first, "b");

  std::vector<unsigned int> expected;
  expected.push_back(20);
  expected.push_back(40);
  BOOST_CHECK_EQUAL_COLLECTIONS(in[0].second.begin(), in[0].second.end(),
                                expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(testRemoveEdge) {
  petrenko::Graph g("test");

  g.addEdge("a", "b", 40);
  g.addEdge("a", "b", 30);

  BOOST_CHECK(g.hasEdge("a", "b", 40));
  BOOST_CHECK(g.hasEdge("a", "b", 30));

  g.removeEdge("a", "b", 40);

  BOOST_CHECK(!g.hasEdge("a", "b", 40));
  BOOST_CHECK(g.hasEdge("a", "b", 30));
}

BOOST_AUTO_TEST_CASE(testRemoveNonExistentEdge) {
  petrenko::Graph g("test");

  g.addEdge("a", "b", 40);

  bool result = g.removeEdge("a", "b", 999);
  BOOST_CHECK(!result);
  BOOST_CHECK(g.hasEdge("a", "b", 40));
}

BOOST_AUTO_TEST_CASE(testHasVertex) {
  petrenko::Graph g("test");

  g.addEdge("a", "b", 40);

  BOOST_CHECK(g.hasVertex("a"));
  BOOST_CHECK(g.hasVertex("b"));
  BOOST_CHECK(!g.hasVertex("c"));
}

BOOST_AUTO_TEST_CASE(testGetVerticesSorted) {
  petrenko::Graph g("test");

  g.addEdge("c", "d", 10);
  g.addEdge("a", "b", 20);
  g.addEdge("e", "f", 30);

  std::vector<std::string> vertices = g.getVertices();
  BOOST_CHECK_EQUAL(vertices.size(), 6);

  for (size_t i = 0; i < vertices.size() - 1; ++i) {
    BOOST_CHECK(vertices[i] < vertices[i + 1]);
  }
}

BOOST_AUTO_TEST_CASE(testAddVertices) {
  petrenko::Graph g("test");

  std::vector<std::string> verts;
  verts.push_back("x");
  verts.push_back("y");
  verts.push_back("z");

  g.addVertices(verts);

  BOOST_CHECK(g.hasVertex("x"));
  BOOST_CHECK(g.hasVertex("y"));
  BOOST_CHECK(g.hasVertex("z"));
  BOOST_CHECK_EQUAL(g.getVertices().size(), 3);
}

BOOST_AUTO_TEST_CASE(testDuplicateVertices) {
  petrenko::Graph g("test");

  g.addEdge("a", "b", 10);
  g.addEdge("a", "c", 20);
  g.addEdge("d", "a", 30);

  std::vector<std::string> vertices = g.getVertices();
  BOOST_CHECK_EQUAL(vertices.size(), 4);
}
