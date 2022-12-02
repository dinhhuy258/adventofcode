#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

int main() {
  std::unordered_map<std::string, int> roundScores = {
    { "A X", 3 },
    { "A Y", 6 },
    { "A Z", 0 },

    { "B X", 0 },
    { "B Y", 3 },
    { "B Z", 6 },

    { "C X", 6 },
    { "C Y", 0 },
    { "C Z", 3 },
  };

  std::ifstream inputFile("input.txt");

  std::string round;
  int score = 0;
  while (std::getline(inputFile, round)) {
    char yourChoice = round[2]; 
    score += yourChoice - 'X' + 1; 
    score += roundScores[round];
  }

  std::cout << score << std::endl;

  inputFile.close();
}
