#include <cctype>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Command {
private:
  int quantity;
  int from;
  int to;

public:
  Command(std::string &commandStr) {
    int idx = 5;

    std::string quantityStr = "";
    while (idx < commandStr.size() && std::isdigit(commandStr[idx])) {
      quantityStr.push_back(commandStr[idx]);
      ++idx;
    }

    while (idx < commandStr.size() && !std::isdigit(commandStr[idx])) {
      ++idx;
    }

    std::string fromStr = "";
    while (idx < commandStr.size() && std::isdigit(commandStr[idx])) {
      fromStr.push_back(commandStr[idx]);
      ++idx;
    }

    while (idx < commandStr.size() && !std::isdigit(commandStr[idx])) {
      ++idx;
    }

    std::string toStr = "";
    while (idx < commandStr.size() && std::isdigit(commandStr[idx])) {
      toStr.push_back(commandStr[idx]);
      ++idx;
    }

    quantity = std::stoi(quantityStr);
    from = std::stoi(fromStr);
    to = std::stoi(toStr);
  }

  int getQuantity() { return quantity; }

  int getFrom() { return from; }

  int getTo() { return to; }
};

std::vector<std::string> stacks;
std::vector<Command> commands;

std::string readTopStack() {
  std::string topStack = "";
  for (auto &stack : stacks) {
    topStack += stack.back();
  }

  return topStack;
}

void readInput() {
  std::ifstream inputFile("input.txt");
  std::string inputLine;
  // according to the input file, we have 9 stacks
  stacks = std::vector<std::string>(9, "");
  commands.clear();

  // according to the input file, each stack have maximum of 8 boxes
  for (int i = 0; i < 8; ++i) {
    std::getline(inputFile, inputLine);
    for (int j = 1; j < inputLine.size(); j += 4) {
      if (inputLine[j] != ' ') {
        stacks[j / 4] = std::string(1, inputLine[j]) + stacks[j / 4];
      }
    }
  }

  while (std::getline(inputFile, inputLine)) {
    if (inputLine.substr(0, 4) == "move") {
      commands.emplace_back(inputLine);
    }
  }

  inputFile.close();
}

int main() {
  // CrateMover 9000
  readInput();
  for (auto &command : commands) {
    int quantity = command.getQuantity();
    int from = command.getFrom() - 1;
    int to = command.getTo() - 1;

    for (int i = 0; i < quantity; ++i) {
      auto box = stacks[from].back();
      stacks[from].pop_back();
      stacks[to].push_back(box);
    }
  }

  std::cout << readTopStack() << std::endl;

  // CrateMover 9001
  readInput();
  for (auto &command : commands) {
    int quantity = command.getQuantity();
    int from = command.getFrom() - 1;
    int to = command.getTo() - 1;

    auto movedBoxes = stacks[from].substr(stacks[from].size() - quantity);
    stacks[from].erase(stacks[from].size() - movedBoxes.size());
    stacks[to] += movedBoxes;
  }

  std::cout << readTopStack() << std::endl;
}
