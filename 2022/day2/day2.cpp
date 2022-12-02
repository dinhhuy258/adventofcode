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

  std::unordered_map<std::string, char> yourChoices = {
    { "A X", 'C' },
    { "A Y", 'A' },
    { "A Z", 'B' },

    { "B X", 'A' },
    { "B Y", 'B' },
    { "B Z", 'C' },

    { "C X", 'B' },
    { "C Y", 'C' },
    { "C Z", 'A' },
  };

  std::ifstream inputFile("input.txt");

  int score1 = 0;
  int score2 = 0;

  std::string round;
  while (std::getline(inputFile, round)) {
    score1 += round[2] - 'X' + 1; 
    score1 += roundScores[round];
  
    score2 += yourChoices[round] - 'A' + 1;
    score2 += (round[2] - 'X') * 3;
  }

  std::cout << score1 << std::endl;
  std::cout << score2 << std::endl;

  inputFile.close();
}
