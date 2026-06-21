#ifndef STACK_HPP
#define STACK_HPP

#include "../S1/list.hpp"

namespace petrenko {
  template<class T>
  struct Stack {
  private:
    List<T> data;
  public:
    Stack() = default;
    Stack(const Stack& other) = default;
    Stack(Stack&& other) noexcept = default;

    Stack& operator=(const Stack& other) = default;
    Stack& operator=(Stack&& other) noexcept = default;

    void push(const T& rhs) {
      data.insert(rhs, data.getSize());
    }

    T pop() {
      if (empty()) {
        throw std::runtime_error("Stack is empty");
      }
      T value = data[data.getSize() - 1];
      data.removeAt(data.getSize() - 1);
      return value;
    }

    const T& top() const {
      if (empty()) {
        throw std::runtime_error("Stack is empty");
      }
      return data[data.getSize() - 1];
    }

    bool empty() const {
      return data.getSize() == 0;
    }

    size_t size() const {
      return data.getSize();
    }

    void clear() {
      data.clear();
    }

    bool operator==(const Stack<T>& other) const {
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

    bool operator!=(const Stack<T>& other) const {
      return !(*this == other);
    }
  };
}

#endif
