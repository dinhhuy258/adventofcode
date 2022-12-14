#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tgmath.h>
#include <bitset>

int getSumPrioritiesOfSharedItems(std::string& runsack) {
  int priority = 0;
  int itemsCount = runsack.size();
  long itemMask = 0;

  for (int i = 0; i < itemsCount / 2; ++i) {
    if (runsack[i] <= 'Z') {
      itemMask |= (long(1) << (runsack[i] - 'A' + 26)); 
    } else {
      itemMask |= (1 << (runsack[i] - 'a'));
    }
  }

  for (int i = itemsCount / 2; i < itemsCount; ++i) {
    long mask = runsack[i] <= 'Z' ? (long(1) << (runsack[i] - 'A' + 26)) : (1 << (runsack[i] - 'a'));
    if ((itemMask & mask) != 0) {
      priority += (runsack[i] <= 'Z') ? (runsack[i] - 'A' + 27) : (runsack[i] - 'a' + 1);
      itemMask ^= mask;
    }
  }

  return priority;
}

int getPriorityOfGroupBadge(std::vector<std::string>& group) {
  int priority = 0;
  long itemMask = -1;
  for (int i = 0; i < group.size(); ++i) {
    long mask = 0;
    for (auto c: group[i]) {
      if (c <= 'Z') {
        mask |= (long(1) << (c - 'A' + 26)); 
      } else {
        mask |= (1 << (c - 'a'));
      }
    }

    if (itemMask == -1) {
      itemMask = mask;
    } else {
      itemMask &= mask;
    }
  }

  // itemMask has only one bit set
  return std::log2(itemMask & -itemMask) + 1;
}

int main() {
  std::ifstream inputFile("input.txt");
  std::string rucksack;

  int sumPriorities = 0;
  int sumPrioritiesOfGroup = 0;

  std::vector<std::string> group;
  while (std::getline(inputFile, rucksack)) {
    sumPriorities += getSumPrioritiesOfSharedItems(rucksack);
    group.push_back(rucksack);
    if (group.size() == 3) {
      sumPrioritiesOfGroup += getPriorityOfGroupBadge(group);
      group.clear();
    }
  }

  std::cout << sumPriorities << std::endl;
  std::cout << sumPrioritiesOfGroup << std::endl;

  inputFile.close();
}
