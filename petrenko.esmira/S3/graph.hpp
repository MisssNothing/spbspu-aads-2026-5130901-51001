#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "hash-table.hpp"
#include <string>
#include <vector>

namespace petrenko {

class Graph {
private:
  std::string name_;
  HashTable<std::string, std::vector<unsigned int>,
            std::hash<std::string>, std::equal_to<std::string> > outgoing_;
  HashTable<std::string, std::vector<unsigned int>,
            std::hash<std::string>, std::equal_to<std::string> > incoming_;
  std::vector<std::string> vertices_;

  bool hasVertexInVector(const std::string& vertex) const {
    for (size_t i = 0; i < vertices_.size(); ++i) {
      if (vertices_[i] == vertex) {
        return true;
      }
    }
    return false;
  }

  void addVertexToVector(const std::string& vertex) {
    if (!hasVertexInVector(vertex)) {
      vertices_.push_back(vertex);
    }
  }

  void sortVector(std::vector<std::string>& vec) const {
    for (size_t i = 0; i < vec.size(); ++i) {
      for (size_t j = i + 1; j < vec.size(); ++j) {
        if (vec[i] > vec[j]) {
          std::string temp = vec[i];
          vec[i] = vec[j];
          vec[j] = temp;
        }
      }
    }
  }

  void sortVectorOfWeights(std::vector<unsigned int>& vec) const {
    for (size_t i = 0; i < vec.size(); ++i) {
      for (size_t j = i + 1; j < vec.size(); ++j) {
        if (vec[i] > vec[j]) {
          unsigned int temp = vec[i];
          vec[i] = vec[j];
          vec[j] = temp;
        }
      }
    }
  }

  bool findWeightInVector(const std::vector<unsigned int>& vec,
                          unsigned int weight) const {
    for (size_t i = 0; i < vec.size(); ++i) {
      if (vec[i] == weight) {
        return true;
      }
    }
    return false;
  }

  void addToMap(HashTable<std::string, std::vector<unsigned int>,
                std::hash<std::string>, std::equal_to<std::string> >& map,
                const std::string& vertex, unsigned int weight) {
    std::vector<unsigned int> edges;
    if (map.get(vertex, edges)) {
      edges.push_back(weight);
      sortVectorOfWeights(edges);
      map.add(vertex, edges);
    } else {
      std::vector<unsigned int> new_edges;
      new_edges.push_back(weight);
      map.add(vertex, new_edges);
    }
  }

  bool removeFromMap(HashTable<std::string, std::vector<unsigned int>,
                     std::hash<std::string>, std::equal_to<std::string> >& map,
                     const std::string& vertex, unsigned int weight) {
    std::vector<unsigned int> edges;
    if (!map.get(vertex, edges)) {
      return false;
    }

    bool found = false;
    size_t index = 0;
    for (size_t i = 0; i < edges.size(); ++i) {
      if (edges[i] == weight) {
        found = true;
        index = i;
        break;
      }
    }

    if (!found) {
      return false;
    }

    for (size_t i = index; i < edges.size() - 1; ++i) {
      edges[i] = edges[i + 1];
    }
    edges.pop_back();

    if (edges.empty()) {
      map.drop(vertex, edges);
    } else {
      map.add(vertex, edges);
    }
    return true;
  }

public:
  Graph(const std::string& name) : name_(name), outgoing_(32), incoming_(32) {}

  const std::string& getName() const {
    return name_;
  }

  void addEdge(const std::string& from, const std::string& to,
               unsigned int weight) {
    addVertexToVector(from);
    addVertexToVector(to);
    addToMap(outgoing_, from, weight);
    addToMap(incoming_, to, weight);
  }

  bool removeEdge(const std::string& from, const std::string& to,
                  unsigned int weight) {
    if (!hasEdge(from, weight)) {
      return false;
    }
    bool out_ok = removeFromMap(outgoing_, from, weight);
    bool in_ok = removeFromMap(incoming_, to, weight);
    return out_ok && in_ok;
  }

  std::vector<std::string> getVertices() const {
    std::vector<std::string> result = vertices_;
    for (size_t i = 0; i < result.size(); ++i) {
      for (size_t j = i + 1; j < result.size(); ++j) {
        if (result[i] > result[j]) {
          std::string temp = result[i];
          result[i] = result[j];
          result[j] = temp;
        }
      }
    }
    return result;
  }

  std::vector<std::pair<std::string, std::vector<unsigned int> > >
  getOutbound(const std::string& vertex) const {
    std::vector<std::pair<std::string, std::vector<unsigned int> > > result;

    if (!hasVertexInVector(vertex)) {
      return result;
    }

    std::vector<unsigned int> edges;
    if (outgoing_.get(vertex, edges)) {
      result.push_back(std::pair<std::string, std::vector<unsigned int> >(vertex, edges));
    }

    return result;
  }

  std::vector<std::pair<std::string, std::vector<unsigned int> > >
  getInbound(const std::string& vertex) const {
    std::vector<std::pair<std::string, std::vector<unsigned int> > > result;

    if (!hasVertexInVector(vertex)) {
      return result;
    }

    std::vector<unsigned int> edges;
    if (incoming_.get(vertex, edges)) {
      result.push_back(std::pair<std::string, std::vector<unsigned int> >(vertex, edges));
    }

    return result;
  }

  bool hasVertex(const std::string& vertex) const {
    return hasVertexInVector(vertex);
  }

  bool hasEdge(const std::string& from,
               unsigned int weight) const {
    std::vector<unsigned int> edges;
    if (!outgoing_.get(from, edges)) {
      return false;
    }
    return findWeightInVector(edges, weight);
  }

  void addVertices(const std::vector<std::string>& vertices) {
    for (size_t i = 0; i < vertices.size(); ++i) {
      addVertexToVector(vertices[i]);
    }
  }
};

}

#endif
