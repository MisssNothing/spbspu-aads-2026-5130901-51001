#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <iostream>
#include <utility>
#include <vector>

namespace petrenko {

namespace BucketState {
  const int EMPTY = 0;
  const int OCCUPIED = 1;
  const int TOMBSTONE = 2;
}

template<class Key, class Value, class Hash, class Equal>
class HashTable {
private:
  struct Bucket {
    Key key_;
    Value value_;
    int state_;

    Bucket() : state_(BucketState::EMPTY) {}
    Bucket(const Key& k, const Value& v)
      : key_(k), value_(v), state_(BucketState::OCCUPIED) {}
  };

  std::vector<Bucket> table_;
  size_t num_elements_;
  size_t num_tombstones_;
  Hash hash_func_;
  Equal equal_func_;

  size_t probe(size_t index, size_t attempt) const {
    return (index + attempt * attempt) % table_.size();
  }

  size_t findIndex(const Key& key) const {
    if (table_.empty()) {
      return table_.size();
    }

    size_t hash = hash_func_(key) % table_.size();
    size_t attempt = 0;
    size_t first_tombstone = table_.size();

    while (attempt < table_.size()) {
      size_t idx = probe(hash, attempt);

      if (table_[idx].state_ == BucketState::EMPTY) {
        return (first_tombstone != table_.size()) ? first_tombstone : idx;
      }

      if (table_[idx].state_ == BucketState::TOMBSTONE) {
        if (first_tombstone == table_.size()) {
          first_tombstone = idx;
        }
        ++attempt;
        continue;
      }

      if (table_[idx].state_ == BucketState::OCCUPIED &&
          equal_func_(table_[idx].key_, key)) {
        return idx;
      }
      ++attempt;
    }

    return (first_tombstone != table_.size()) ? first_tombstone : table_.size();
  }

  size_t findExistingIndex(const Key& key) const {
    if (table_.empty()) {
      return table_.size();
    }

    size_t hash = hash_func_(key) % table_.size();
    size_t attempt = 0;

    while (attempt < table_.size()) {
      size_t idx = probe(hash, attempt);

      if (table_[idx].state_ == BucketState::EMPTY) {
        return table_.size();
      }

      if (table_[idx].state_ == BucketState::OCCUPIED &&
          equal_func_(table_[idx].key_, key)) {
        return idx;
      }

      ++attempt;
    }

    return table_.size();
  }

  double loadFactor() const {
    if (table_.empty()) {
      return 1.0;
    }
    return static_cast<double>(num_elements_ + num_tombstones_) / table_.size();
  }

public:
  class Iterator {
  private:
    const HashTable* table_;
    size_t index_;

  public:
    Iterator(const HashTable* table, size_t index)
      : table_(table), index_(index) {}

    Iterator& operator++() {
      do {
        ++index_;
      } while (index_ < table_->table_.size() &&
               table_->table_[index_].state_ != BucketState::OCCUPIED);
      return *this;
    }

    std::pair<Key, Value> operator*() const {
      return {table_->table_[index_].key_, table_->table_[index_].value_};
    }

    bool operator!=(const Iterator& other) const {
      return index_ != other.index_;
    }
  };

  HashTable(size_t initial_capacity = 16)
    : num_elements_(0), num_tombstones_(0) {
    if (initial_capacity < 4) {
      initial_capacity = 4;
    }
    table_.resize(initial_capacity);
  }

  void add(const Key& key, const Value& value) {
    while (num_elements_ > table_.size() / 2) {
      rehash(table_.size() * 2);
    }

    size_t idx = findIndex(key);
    if (idx == table_.size()) {
      rehash(table_.size() * 2);
      idx = findIndex(key);
      if (idx == table_.size()) {
        return;
      }
    }

    size_t existing_idx = findExistingIndex(key);
    if (existing_idx != table_.size()) {
      table_[existing_idx].value_ = value;
      return;
    }

    if (table_[idx].state_ == BucketState::TOMBSTONE) {
      num_tombstones_--;
    }

    table_[idx] = Bucket(key, value);
    num_elements_++;
  }

  bool drop(const Key& key, Value& out_value) {
    size_t idx = findExistingIndex(key);
    if (idx == table_.size()) {
      return false;
    }

    out_value = table_[idx].value_;
    table_[idx].state_ = BucketState::TOMBSTONE;
    num_elements_--;
    num_tombstones_++;
    return true;
  }

  bool has(const Key& key) const {
    size_t idx = findExistingIndex(key);
    return idx != table_.size();
  }

  bool get(const Key& key, Value& out_value) const {
    size_t idx = findExistingIndex(key);
    if (idx == table_.size()) {
      return false;
    }
    out_value = table_[idx].value_;
    return true;
  }

  void rehash(size_t slots) {
    if (slots < num_elements_ * 2) {
      slots = num_elements_ * 2;
    }
    if (slots < 4) {
      slots = 4;
    }

    std::vector<Bucket> old_table = std::move(table_);
    table_.clear();
    table_.resize(slots);

    num_elements_ = 0;
    num_tombstones_ = 0;

    for (size_t i = 0; i < old_table.size(); ++i) {
      if (old_table[i].state_ == BucketState::OCCUPIED) {
        size_t hash = hash_func_(old_table[i].key_) % table_.size();
        size_t attempt = 0;
        size_t idx;

        while (attempt < table_.size()) {
          idx = probe(hash, attempt);
          if (table_[idx].state_ == BucketState::EMPTY) {
            table_[idx] = std::move(old_table[i]);
            num_elements_++;
            break;
          }
          ++attempt;
        }
      }
    }
  }

  void cleanUp() {
    if (num_tombstones_ == 0) {
      return;
    }

    std::vector<Bucket> old_table = std::move(table_);
    table_.clear();
    table_.resize(old_table.size());
    num_elements_ = 0;
    num_tombstones_ = 0;

    for (size_t i = 0; i < old_table.size(); ++i) {
      Bucket& bucket = old_table[i];
      if (bucket.state_ == BucketState::OCCUPIED) {
        size_t idx = findIndex(bucket.key_);
        if (idx != table_.size()) {
          table_[idx] = std::move(bucket);
          num_elements_++;
        }
      }
    }
  }

  Iterator begin() const {
    for (size_t i = 0; i < table_.size(); ++i) {
      if (table_[i].state_ == BucketState::OCCUPIED) {
        return Iterator(this, i);
      }
    }
    return end();
  }

  Iterator end() const {
    return Iterator(this, table_.size());
  }

  size_t size() const {
    return num_elements_;
  }

  size_t capacity() const {
    return table_.size();
  }

  size_t tombstoneCount() const {
    return num_tombstones_;
  }
};

}

#endif
