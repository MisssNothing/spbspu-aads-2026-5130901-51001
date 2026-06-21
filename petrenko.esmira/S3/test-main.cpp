#define BOOST_TEST_MODULE IntegrationTests
#include <boost/test/included/unit_test.hpp>
#include "hash-table.hpp"
#include "graph.hpp"
#include <string>

BOOST_AUTO_TEST_CASE(testGraphInHashTable) {
  petrenko::HashTable<std::string, petrenko::Graph*,
                      std::hash<std::string>,
                      std::equal_to<std::string> > table;

  petrenko::Graph* g1 = new petrenko::Graph("graph1");
  petrenko::Graph* g2 = new petrenko::Graph("graph2");

  table.add("g1", g1);
  table.add("g2", g2);

  petrenko::Graph* retrieved = nullptr;
  BOOST_CHECK(table.get("g1", retrieved));
  BOOST_CHECK_EQUAL(retrieved->getName(), "graph1");

  delete g1;
  delete g2;
}

BOOST_AUTO_TEST_CASE(testGraphAddMultipleEdges) {
  petrenko::Graph g("test");

  g.addEdge("a", "b", 10);
  g.addEdge("a", "b", 20);
  g.addEdge("a", "b", 10);

  std::vector<std::pair<std::string, std::vector<unsigned int> > > out =
      g.getOutbound("a");
  BOOST_CHECK_EQUAL(out[0].second.size(), 3);
}

BOOST_AUTO_TEST_CASE(testGraphRemoveAllEdges) {
  petrenko::Graph g("test");

  g.addEdge("a", "b", 10);
  g.addEdge("a", "b", 20);

  g.removeEdge("a", "b", 10);
  g.removeEdge("a", "b", 20);

  BOOST_CHECK(!g.hasEdge("a", "b", 10));
  BOOST_CHECK(!g.hasEdge("a", "b", 20));
  BOOST_CHECK(g.hasVertex("a"));
  BOOST_CHECK(g.hasVertex("b"));
}
