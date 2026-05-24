#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <functional>
#include <string>
#include <utility>

namespace petrenko {

template<class Key, class Value>
class BSTIterator;

template<class Key, class Value>
class BSTConstIterator;

template<class Key, class Value>
struct BSTNode {
  Key key_;
  Value value_;
  BSTNode<Key, Value>* left_;
  BSTNode<Key, Value>* right_;
  BSTNode<Key, Value>* parent_;

  BSTNode(const Key& k, const Value& v)
    : key_(k), value_(v), left_(nullptr), right_(nullptr), parent_(nullptr)
  {
  }
};

template<class Key, class Value>
class BSTIterator {
public:
  BSTIterator();
  explicit BSTIterator(BSTNode<Key, Value>* node);

  std::pair<const Key&, Value&> operator*() const;
  BSTIterator& operator++();
  BSTIterator operator++(int);

  bool operator==(const BSTIterator& other) const;
  bool operator!=(const BSTIterator& other) const;

  BSTNode<Key, Value>* getNode() const;

private:
  BSTNode<Key, Value>* node_;
};

template<class Key, class Value>
class BSTConstIterator {
public:
  BSTConstIterator();
  explicit BSTConstIterator(const BSTNode<Key, Value>* node);
  BSTConstIterator(const BSTIterator<Key, Value>& it);

  std::pair<const Key&, const Value&> operator*() const;
  BSTConstIterator& operator++();
  BSTConstIterator operator++(int);

  bool operator==(const BSTConstIterator& other) const;
  bool operator!=(const BSTConstIterator& other) const;

  const BSTNode<Key, Value>* getNode() const;

private:
  const BSTNode<Key, Value>* node_;
};

template<class Key, class Value, class Compare = std::less<Key> >
class BSTree {
public:
  using iterator = BSTIterator<Key, Value>;
  using const_iterator = BSTConstIterator<Key, Value>;

  BSTree();
  BSTree(const BSTree& other);
  BSTree& operator=(const BSTree& other);
  ~BSTree();

  void push(const Key& k, const Value& v);
  Value get(const Key& k);
  Value drop(const Key& k);

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

  size_t height(const_iterator it) const;
  size_t height() const;

  size_t size() const;
  bool empty() const;
  void clear();

private:
  BSTNode<Key, Value>* fake_root_;
  Compare comp_;
  size_t size_;

  BSTNode<Key, Value>* findNode(const Key& k) const;
  BSTNode<Key, Value>* getMinimum(BSTNode<Key, Value>* node) const;
  BSTNode<Key, Value>* getMaximum(BSTNode<Key, Value>* node) const;
  size_t computeHeight(BSTNode<Key, Value>* node) const;
  void deleteSubtree(BSTNode<Key, Value>* node);
  BSTNode<Key, Value>* copySubtree(BSTNode<Key, Value>* node, BSTNode<Key, Value>* parent);
};

}

#include "bstree.cpp"

#endif
