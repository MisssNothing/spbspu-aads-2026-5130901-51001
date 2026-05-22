#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <iostream>
#include <string>
#include <functional>

template<class Key, class Value>
class BSTIterator;

template<class Key, class Value>
class BSTConstIterator;

template<class Key, class Value>
struct BSTNode {
  Key key;
  Value value;
  BSTNode<Key, Value>* left;
  BSTNode<Key, Value>* right;
  BSTNode<Key, Value>* parent;

  BSTNode(const Key& k, const Value& v)
    : key(k), value(v), left(nullptr), right(nullptr), parent(nullptr) {}
};

template<class Key, class Value>
class BSTIterator {
private:
  BSTNode<Key, Value>* node;

public:
  BSTIterator();
  explicit BSTIterator(BSTNode<Key, Value>* n);

  std::pair<const Key&, Value&> operator*() const;
  std::pair<const Key&, Value&> operator*();

  BSTIterator& operator++();
  BSTIterator operator++(int);

  bool operator==(const BSTIterator& other) const;
  bool operator!=(const BSTIterator& other) const;

  BSTNode<Key, Value>* getNode() const;
};

template<class Key, class Value>
class BSTConstIterator {
private:
  const BSTNode<Key, Value>* node;

public:
  BSTConstIterator();
  explicit BSTConstIterator(const BSTNode<Key, Value>* n);
  explicit BSTConstIterator(const BSTIterator<Key, Value>& it);

  std::pair<const Key&, const Value&> operator*() const;

  BSTConstIterator& operator++();
  BSTConstIterator operator++(int);

  bool operator==(const BSTConstIterator& other) const;
  bool operator!=(const BSTConstIterator& other) const;

  const BSTNode<Key, Value>* getNode() const;
};

template<class Key, class Value, class Compare = std::less<Key>>
class BSTree {
private:
  BSTNode<Key, Value>* root;
  Compare comp;
  size_t treeSize;

  BSTNode<Key, Value>* findNode(const Key& k) const;
  BSTNode<Key, Value>* getMinimum(BSTNode<Key, Value>* node) const;
  BSTNode<Key, Value>* getMaximum(BSTNode<Key, Value>* node) const;
  size_t computeHeight(BSTNode<Key, Value>* node) const;
  void deleteSubtree(BSTNode<Key, Value>* node);
  BSTNode<Key, Value>* copySubtree(BSTNode<Key, Value>* node, BSTNode<Key, Value>* parent);

public:
  BSTree();
  BSTree(const BSTree& other);
  BSTree& operator=(const BSTree& other);
  ~BSTree();

  void push(Key k, Value v);
  Value get(Key k);
  Value drop(Key k);

  using iterator = BSTIterator<Key, Value>;
  using const_iterator = BSTConstIterator<Key, Value>;

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

  iterator find(const Key& k);
  const_iterator find(const Key& k) const;

  const_iterator rotateLeft(const_iterator it);
  const_iterator rotateRight(const_iterator it);
  const_iterator rotateLargeLeft(const_iterator it);
  const_iterator rotateLargeRight(const_iterator it);

  size_t height(const_iterator it);
  size_t height();

  size_t size() const;
  bool empty() const;
  void clear();
};

#endif
