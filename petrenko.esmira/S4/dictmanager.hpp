#ifndef DICTIONARY_MANAGER_HPP
#define DICTIONARY_MANAGER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "bstree.hpp"

namespace petrenko {

struct DictionaryEntry {
  std::string name_;
  BSTree<int, std::string> tree_;
};

class DictionaryManager {
public:
  void loadFromFile(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
      throw "Cannot open file: " + filename;
    }
    std::string line;
    while (std::getline(file, line)) {
      if (line.empty()) {
        continue;
      }
      std::stringstream ss(line);
      std::string dict_name;
      ss >> dict_name;
      BSTree<int, std::string> tree;
      int key;
      std::string value;
      while (ss >> key >> value) {
        tree.push(key, value);
      }
      setDictionary(dict_name, tree);
    }
  }

  void processCommand(const std::string& command) {
    std::stringstream ss(command);
    std::string cmd;
    ss >> cmd;
    if (cmd == "print") {
      std::string dataset;
      ss >> dataset;
      int idx = findDictionary(dataset);
      if (idx == -1) {
        std::cout << "<INVALID COMMAND>" << '\n';
        return;
      }
      const BSTree<int, std::string>& dict = dictionaries_[idx].tree_;
      if (dict.empty()) {
        std::cout << "<EMPTY>" << '\n';
      } else {
        std::cout << dataset;
        for (BSTree<int, std::string>::const_iterator it = dict.begin(); it != dict.end(); ++it) {
          std::pair<const int&, const std::string&> pair = *it;
          std::cout << " " << pair.first << " " << pair.second;
        }
        std::cout << '\n';
      }
    } else if (cmd == "complement") {
      std::string new_dataset, dataset1, dataset2;
      ss >> new_dataset >> dataset1 >> dataset2;
      if (findDictionary(dataset1) == -1 || findDictionary(dataset2) == -1) {
        std::cout << "<INVALID COMMAND>" << '\n';
        return;
      }
      BSTree<int, std::string> dict1 = getDictionary(dataset1);
      BSTree<int, std::string> dict2 = getDictionary(dataset2);
      BSTree<int, std::string> result = complement(dict1, dict2);
      setDictionary(new_dataset, result);
    } else if (cmd == "intersect") {
      std::string new_dataset, dataset1, dataset2;
      ss >> new_dataset >> dataset1 >> dataset2;
      if (findDictionary(dataset1) == -1 || findDictionary(dataset2) == -1) {
        std::cout << "<INVALID COMMAND>" << '\n';
        return;
      }
      BSTree<int, std::string> dict1 = getDictionary(dataset1);
      BSTree<int, std::string> dict2 = getDictionary(dataset2);
      BSTree<int, std::string> result = intersect(dict1, dict2);
      setDictionary(new_dataset, result);
    } else if (cmd == "union") {
      std::string new_dataset, dataset1, dataset2;
      ss >> new_dataset >> dataset1 >> dataset2;
      if (findDictionary(dataset1) == -1 || findDictionary(dataset2) == -1) {
        std::cout << "<INVALID COMMAND>" << '\n';
        return;
      }
      BSTree<int, std::string> dict1 = getDictionary(dataset1);
      BSTree<int, std::string> dict2 = getDictionary(dataset2);
      BSTree<int, std::string> result = setUnion(dict1, dict2);
      setDictionary(new_dataset, result);
    } else {
      std::cout << "<INVALID COMMAND>" << '\n';
    }
  }

  BSTree<int, std::string> complement(
    const BSTree<int, std::string>& a,
    const BSTree<int, std::string>& b)
  {
    BSTree<int, std::string> result;
    for (BSTree<int, std::string>::const_iterator it = a.begin(); it != a.end(); ++it) {
      std::pair<const int&, const std::string&> pair = *it;
      if (b.find(pair.first) == b.end()) {
        result.push(pair.first, pair.second);
      }
    }
    return result;
  }

  BSTree<int, std::string> intersect(
    const BSTree<int, std::string>& a,
    const BSTree<int, std::string>& b)
  {
    BSTree<int, std::string> result;
    for (BSTree<int, std::string>::const_iterator it = a.begin(); it != a.end(); ++it) {
      std::pair<const int&, const std::string&> pair = *it;
      if (b.find(pair.first) != b.end()) {
        result.push(pair.first, pair.second);
      }
    }
    return result;
  }

  BSTree<int, std::string> setUnion(
    const BSTree<int, std::string>& a,
    const BSTree<int, std::string>& b)
  {
    BSTree<int, std::string> result;
    for (BSTree<int, std::string>::const_iterator it = a.begin(); it != a.end(); ++it) {
      std::pair<const int&, const std::string&> pair = *it;
      result.push(pair.first, pair.second);
    }
    for (BSTree<int, std::string>::const_iterator it = b.begin(); it != b.end(); ++it) {
      std::pair<const int&, const std::string&> pair = *it;
      if (result.find(pair.first) == result.end()) {
        result.push(pair.first, pair.second);
      }
    }
    return result;
  }

private:
  std::vector<DictionaryEntry> dictionaries_;

  int findDictionary(const std::string& name) const {
    for (size_t i = 0; i < dictionaries_.size(); ++i) {
      if (dictionaries_[i].name_ == name) {
        return static_cast<int>(i);
      }
    }
    return -1;
  }

  BSTree<int, std::string> getDictionary(const std::string& name) const {
    int idx = findDictionary(name);
    if (idx == -1) {
      return BSTree<int, std::string>();
    }
    return dictionaries_[idx].tree_;
  }

  void setDictionary(const std::string& name, const BSTree<int, std::string>& tree) {
    int idx = findDictionary(name);
    if (idx == -1) {
      DictionaryEntry entry;
      entry.name_ = name;
      entry.tree_ = tree;
      dictionaries_.push_back(entry);
    } else {
      dictionaries_[idx].tree_ = tree;
    }
  }
};

}

#endif
