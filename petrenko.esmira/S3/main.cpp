#include "hash-table.hpp"
#include "graph.hpp"
#include "sha1.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace petrenko {

HashTable<std::string, Graph*, std::hash<std::string>, std::equal_to<std::string> > graphs;

Graph* getGraph(const std::string& name) {
  Graph* graph_ptr = nullptr;
  if (!graphs.get(name, graph_ptr)) {
    return nullptr;
  }
  return graph_ptr;
}

void printInvalid() {
  std::cerr << "<INVALID COMMAND>" << std::endl;
}

void printWeights(const std::vector<unsigned int>& weights) {
  for (size_t i = 0; i < weights.size(); ++i) {
    std::cout << " " << weights[i];
  }
}

void handleGraphs() {
  std::vector<std::string> graph_names;
  for (HashTable<std::string, Graph*,
                 std::hash<std::string>,
                 std::equal_to<std::string> >::Iterator it = graphs.begin();
       it != graphs.end(); ++it) {
    std::pair<std::string, Graph*> p = *it;
    graph_names.push_back(p.first);
  }

  for (size_t i = 0; i < graph_names.size(); ++i) {
    for (size_t j = i + 1; j < graph_names.size(); ++j) {
      if (graph_names[i] > graph_names[j]) {
        std::string temp = graph_names[i];
        graph_names[i] = graph_names[j];
        graph_names[j] = temp;
      }
    }
  }

  for (size_t i = 0; i < graph_names.size(); ++i) {
    std::cout << graph_names[i] << std::endl;
  }
}

void handleVertexes(const std::vector<std::string>& tokens) {
  if (tokens.size() != 2) {
    printInvalid();
    return;
  }

  Graph* graph = getGraph(tokens[1]);
  if (graph == nullptr) {
    printInvalid();
    return;
  }

  std::vector<std::string> vertices = graph->getVertices();
  for (size_t i = 0; i < vertices.size(); ++i) {
    std::cout << vertices[i] << std::endl;
  }
}

void handleOutbound(const std::vector<std::string>& tokens) {
  if (tokens.size() != 3) {
    printInvalid();
    return;
  }

  Graph* graph = getGraph(tokens[1]);
  if (graph == nullptr || !graph->hasVertex(tokens[2])) {
    printInvalid();
    return;
  }

  std::vector<std::pair<std::string, std::vector<unsigned int> > > outbound;
  outbound = graph->getOutbound(tokens[2]);
  for (size_t i = 0; i < outbound.size(); ++i) {
    std::cout << outbound[i].first;
    printWeights(outbound[i].second);
    std::cout << std::endl;
  }
}

void handleInbound(const std::vector<std::string>& tokens) {
  if (tokens.size() != 3) {
    printInvalid();
    return;
  }

  Graph* graph = getGraph(tokens[1]);
  if (graph == nullptr || !graph->hasVertex(tokens[2])) {
    printInvalid();
    return;
  }

  std::vector<std::pair<std::string, std::vector<unsigned int> > > inbound;
  inbound = graph->getInbound(tokens[2]);
  for (size_t i = 0; i < inbound.size(); ++i) {
    std::cout << inbound[i].first;
    printWeights(inbound[i].second);
    std::cout << std::endl;
  }
}

void handleBind(const std::vector<std::string>& tokens) {
  if (tokens.size() != 5) {
    printInvalid();
    return;
  }

  Graph* graph = getGraph(tokens[1]);
  if (graph == nullptr) {
    printInvalid();
    return;
  }

  std::string from = tokens[2];
  std::string to = tokens[3];
  unsigned int weight = static_cast<unsigned int>(std::stoul(tokens[4]));

  graph->addEdge(from, to, weight);
}

void handleCut(const std::vector<std::string>& tokens) {
  if (tokens.size() != 5) {
    printInvalid();
    return;
  }

  Graph* graph = getGraph(tokens[1]);
  if (graph == nullptr) {
    printInvalid();
    return;
  }

  std::string from = tokens[2];
  std::string to = tokens[3];
  unsigned int weight = static_cast<unsigned int>(std::stoul(tokens[4]));

  if (!graph->removeEdge(from, to, weight)) {
    printInvalid();
  }
}

void handleCreate(const std::vector<std::string>& tokens) {
  if (tokens.size() < 2) {
    printInvalid();
    return;
  }

  std::string new_name = tokens[1];

  if (graphs.has(new_name)) {
    printInvalid();
    return;
  }

  Graph* new_graph = new Graph(new_name);

  if (tokens.size() > 2) {
    size_t vertex_count = std::stoul(tokens[2]);
    std::vector<std::string> vertices;
    for (size_t i = 3; i < tokens.size() && i < 3 + vertex_count; ++i) {
      vertices.push_back(tokens[i]);
    }
    new_graph->addVertices(vertices);
  }

  graphs.add(new_name, new_graph);
}

