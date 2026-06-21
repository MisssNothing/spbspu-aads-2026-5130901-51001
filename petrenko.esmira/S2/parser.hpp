#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include "stack.hpp"
#include "queue.hpp"

namespace petrenko {
  int getPrecedence(const std::string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/" || op == "%") return 2;
    if (op == "gcd") return 3;
    return 0;
  }

  bool isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" ||
         token == "/" || token == "%" || token == "gcd";
  }

  bool isDigit(char c) {
    return c >= '0' && c <= '9';
  }

  bool isNumber(const std::string& token) {
    if (token.empty()) return false;
    size_t start = (token[0] == '-') ? 1 : 0;
    if (start >= token.size()) return false;
    for (size_t i = start; i < token.size(); ++i) {
      if (!isDigit(token[i])) return false;
    }
    return true;
  }

  long long applyOperator(const std::string& op, long long a, long long b) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") {
      if (b == 0) throw std::runtime_error("Zero");
      return a / b;
    }

    if (op == "%") {
      if (b == 0) throw std::runtime_error("Zero");
      return a % b;
    }

    if (op == "gcd") {
      a = (a < 0) ? -1 * a : a;
      b = (b < 0) ? -1 * b : b;
      long long temp;
      while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
      }
      return a;
    }
    throw std::runtime_error("Unknown operator");
  }

  Queue<std::string> tokenizeLine(const std::string& line) {
    Queue<std::string> tokens;
    std::string token;

    for (size_t i = 0; i < line.size(); ++i) {
      if (line[i] == ' ') {
        if (!token.empty()) {
          tokens.push(token);
          token.clear();
        }
      } else {
        token += line[i];
      }
    }

    if (!token.empty()) {
      tokens.push(token);
    }

    return tokens;
  }

  Queue<std::string> infixToPostfix(Queue<std::string> infix) {
    Stack<std::string> operators;
    Queue<std::string> postfix;

    while (!infix.empty()) {
      std::string token = infix.pop();

      if (isNumber(token)) {
        postfix.push(token);
      } else if (token == "(") {
        operators.push(token);
      } else if (token == ")") {
        while (!operators.empty() && operators.top() != "(") {
          postfix.push(operators.pop());
        }
        if (operators.empty()) {
          throw std::runtime_error("Mismatched parentheses");
        }
        operators.pop();
      } else if (isOperator(token)) {
        while (!operators.empty() && operators.top() != "(" &&
               getPrecedence(operators.top()) >= getPrecedence(token)) {
          postfix.push(operators.pop());
        }
        operators.push(token);
      } else {
        throw std::runtime_error("Invalid token: " + token);
      }
    }

    while (!operators.empty()) {
      if (operators.top() == "(" || operators.top() == ")") {
        throw std::runtime_error("Mismatched parentheses");
      }
      postfix.push(operators.pop());
    }

    return postfix;
  }

  long long evaluatePostfix(Queue<std::string> postfix) {
    Stack<long long> values;

    while (!postfix.empty()) {
      std::string token = postfix.pop();

      if (isNumber(token)) {
        values.push(std::stoll(token));
      } else if (isOperator(token)) {
        if (values.size() < 2) {
          throw std::runtime_error("Not enough operands");
        }
        long long b = values.pop();
        long long a = values.pop();
        values.push(applyOperator(token, a, b));
      } else {
        throw std::runtime_error("Invalid token in postfix");
      }
    }

    if (values.size() != 1) {
      throw std::runtime_error("Invalid expression: wrong number of operands");
    }

    return values.pop();
  }
}
#endif
