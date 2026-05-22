#ifndef BSTREE_CPP
#define BSTREE_CPP

#include "bstree.hpp"

namespace petrenko {

template<class Key, class Value>
BSTIterator<Key, Value>::BSTIterator()
  : node_(nullptr)
{
}

template<class Key, class Value>
BSTIterator<Key, Value>::BSTIterator(BSTNode<Key, Value>* node)
  : node_(node)
{
}

template<class Key, class Value>
std::pair<const Key&, Value&> BSTIterator<Key, Value>::operator*() const {
  return std::pair<const Key&, Value&>(node_->key_, node_->value_);
}

template<class Key, class Value>
BSTIterator<Key, Value>& BSTIterator<Key, Value>::operator++() {
  if (node_->right_) {
    node_ = node_->right_;
    while (node_->left_) {
      node_ = node_->left_;
    }
  } else {
    BSTNode<Key, Value>* parent = node_->parent_;
    while (parent != parent->parent_ && node_ == parent->right_) {
      node_ = parent;
      parent = parent->parent_;
    }
    node_ = parent;
  }
  return *this;
}

template<class Key, class Value>
BSTIterator<Key, Value> BSTIterator<Key, Value>::operator++(int) {
  BSTIterator tmp = *this;
  ++(*this);
  return tmp;
}

template<class Key, class Value>
bool BSTIterator<Key, Value>::operator==(const BSTIterator& other) const {
  return node_ == other.node_;
}

template<class Key, class Value>
bool BSTIterator<Key, Value>::operator!=(const BSTIterator& other) const {
  return node_ != other.node_;
}

template<class Key, class Value>
BSTNode<Key, Value>* BSTIterator<Key, Value>::getNode() const {
  return node_;
}

template<class Key, class Value>
BSTConstIterator<Key, Value>::BSTConstIterator()
  : node_(nullptr)
{
}

template<class Key, class Value>
BSTConstIterator<Key, Value>::BSTConstIterator(const BSTNode<Key, Value>* node)
  : node_(node)
{
}

template<class Key, class Value>
BSTConstIterator<Key, Value>::BSTConstIterator(const BSTIterator<Key, Value>& it)
  : node_(it.getNode())
{
}

template<class Key, class Value>
std::pair<const Key&, const Value&> BSTConstIterator<Key, Value>::operator*() const {
  return std::pair<const Key&, const Value&>(node_->key_, node_->value_);
}

template<class Key, class Value>
BSTConstIterator<Key, Value>& BSTConstIterator<Key, Value>::operator++() {
  if (node_->right_) {
    node_ = node_->right_;
    while (node_->left_) {
      node_ = node_->left_;
    }
  } else {
    const BSTNode<Key, Value>* parent = node_->parent_;
    while (parent != parent->parent_ && node_ == parent->right_) {
      node_ = parent;
      parent = parent->parent_;
    }
    node_ = parent;
  }
  return *this;
}

template<class Key, class Value>
BSTConstIterator<Key, Value> BSTConstIterator<Key, Value>::operator++(int) {
  BSTConstIterator tmp = *this;
  ++(*this);
  return tmp;
}

template<class Key, class Value>
bool BSTConstIterator<Key, Value>::operator==(const BSTConstIterator& other) const {
  return node_ == other.node_;
}

template<class Key, class Value>
bool BSTConstIterator<Key, Value>::operator!=(const BSTConstIterator& other) const {
  return node_ != other.node_;
}

template<class Key, class Value>
const BSTNode<Key, Value>* BSTConstIterator<Key, Value>::getNode() const {
  return node_;
}

template<class Key, class Value, class Compare>
BSTree<Key, Value, Compare>::BSTree()
  : size_(0)
{
  fake_root_ = new BSTNode<Key, Value>(Key(), Value());
  fake_root_->left_ = nullptr;
  fake_root_->right_ = nullptr;
  fake_root_->parent_ = fake_root_;
}

template<class Key, class Value, class Compare>
BSTree<Key, Value, Compare>::BSTree(const BSTree& other)
  : size_(0)
{
  fake_root_ = new BSTNode<Key, Value>(Key(), Value());
  fake_root_->left_ = nullptr;
  fake_root_->right_ = nullptr;
  fake_root_->parent_ = fake_root_;
  if (other.fake_root_->left_) {
    fake_root_->left_ = copySubtree(other.fake_root_->left_, fake_root_);
  }
  size_ = other.size_;
}

template<class Key, class Value, class Compare>
BSTree<Key, Value, Compare>& BSTree<Key, Value, Compare>::operator=(const BSTree& other) {
  if (this != &other) {
    clear();
    if (other.fake_root_->left_) {
      fake_root_->left_ = copySubtree(other.fake_root_->left_, fake_root_);
    }
    size_ = other.size_;
  }
  return *this;
}

template<class Key, class Value, class Compare>
BSTree<Key, Value, Compare>::~BSTree() {
  clear();
  delete fake_root_;
}

template<class Key, class Value, class Compare>
BSTNode<Key, Value>* BSTree<Key, Value, Compare>::copySubtree(
  BSTNode<Key, Value>* node,
  BSTNode<Key, Value>* parent)
{
  if (!node) {
    return nullptr;
  }
  BSTNode<Key, Value>* new_node = new BSTNode<Key, Value>(node->key_, node->value_);
  new_node->parent_ = parent;
  new_node->left_ = copySubtree(node->left_, new_node);
  new_node->right_ = copySubtree(node->right_, new_node);
  return new_node;
}

template<class Key, class Value, class Compare>
void BSTree<Key, Value, Compare>::deleteSubtree(BSTNode<Key, Value>* node) {
  if (!node) {
    return;
  }
  deleteSubtree(node->left_);
  deleteSubtree(node->right_);
  delete node;
}

template<class Key, class Value, class Compare>
BSTNode<Key, Value>* BSTree<Key, Value, Compare>::findNode(const Key& k) const {
  BSTNode<Key, Value>* current = fake_root_->left_;
  while (current) {
    if (comp_(k, current->key_)) {
      current = current->left_;
    } else if (comp_(current->key_, k)) {
      current = current->right_;
    } else {
      return current;
    }
  }
  return nullptr;
}

template<class Key, class Value, class Compare>
BSTNode<Key, Value>* BSTree<Key, Value, Compare>::getMinimum(BSTNode<Key, Value>* node) const {
  if (!node) {
    return nullptr;
  }
  while (node->left_) {
    node = node->left_;
  }
  return node;
}

template<class Key, class Value, class Compare>
BSTNode<Key, Value>* BSTree<Key, Value, Compare>::getMaximum(BSTNode<Key, Value>* node) const {
  if (!node) {
    return nullptr;
  }
  while (node->right_) {
    node = node->right_;
  }
  return node;
}

template<class Key, class Value, class Compare>
size_t BSTree<Key, Value, Compare>::computeHeight(BSTNode<Key, Value>* node) const {
  if (!node) {
    return 0;
  }
  size_t left_height = computeHeight(node->left_);
  size_t right_height = computeHeight(node->right_);
  return 1 + (left_height > right_height ? left_height : right_height);
}

template<class Key, class Value, class Compare>
void BSTree<Key, Value, Compare>::push(const Key& k, const Value& v) {
  if (!fake_root_->left_) {
    fake_root_->left_ = new BSTNode<Key, Value>(k, v);
    fake_root_->left_->parent_ = fake_root_;
    ++size_;
    return;
  }
  BSTNode<Key, Value>* current = fake_root_->left_;
  BSTNode<Key, Value>* parent = nullptr;
  while (current) {
    parent = current;
    if (comp_(k, current->key_)) {
      current = current->left_;
    } else if (comp_(current->key_, k)) {
      current = current->right_;
    } else {
      current->value_ = v;
      return;
    }
  }
  BSTNode<Key, Value>* new_node = new BSTNode<Key, Value>(k, v);
  new_node->parent_ = parent;
  if (comp_(k, parent->key_)) {
    parent->left_ = new_node;
  } else {
    parent->right_ = new_node;
  }
  ++size_;
}

template<class Key, class Value, class Compare>
Value BSTree<Key, Value, Compare>::get(const Key& k) {
  BSTNode<Key, Value>* node = findNode(k);
  if (!node) {
    throw "Key not found";
  }
  return node->value_;
}

template<class Key, class Value, class Compare>
Value BSTree<Key, Value, Compare>::drop(const Key& k) {
  BSTNode<Key, Value>* node = findNode(k);
  if (!node) {
    throw "Key not found";
  }
  Value result = node->value_;
  BSTNode<Key, Value>* parent = node->parent_;
  bool is_left_child = (node == parent->left_);
  if (!node->left_ && !node->right_) {
    if (is_left_child) {
      parent->left_ = nullptr;
    } else {
      parent->right_ = nullptr;
    }
    delete node;
  } else if (!node->left_) {
    if (is_left_child) {
      parent->left_ = node->right_;
    } else {
      parent->right_ = node->right_;
    }
    node->right_->parent_ = parent;
    delete node;
  } else if (!node->right_) {
    if (is_left_child) {
      parent->left_ = node->left_;
    } else {
      parent->right_ = node->left_;
    }
    node->left_->parent_ = parent;
    delete node;
  } else {
    BSTNode<Key, Value>* successor = getMinimum(node->right_);
    node->key_ = successor->key_;
    node->value_ = successor->value_;
    BSTNode<Key, Value>* succ_parent = successor->parent_;
    bool is_succ_left = (successor == succ_parent->left_);
    if (is_succ_left) {
      succ_parent->left_ = successor->right_;
    } else {
      succ_parent->right_ = successor->right_;
    }
    if (successor->right_) {
      successor->right_->parent_ = succ_parent;
    }
    delete successor;
  }
  --size_;
  return result;
}

template<class Key, class Value, class Compare>
typename BSTree<Key, Value, Compare>::iterator BSTree<Key, Value, Compare>::begin() {
  return iterator(getMinimum(fake_root_->left_));
}

template<class Key, class Value, class Compare>
typename BSTree<Key, Value, Compare>::const_iterator BSTree<Key, Value, Compare>::begin() const {
  return const_iterator(getMinimum(fake_root_->left_));
}

template<class Key, class Value, class Compare>
typename BSTree<Key, Value, Compare>::iterator BSTree<Key, Value, Compare>::end() {
  return iterator(nullptr);
}

template<class Key, class Value, class Compare>
typename BSTree<Key, Value, Compare>::const_iterator BSTree<Key, Value, Compare>::end() const {
  return const_iterator(nullptr);
}

template<class Key, class Value, class Compare>
typename BSTree<Key, Value, Compare>::iterator BSTree<Key, Value, Compare>::find(const Key& k) {
  return iterator(findNode(k));
}

template<class Key, class Value, class Compare>
typename BSTree<Key, Value, Compare>::const_iterator BSTree<Key, Value, Compare>::find(const Key& k) const {
  return const_iterator(findNode(k));
}

template<class Key, class Value, class Compare>
typename BSTree<Key, Value, Compare>::const_iterator BSTree<Key, Value, Compare>::rotateLeft(const_iterator it) {
  BSTNode<Key, Value>* node = const_cast<BSTNode<Key, Value>*>(it.getNode());
  if (!node || !node->right_) {
    return it;
  }
  BSTNode<Key, Value>* parent = node->parent_;
  BSTNode<Key, Value>* right_child = node->right_;
  node->right_ = right_child->left_;
  if (right_child->left_) {
    right_child->left_->parent_ = node;
  }
  right_child->left_ = node;
  right_child->parent_ = parent;
  node->parent_ = right_child;
  if (node == parent->left_) {
    parent->left_ = right_child;
  } else {
    parent->right_ = right_child;
  }
  return const_iterator(right_child);
}

template<class Key, class Value, class Compare>
typename BSTree<Key, Value, Compare>::const_iterator BSTree<Key, Value, Compare>::rotateRight(const_iterator it) {
  BSTNode<Key, Value>* node = const_cast<BSTNode<Key, Value>*>(it.getNode());
  if (!node || !node->left_) {
    return it;
  }
  BSTNode<Key, Value>* parent = node->parent_;
  BSTNode<Key, Value>* left_child = node->left_;
  node->left_ = left_child->right_;
  if (left_child->right_) {
    left_child->right_->parent_ = node;
  }
  left_child->right_ = node;
  left_child->parent_ = parent;
  node->parent_ = left_child;
  if (node == parent->left_) {
    parent->left_ = left_child;
  } else {
    parent->right_ = left_child;
  }
  return const_iterator(left_child);
}

template<class Key, class Value, class Compare>
typename BSTree<Key, Value, Compare>::const_iterator BSTree<Key, Value, Compare>::rotateLargeLeft(const_iterator it) {
  BSTNode<Key, Value>* node = const_cast<BSTNode<Key, Value>*>(it.getNode());
  if (!node || !node->right_ || !node->right_->left_) {
    return it;
  }
  rotateRight(const_iterator(node->right_));
  return rotateLeft(it);
}

template<class Key, class Value, class Compare>
typename BSTree<Key, Value, Compare>::const_iterator BSTree<Key, Value, Compare>::rotateLargeRight(const_iterator it) {
  BSTNode<Key, Value>* node = const_cast<BSTNode<Key, Value>*>(it.getNode());
  if (!node || !node->left_ || !node->left_->right_) {
    return it;
  }
  rotateLeft(const_iterator(node->left_));
  return rotateRight(it);
}

template<class Key, class Value, class Compare>
size_t BSTree<Key, Value, Compare>::height(const_iterator it) const {
  const BSTNode<Key, Value>* node = it.getNode();
  if (!node) {
    return 0;
  }
  return computeHeight(const_cast<BSTNode<Key, Value>*>(node));
}

template<class Key, class Value, class Compare>
size_t BSTree<Key, Value, Compare>::height() const {
  return computeHeight(fake_root_->left_);
}

template<class Key, class Value, class Compare>
size_t BSTree<Key, Value, Compare>::size() const {
  return size_;
}

template<class Key, class Value, class Compare>
bool BSTree<Key, Value, Compare>::empty() const {
  return size_ == 0;
}

template<class Key, class Value, class Compare>
void BSTree<Key, Value, Compare>::clear() {
  deleteSubtree(fake_root_->left_);
  fake_root_->left_ = nullptr;
  size_ = 0;
}

}

#endif
