#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <optional>

std::vector<std::pair<int, int>> directions = {
    std::make_pair(0, -1), // north
    std::make_pair(0, 1),  // south
    std::make_pair(-1, 0), // west
    std::make_pair(1, 0),  // east
};

std::set<std::pair<int, int>> elves;

void readInput() {
  std::ifstream inputFile("input.txt");
  std::string line;
  int y = 0;
  while (std::getline(inputFile, line)) {
    int x = 0;
    for (auto c : line) {
      if (c == '#') {
        elves.insert(std::make_pair(x, y));
      }
      ++x;
    }
    ++y;
  }
  inputFile.close();
}

bool hasElve(const std::pair<int, int> &pos) {
  return elves.find(pos) != elves.end();
}

bool hasElveSurrounding(int x, int y) {
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0) {
        continue;
      }

      int newX = x + dx;
      int newY = y + dy;
      if (hasElve(std::make_pair(newX, newY))) {
        return true;
      }
    }
  }

  return false;
}

std::optional<std::pair<int, int>> nextPos(int round, int x, int y) {
  if (!hasElveSurrounding(x, y)) {
    return {};
  }

  for (int i = 0; i < 4; ++i) {
    int idx = round + i;
    idx %= 4;
    int newX = x + directions[idx].first;
    int newY = y + directions[idx].second;

    if ((directions[idx].first == 0 &&
             !hasElve(std::make_pair(newX - 1, newY)) &&
             !hasElve(std::make_pair(newX, newY)) &&
             !hasElve(std::make_pair(newX + 1, newY)) ||
         (directions[idx].second == 0 &&
          !hasElve(std::make_pair(newX, newY - 1)) &&
          !hasElve(std::make_pair(newX, newY)) &&
          !hasElve(std::make_pair(newX, newY + 1))))) {

      return std::make_optional<std::pair<int, int>>(
          std::make_pair(newX, newY));
    }
  }

  return {};
}

int calculateEmptyGroundTiles() {
  int minX, minY, maxX, maxY;
  minX = minY = std::numeric_limits<int>::max();
  maxX = maxY = std::numeric_limits<int>::min();
  for (auto &elve : elves) {
    int x = elve.first;
    int y = elve.second;
    minX = std::min(minX, x);
    maxX = std::max(maxX, x);
    minY = std::min(minY, y);
    maxY = std::max(maxY, y);
  }

  return (maxX - minX + 1) * (maxY - minY + 1) - elves.size();
}

int main() {
  // read input
  readInput();

  int emptyGroupTilesCount = 0;
  int round = 0;
  while (true) {
    std::map<std::pair<int, int>, int> elvesNextPosition;
    // first half round
    for (auto &elve : elves) {
      auto nextPosition = nextPos(round, elve.first, elve.second);
      if (nextPosition.has_value()) {
        ++elvesNextPosition[nextPosition.value()];
      }
    }

    // second half round
    bool hasElveMove = false;
    std::set<std::pair<int, int>> nextElves;
    for (auto &elve : elves) {
      auto nextPosition = nextPos(round, elve.first, elve.second);
      if (nextPosition.has_value() &&
          elvesNextPosition[nextPosition.value()] == 1) {
        hasElveMove = true;
        nextElves.insert(nextPosition.value());
      } else {
        nextElves.insert(elve);
      }
    }

    if (!hasElveMove) {
      break;
    }

    std::swap(elves, nextElves);
    ++round;
    if (round == 10) {
      emptyGroupTilesCount = calculateEmptyGroundTiles();
    }
  }

  std::cout << emptyGroupTilesCount << std::endl;
  std::cout << round + 1 << std::endl;
}