void handleMerge(const std::vector<std::string>& tokens) {
  if (tokens.size() != 4) {
    printInvalid();
    return;
  }

  std::string new_name = tokens[1];
  std::string old1 = tokens[2];
  std::string old2 = tokens[3];

  if (graphs.has(new_name) || !graphs.has(old1) || !graphs.has(old2)) {
    printInvalid();
    return;
  }

  Graph* new_graph = new Graph(new_name);
  Graph* g1 = getGraph(old1);
  Graph* g2 = getGraph(old2);

  std::vector<std::string> verts1 = g1->getVertices();
  std::vector<std::string> verts2 = g2->getVertices();

  for (size_t i = 0; i < verts1.size(); ++i) {
    std::vector<std::string> v(1, verts1[i]);
    new_graph->addVertices(v);
  }
  for (size_t i = 0; i < verts2.size(); ++i) {
    std::vector<std::string> v(1, verts2[i]);
    new_graph->addVertices(v);
  }

  graphs.add(new_name, new_graph);
}

void handleExtract(const std::vector<std::string>& tokens) {
  if (tokens.size() < 4) {
    printInvalid();
    return;
  }

  std::string new_name = tokens[1];
  std::string old_name = tokens[2];
  size_t vertex_count = std::stoul(tokens[3]);

  if (graphs.has(new_name) || !graphs.has(old_name)) {
    printInvalid();
    return;
  }

  std::vector<std::string> vertices_to_extract;
  for (size_t i = 4; i < tokens.size() && i < 4 + vertex_count; ++i) {
    vertices_to_extract.push_back(tokens[i]);
  }

  Graph* old_graph = getGraph(old_name);
  for (size_t i = 0; i < vertices_to_extract.size(); ++i) {
    if (!old_graph->hasVertex(vertices_to_extract[i])) {
      printInvalid();
      return;
    }
  }

  Graph* new_graph = new Graph(new_name);
  new_graph->addVertices(vertices_to_extract);

  graphs.add(new_name, new_graph);
}

void loadGraphsFromFile(const std::string& filename) {
  std::ifstream file(filename.c_str());
  if (!file.is_open()) {
    std::cerr << "Error: Cannot open file " << filename << std::endl;
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    std::istringstream iss(line);
    std::string graph_name;
    int edge_count;

    iss >> graph_name >> edge_count;

    if (graphs.has(graph_name)) {
      for (int i = 0; i < edge_count; ++i) {
        std::getline(file, line);
      }
      continue;
    }

    Graph* graph = new Graph(graph_name);

    for (int i = 0; i < edge_count; ++i) {
      if (!std::getline(file, line)) {
        break;
      }
      if (line.empty()) {
        --i;
        continue;
      }

      std::istringstream edge_iss(line);
      std::string from, to;
      unsigned int weight;

      edge_iss >> from >> to >> weight;
      graph->addEdge(from, to, weight);
    }

    graphs.add(graph_name, graph);
  }
}

void processCommand(const std::string& command_line) {
  if (command_line.empty()) {
    return;
  }

  std::istringstream iss(command_line);
  std::string command;
  iss >> command;

  std::vector<std::string> tokens;
  tokens.push_back(command);
  std::string token;
  while (iss >> token) {
    tokens.push_back(token);
  }

  if (command == "graphs" || command == "graph") {
    handleGraphs();
  } else if (command == "vertexes") {
    handleVertexes(tokens);
  } else if (command == "outbound") {
    handleOutbound(tokens);
  } else if (command == "inbound") {
    handleInbound(tokens);
  } else if (command == "bind") {
    handleBind(tokens);
  } else if (command == "cut") {
    handleCut(tokens);
  } else if (command == "create") {
    handleCreate(tokens);
  } else if (command == "merge") {
    handleMerge(tokens);
  } else if (command == "extract") {
    handleExtract(tokens);
  } else {
    printInvalid();
  }
}

void runInteractive() {
  std::string line;
  while (std::getline(std::cin, line)) {
    processCommand(line);
  }
}

void cleanup() {
  for (HashTable<std::string, Graph*,
                 std::hash<std::string>,
                 std::equal_to<std::string> >::Iterator it = graphs.begin();
       it != graphs.end(); ++it) {
    std::pair<std::string, Graph*> p = *it;
    delete p.second;
  }
}

}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
    return 1;
  }

  petrenko::loadGraphsFromFile(argv[1]);
  petrenko::runInteractive();
  petrenko::cleanup();

  return 0;
}
