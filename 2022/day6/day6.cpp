#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

int numberOfProcessedCharacter(std::string& signal, int distinctCharactersCount) {
  std::vector<int> lastIdx('z' - 'a' + 1, -1);
  int leftIdx = 0;
  for (int idx = 0; idx < signal.size(); ++idx) {
    if (lastIdx[signal[idx] - 'a'] >= leftIdx) {
      leftIdx = lastIdx[signal[idx] - 'a'] + 1;
    }

    lastIdx[signal[idx] - 'a'] = idx;

    if (idx - leftIdx + 1 == distinctCharactersCount) {
      break;
    }
  }

  return leftIdx + distinctCharactersCount;
}

int main() {
  std::ifstream inputFile("input.txt");
  std::string signal;
  std::getline(inputFile, signal);

  // part 1
  std::cout << numberOfProcessedCharacter(signal, 4) << std::endl;
  // part 2
  std::cout << numberOfProcessedCharacter(signal, 14) << std::endl;

  inputFile.close();
}
