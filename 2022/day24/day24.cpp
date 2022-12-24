#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

int width, height;

std::vector<std::pair<int, int>> directions = {
    std::make_pair(0, -1), // north
    std::make_pair(0, 1),  // south
    std::make_pair(-1, 0), // west
    std::make_pair(1, 0),  // east
};

std::map<std::pair<int, int> /* position */, std::vector<int> /* directions */>
    blizzards;

void readInput() {
  std::ifstream inputFile("input.txt");
  std::string line;
  width = 0;
  height = 0;
  while (std::getline(inputFile, line)) {
    width = line.size();
    for (int x = 0; x < width; ++x) {
      switch (line[x]) {
      case '>':
        blizzards[std::make_pair(x, height)] = {3};
        break;
      case '<':
        blizzards[std::make_pair(x, height)] = {2};
        break;
      case 'v':
        blizzards[std::make_pair(x, height)] = {1};
        break;
      case '^':
        blizzards[std::make_pair(x, height)] = {0};
        break;
      }
    }
    ++height;
  }
  inputFile.close();
}

int calculateMinutesToReachTheGoal(std::pair<int, int> startPos,
                                   std::pair<int, int> endPos) {
  int minutes = 0;

  std::set<std::pair<int, int>> states;
  states.insert(startPos);
  while (true) {
    std::set<std::pair<int, int>> newStates;

    // update blizzards
    std::map<std::pair<int, int> /* position */,
             std::vector<int> /* directions */>
        nextBlizzards;
    for (auto &[blizzardPosition, blizzardDirections] : blizzards) {
      int x = blizzardPosition.first;
      int y = blizzardPosition.second;

      for (auto blizzardDirection : blizzardDirections) {
        int newX = x + directions[blizzardDirection].first;
        int newY = y + directions[blizzardDirection].second;
        if (newX == 0) {
          newX = width - 2;
        } else if (newX == width - 1) {
          newX = 1;
        } else if (newY == 0) {
          newY = height - 2;
        } else if (newY == height - 1) {
          newY = 1;
        }

        nextBlizzards[std::make_pair(newX, newY)].push_back(blizzardDirection);
      }
    }
    blizzards = nextBlizzards;

    for (auto pos : states) {
      // not move
      if (blizzards.find(pos) == blizzards.end()) {
        newStates.insert(pos);
      }

      for (auto &d : directions) {
        auto newX = d.first + pos.first;
        auto newY = d.second + pos.second;
        if (newX == endPos.first && newY == endPos.second) {
          return minutes + 1;
        }

        if (newX <= 0 || newX >= width - 1 || newY <= 0 || newY >= height - 1 ||
            blizzards.find(std::make_pair(newX, newY)) != blizzards.end()) {
          continue;
        }

        newStates.insert(std::make_pair(newX, newY));
      }
    }

    states = newStates;

    ++minutes;
  }

  return -1;
}

int main() {
  // read input
  readInput();

  // part 1
  auto startPos = std::make_pair(1, 0);
  auto endPos = std::make_pair(width - 2, height - 1);
  int part1 = calculateMinutesToReachTheGoal(startPos, endPos);
  int part2 = part1;
  part2 += calculateMinutesToReachTheGoal(endPos, startPos);
  part2 += calculateMinutesToReachTheGoal(startPos, endPos);

  std::cout << part1 << std::endl;
  std::cout << part2 << std::endl;
}
