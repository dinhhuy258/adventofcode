#include <fstream>
#include <iostream>
#include <queue>
#include <string>

int main() {
  std::ifstream inputFile("input.txt");

  std::string calorie;
  int currentCalories = 0;
  // min heap
  std::priority_queue<int, std::vector<int>, std::greater<int>> caloriesQueue;

  while (std::getline(inputFile, calorie)) {
    if (calorie == "") {
      if (caloriesQueue.size() < 3) {
        caloriesQueue.push(currentCalories);
      } else if (caloriesQueue.top() < currentCalories) {
        caloriesQueue.pop();
        caloriesQueue.push(currentCalories);
      }

      currentCalories = 0;
    } else {
      currentCalories += std::stoi(calorie);
    }
  }

  if (caloriesQueue.size() < 3) {
    caloriesQueue.push(currentCalories);
  } else if (caloriesQueue.top() < currentCalories) {
    caloriesQueue.pop();
    caloriesQueue.push(currentCalories);
  }

  int totalTopThreeCalories = 0;
  int maxCalories = 0;

  while (!caloriesQueue.empty()) {
    totalTopThreeCalories += caloriesQueue.top();
    maxCalories = std::max(maxCalories, caloriesQueue.top());
    caloriesQueue.pop();
  }

  std::cout << maxCalories << std::endl;
  std::cout << totalTopThreeCalories << std::endl;

  inputFile.close();
}
