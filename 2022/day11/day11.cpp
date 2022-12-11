#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <vector>

class Monkey {
public:
  int id;
  std::string operation;
  std::vector<long long> items;
  int test;
  int ifTrue;
  int ifFalse;
  int inspectionCount;

public:
  Monkey(int id, std::string operation, std::vector<long long> currentItems,
         int test, int ifTrue, int ifFalse) {
    this->id = id;
    this->operation = std::move(operation);
    this->items = std::move(currentItems);
    this->test = test;
    this->ifTrue = ifTrue;
    this->ifFalse = ifFalse;
    this->inspectionCount = 0;
  }
};

std::vector<std::string> split(std::string &input, char delimiter) {
  std::vector<std::string> items;
  std::string item;
  int inputSize = input.size();
  int inputIdx = 0;

  while (inputIdx < inputSize) {
    if (input[inputIdx] == delimiter) {
      items.push_back(item);
      item = "";
      ++inputIdx;
    } else {
      item.push_back(input[inputIdx++]);
    }
  }

  items.push_back(item);

  return items;
}

std::vector<Monkey *> monkeys;
int mod = 1;

void readInput() {
  monkeys.clear();

  std::ifstream inputFile("input.txt");
  std::string line;
  while (true) {
    // monkey id
    std::getline(inputFile, line);
    line = line.substr(std::string("Monkey ").size());
    line.pop_back(); // character : at the end
    auto monkeyId = std::stoi(line);

    // starting items
    std::vector<long long> items;
    std::getline(inputFile, line);
    line = line.substr(std::string("  Starting items: ").size());
    auto itemsStr = split(line, ',');
    for (auto &itemStr : itemsStr) {
      items.push_back(std::stoi(itemStr));
    }

    // operation
    std::getline(inputFile, line);
    auto operation = line.substr(std::string("  Operation: new = ").size());

    // divisible value
    std::getline(inputFile, line);
    auto divisibleValue =
        std::stoi(line.substr(std::string("  Test: divisible by ").size()));

    // monkey if true
    std::getline(inputFile, line);
    auto monkeyIdIfTrue = std::stoi(
        line.substr(std::string("    If true: throw to monkey ").size()));

    // monkey if false
    std::getline(inputFile, line);
    auto monkeyIdIfFalse = std::stoi(
        line.substr(std::string("    If false: throw to monkey ").size()));

    auto monkey = new Monkey(monkeyId, operation, items, divisibleValue,
                             monkeyIdIfTrue, monkeyIdIfFalse);
    monkeys.push_back(monkey);

    if (!std::getline(inputFile, line)) {
      // end of file
      break;
    }
  }
  inputFile.close();

  mod = 1;
  for (auto monkey : monkeys) {
    mod *= monkey->test;
  }
}

void cleanUp() {
  for (auto monkey : monkeys) {
    delete monkey;
  }

  monkeys.clear();
}

long long operate(int old, std::string operation) {
  operation =
      std::regex_replace(operation, std::regex("old"), std::to_string(old));
  operation.erase(std::remove(operation.begin(), operation.end(), ' '),
                  operation.end());

  int operatorIdx = 0;
  for (int i = 0; i < operation.size(); ++i) {
    if (!std::isdigit(operation[i])) {
      operatorIdx = i;
      break;
    }
  }

  long long firstNumber = std::stoi(operation.substr(0, operatorIdx));
  long long secondNumber = std::stoi(operation.substr(operatorIdx + 1));
  if (operation[operatorIdx] == '+') {
    return firstNumber + secondNumber;
  }

  return firstNumber * secondNumber;
}

void roundPart1() {
  for (auto monkey : monkeys) {
    for (auto item : monkey->items) {
      ++monkey->inspectionCount;
      auto val = operate(item, monkey->operation);
      val = val / 3;

      if (val % monkey->test == 0) {
        monkeys[monkey->ifTrue]->items.push_back(val);
      } else {
        monkeys[monkey->ifFalse]->items.push_back(val);
      }
    }

    monkey->items.clear();
  }
}

void roundPart2() {
  for (auto monkey : monkeys) {
    for (auto item : monkey->items) {
      ++monkey->inspectionCount;
      auto val = operate(item, monkey->operation) % mod;

      if (val % monkey->test == 0) {
        monkeys[monkey->ifTrue]->items.push_back(val);
      } else {
        monkeys[monkey->ifFalse]->items.push_back(val);
      }
    }

    monkey->items.clear();
  }
}

long long getMonkeyBusiness() {
  std::vector<long long> inspectionsCount;
  for (auto monkey : monkeys) {
    inspectionsCount.push_back(monkey->inspectionCount);
  }

  std::sort(inspectionsCount.rbegin(), inspectionsCount.rend());

  return inspectionsCount[0] * inspectionsCount[1];
}

int main() {
  // part 1
  readInput();
  for (int i = 0; i < 20; ++i) {
    roundPart1();
  }
  long long monkeyBusiness1 = getMonkeyBusiness();
  cleanUp();

  // part 2
  readInput();
  for (int i = 0; i < 10000; ++i) {
    roundPart2();
  }
  long long monkeyBusiness2 = getMonkeyBusiness();
  cleanUp();

  // print the result
  std::cout << monkeyBusiness1 << std::endl;
  std::cout << monkeyBusiness2 << std::endl;
}
