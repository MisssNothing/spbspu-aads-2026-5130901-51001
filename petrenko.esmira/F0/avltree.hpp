#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <algorithm>
#include <utility>
#include <vector>

namespace petrenko {

template<typename Key, typename Value>
class AVLTree
{
private:
  struct Node
  {
    Key key_;
    Value value_;
    Node* left_;
    Node* right_;
    int height_;

    Node(const Key& key, const Value& value)
      : key_(key)
      , value_(value)
      , left_(nullptr)
      , right_(nullptr)
      , height_(1)
    {
    }
  };

  Node* root_;
  size_t size_;

  int getHeight(Node* node) const
  {
    if (node)
    {
      return node->height_;
    }
    return 0;
  }

  int getBalanceFactor(Node* node) const
  {
    if (node)
    {
      return getHeight(node->left_) - getHeight(node->right_);
    }
    return 0;
  }

  void updateHeight(Node* node)
  {
    if (node)
    {
      int leftHeight = getHeight(node->left_);
      int rightHeight = getHeight(node->right_);
      node->height_ = 1 + std::max(leftHeight, rightHeight);
    }
  }

  Node* rotateRight(Node* y)
  {
    Node* x = y->left_;
    Node* t2 = x->right_;

    x->right_ = y;
    y->left_ = t2;

    updateHeight(y);
    updateHeight(x);

    return x;
  }

  Node* rotateLeft(Node* x)
  {
    Node* y = x->right_;
    Node* t2 = y->left_;

    y->left_ = x;
    x->right_ = t2;

    updateHeight(x);
    updateHeight(y);

    return y;
  }

  Node* balance(Node* node)
  {
    if (!node)
    {
      return node;
    }

    updateHeight(node);
    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1)
    {
      if (getBalanceFactor(node->left_) < 0)
      {
        node->left_ = rotateLeft(node->left_);
      }
      return rotateRight(node);
    }

    if (balanceFactor < -1)
    {
      if (getBalanceFactor(node->right_) > 0)
      {
        node->right_ = rotateRight(node->right_);
      }
      return rotateLeft(node);
    }

    return node;
  }

  Node* insertNode(Node* node, const Key& key, const Value& value)
  {
    if (!node)
    {
      ++size_;
      return new Node(key, value);
    }

    if (key < node->key_)
    {
      node->left_ = insertNode(node->left_, key, value);
    }
    else if (key > node->key_)
    {
      node->right_ = insertNode(node->right_, key, value);
    }
    else
    {
      node->value_ = value;
      return node;
    }

    return balance(node);
  }

  Node* findMin(Node* node) const
  {
    while (node && node->left_)
    {
      node = node->left_;
    }
    return node;
  }

  Node* removeNode(Node* node, const Key& key)
  {
    if (!node)
    {
      return node;
    }

    if (key < node->key_)
    {
      node->left_ = removeNode(node->left_, key);
    }
    else if (key > node->key_)
    {
      node->right_ = removeNode(node->right_, key);
    }
    else
    {
      if (!node->left_ || !node->right_)
      {
        Node* temp = node->left_ ? node->left_ : node->right_;
        delete node;
        --size_;
        return temp;
      }
      else
      {
        Node* temp = findMin(node->right_);
        node->key_ = temp->key_;
        node->value_ = temp->value_;
        node->right_ = removeNode(node->right_, temp->key_);
      }
    }

    return balance(node);
  }

  Node* findNode(Node* node, const Key& key) const
  {
    if (!node)
    {
      return nullptr;
    }

    if (key < node->key_)
    {
      return findNode(node->left_, key);
    }

    if (key > node->key_)
    {
      return findNode(node->right_, key);
    }

    return node;
  }

  void inorderCollect(Node* node, std::vector<std::pair<Key, Value>>& items) const
  {
    if (!node)
    {
      return;
    }

    inorderCollect(node->left_, items);
    items.push_back(std::make_pair(node->key_, node->value_));
    inorderCollect(node->right_, items);
  }

  void clearNode(Node* node)
  {
    if (!node)
    {
      return;
    }

    clearNode(node->left_);
    clearNode(node->right_);
    delete node;
  }

  Node* copyNode(Node* node)
  {
    if (!node)
    {
      return nullptr;
    }

    Node* newNode = new Node(node->key_, node->value_);
    newNode->left_ = copyNode(node->left_);
    newNode->right_ = copyNode(node->right_);
    newNode->height_ = node->height_;

    return newNode;
  }

public:
  AVLTree()
    : root_(nullptr)
    , size_(0)
  {
  }

  AVLTree(const AVLTree& other)
    : root_(nullptr)
    , size_(other.size_)
  {
    root_ = copyNode(other.root_);
  }

  AVLTree& operator=(const AVLTree& other)
  {
    if (this != &other)
    {
      clearNode(root_);
      size_ = other.size_;
      root_ = copyNode(other.root_);
    }
    return *this;
  }

  ~AVLTree()
  {
    clearNode(root_);
  }

  void insert(const Key& key, const Value& value)
  {
    root_ = insertNode(root_, key, value);
  }

  void remove(const Key& key)
  {
    root_ = removeNode(root_, key);
  }

  bool find(const Key& key, Value& value) const
  {
    Node* node = findNode(root_, key);
    if (node)
    {
      value = node->value_;
      return true;
    }
    return false;
  }

  bool contains(const Key& key) const
  {
    return findNode(root_, key) != nullptr;
  }

  bool empty() const
  {
    return size_ == 0;
  }

  size_t size() const
  {
    return size_;
  }

  std::vector<std::pair<Key, Value>> toVector() const
  {
    std::vector<std::pair<Key, Value>> items;
    inorderCollect(root_, items);
    return items;
  }
};

}

#endif
