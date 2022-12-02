#include <iostream>
#include <fstream>
#include <string>

int main() {
  std::ifstream inputFile("input.txt");

  std::string calorie;
  int maxCalories = 0;
  int currentCalories = 0;

  while (std::getline(inputFile, calorie)) {
    if (calorie == "") {
      maxCalories = std::max(maxCalories, currentCalories);
      currentCalories = 0;
    } else {
      currentCalories += std::stoi(calorie);
    }
  }

  maxCalories = std::max(maxCalories, currentCalories);
  std::cout << maxCalories << std::endl;

  inputFile.close();
}
