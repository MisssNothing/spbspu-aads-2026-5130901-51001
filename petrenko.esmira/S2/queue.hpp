#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "../S1/list.hpp"

namespace petrenko {
  template< class T >
  struct Queue {
  private:
    List< T > data;
  public:
    Queue() = default;
    Queue(const Queue& other) = default;
    Queue(Queue&& other) noexcept = default;

    Queue& operator=(const Queue& other) = default;
    Queue& operator=(Queue&& other) noexcept = default;

    void push(const T& rhs) {
      data.insert(rhs, data.getSize());
    }

    bool empty() const {
      return data.getSize() == 0;
    }

    T pop() {
      if (empty()) {
        throw std::runtime_error("Queue is empty");
      }
      T value = data[0];
      data.removeAt(0);
      return value;
    }

    const T& front() const {
      if (empty()) {
        throw std::runtime_error("Queue is empty");
      }
      return data[0];
    }

    const T& back() const {
      if (empty()) {
        throw std::runtime_error("Queue is empty");
      }
      return data[data.getSize() - 1];
    }

    size_t size() const {
      return data.getSize();
    }

    void clear() {
      data.clear();
    }

    bool operator==(const Queue<T>& other) const {
      if (size() != other.size()) {
        return false;
      }
      for (size_t i = 0; i < size(); ++i) {
        if (data[i] != other.data[i]) {
          return false;
        }
      }
      return true;
    }

    bool operator!=(const Queue<T>& other) const {
      return !(*this == other);
    }
  };
}
#endif
