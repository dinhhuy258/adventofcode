#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

std::unordered_map<std::string, std::string> equations;

void readInput() {
  std::ifstream inputFile("input.txt");
  std::string line;

  while (std::getline(inputFile, line)) {
    // in my test case, size(variable) = 4
    // i don't want to spend so much time to write an generic solution
    equations[line.substr(0, 4)] = line.substr(6);
  }

  inputFile.close();
}

int main() {
  // read input
  readInput();

  // part 1
  std::unordered_map<std::string, std::vector<std::string>> dependencies;
  std::unordered_map<std::string, int> degree;
  std::queue<std::string> queue;
  for (auto &[lhs, rhs] : equations) {
    // in my test case, size(variable) = 4
    // i don't want to spend so much time to write an generic solution
    if (rhs.size() == 11) {
      std::string first = rhs.substr(0, 4);
      std::string second = rhs.substr(7);

      dependencies[first].push_back(lhs);
      dependencies[second].push_back(lhs);
      degree[lhs] += 2;
    } else {
      // value
      queue.push(lhs);
    }
  }

  std::vector<std::string> topologicalOrdering;
  while (!queue.empty()) {
    auto monkey = queue.front();
    queue.pop();

    topologicalOrdering.push_back(monkey);

    for (int i = 0; i < dependencies[monkey].size(); ++i) {
      --degree[dependencies[monkey][i]];
      if (degree[dependencies[monkey][i]] == 0) {
        queue.push(dependencies[monkey][i]);
      }
    }
  }

  std::unordered_map<std::string, long long> values;
  auto calculate = [](long long val1, long long val2, char opt) {
    switch (opt) {
    case '+':
      return val1 + val2;
    case '-':
      return val1 - val2;
    case '*':
      return val1 * val2;
    case '/':
      return val1 / val2;
    }

    return 0LL;
  };
  for (auto &lhs : topologicalOrdering) {
    auto rhs = equations[lhs];
    if (rhs.size() == 11) {
      std::string firstMonkey = rhs.substr(0, 4);
      std::string secondMonkey = rhs.substr(7);

      values[lhs] =
          calculate(values[firstMonkey], values[secondMonkey], rhs[5]);
    } else {
      // value
      values[lhs] = std::stoll(rhs);
    }
  }

  std::cout << values["root"] << std::endl;

  // part 2
  std::string firstMonkey = equations["root"].substr(0, 4);
  std::string secondMonkey = equations["root"].substr(7);
  std::string invalidMonkey;

  std::string you = "humn";
  queue.push(you);
  while (!queue.empty()) {
    auto monkey = queue.front();
    queue.pop();
    values.erase(monkey);
    if (monkey != "root") {
      invalidMonkey = monkey;
    }

    for (int i = 0; i < dependencies[monkey].size(); ++i) {
      queue.push(dependencies[monkey][i]);
    }
  }

  values[invalidMonkey] =
      invalidMonkey == firstMonkey ? values[secondMonkey] : values[firstMonkey];
  queue.push(invalidMonkey);
  while (!queue.empty()) {
    auto monkey = queue.front();
    queue.pop();
    if (monkey == you) {
      break;
    }
    auto equation = equations[monkey];

    firstMonkey = equation.substr(0, 4);
    secondMonkey = equation.substr(7);
    if (values.find(firstMonkey) == values.end()) {
      queue.push(firstMonkey);
      char opt = equation[5];
      switch (opt) {
      case '+':
        opt = '-';
        break;
      case '-':
        opt = '+';
        break;
      case '*':
        opt = '/';
        break;
      case '/':
        opt = '*';
        break;
      }
      values[firstMonkey] =
          calculate(values[monkey], values[secondMonkey], opt);
    } else {
      queue.push(secondMonkey);
      switch (equation[5]) {
      case '+':
        values[secondMonkey] =
            calculate(values[monkey], values[firstMonkey], '-');
        break;
      case '-':
        values[secondMonkey] =
            calculate(values[firstMonkey], values[monkey], '-');
        break;
      case '*':
        values[secondMonkey] =
            calculate(values[monkey], values[firstMonkey], '/');
        break;
      case '/':
        values[secondMonkey] =
            calculate(values[firstMonkey], values[monkey], '/');
        break;
      }
    }
  }

  std::cout << values[you] << std::endl;
}
