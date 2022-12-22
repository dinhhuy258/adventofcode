#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

enum Facing { right = 0, down = 1, left = 2, up = 3 };

const auto cubeSize = 50; // according to my test case
int startX = -1, startY = -1;
std::vector<int> dx = {1, 0, -1, 0};
std::vector<int> dy = {0, 1, 0, -1};
std::unordered_set<std::string> tiles;
std::vector<std::string> instructions;
std::vector<std::pair<int, int>> wrappableRows2D;
std::vector<std::pair<int, int>> wrappableCols2D;

std::string getTileKey(int x, int y) {
  return std::to_string(x) + "," + std::to_string(y);
}

void readInput() {
  std::ifstream inputFile("input.txt");
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(inputFile, line)) {
    lines.push_back(line);
  }
  inputFile.close();

  int steps = -1;
  std::string instruction = lines.back();
  for (auto c : instruction) {
    if (std::isdigit(c)) {
      if (steps == -1) {
        steps = 0;
      }
      steps *= 10;
      steps += (c - '0');
    } else {
      if (steps != -1) {
        instructions.push_back(std::to_string(steps));
      }
      instructions.push_back(std::string(1, c));
      steps = -1;
    }
  }
  if (steps != -1) {
    instructions.push_back(std::to_string(steps));
  }

  lines.pop_back(); // instruction
  lines.pop_back(); // blank line

  int maxCol = 0;
  for (auto &line : lines) {
    maxCol = std::max(maxCol, (int)(line.size()));
  }

  wrappableRows2D.push_back(std::make_pair(0, 0)); // dummy row
  wrappableCols2D.push_back(std::make_pair(0, 0)); // dummy col
  while (maxCol > 0) {
    wrappableCols2D.push_back(std::make_pair(std::numeric_limits<int>::max(),
                                             std::numeric_limits<int>::min()));
    --maxCol;
  }

  int y = 1;
  for (auto &line : lines) {
    int x = 1;
    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    for (auto c : line) {
      if (c != ' ') {
        if (c == '.') {
          tiles.insert(getTileKey(x, y));
        }

        if (startX == -1) {
          startX = x;
          startY = y;
        }

        minX = std::min(minX, x);
        maxX = std::max(maxX, x);
        wrappableCols2D[x].first = std::min(wrappableCols2D[x].first, y);
        wrappableCols2D[x].second = std::max(wrappableCols2D[x].second, y);
      }
      ++x;
    }

    wrappableRows2D.push_back(std::make_pair(minX, maxX));

    ++y;
  }
}

std::tuple<int /* x */, int /* y */, int /* direction */>
nextPosition2D(int x, int y, int direction) {
  auto nextX = x + dx[direction];
  auto nextY = y + dy[direction];
  int nextDirection = direction;
  if (direction == right) {
    if (x == wrappableRows2D[y].second) {
      nextX = wrappableRows2D[y].first;
    }
  } else if (direction == down) {
    if (y == wrappableCols2D[x].second) {
      nextY = wrappableCols2D[x].first;
    }
  } else if (direction == left) {
    if (x == wrappableRows2D[y].first) {
      nextX = wrappableRows2D[y].second;
    }
  } else if (direction == up) {
    if (y == wrappableCols2D[x].first) {
      nextY = wrappableCols2D[x].second;
    }
  }

  return std::make_tuple(nextX, nextY, nextDirection);
}

std::tuple<int /* x */, int /* y */, int /* direction */>
nextPosition3D(int x, int y, int direction) {
  auto nextX = x + dx[direction];
  auto nextY = y + dy[direction];
  int nextDirection = direction;
  if (direction == right) {
    if (nextX == 3 * cubeSize + 1) {
      // 5 ->  6
      nextY = cubeSize + 1 - (nextY - 0 * cubeSize) + 2 * cubeSize;
      nextX = 2 * cubeSize;
      nextDirection = left;
    } else if (nextX == 2 * cubeSize + 1 and nextY >= cubeSize + 1 and
               nextY <= 2 * cubeSize) {
      // 4 -> 5
      nextX = nextY + cubeSize;
      nextY = cubeSize;
      nextDirection = up;
    } else if (nextX == 2 * cubeSize + 1 and nextY >= 2 * cubeSize + 1) {
      // 6 -> 5
      nextY = cubeSize + 1 - (nextY - 2 * cubeSize) + 0 * cubeSize;
      nextX = 3 * cubeSize;
      nextDirection = left;
    } else if (nextX == cubeSize + 1 and nextY >= 3 * cubeSize + 1) {
      // 3 -> 6
      nextX = nextY - 2 * cubeSize;
      nextY = 3 * cubeSize;
      nextDirection = up;
    }
  } else if (direction == down) {
    if (nextY == 4 * cubeSize + 1) {
      // 3 -> 5
      nextX = nextX + 2 * cubeSize;
      nextY = 1;
      nextDirection = down;
    } else if (nextY == 3 * cubeSize + 1 and nextX >= cubeSize + 1) {
      // 6 -> 3
      nextY = nextX + 2 * cubeSize;
      nextX = cubeSize;
      nextDirection = left;
    } else if (nextY == cubeSize + 1 and nextX >= 2 * cubeSize + 1) {
      // 5 -> 4
      nextY = nextX - cubeSize;
      nextX = 2 * cubeSize;
      nextDirection = left;
    }
  } else if (direction == left) {
    if (nextX == cubeSize and nextY <= cubeSize) {
      // 1 -> 2
      nextY = cubeSize + 1 - (nextY - 0 * cubeSize) + 2 * cubeSize;
      nextX = 1;
      nextDirection = right;
    } else if (nextX == cubeSize and nextY >= cubeSize + 1 and
               nextY <= 2 * cubeSize) {
      // 4 -> 2
      nextX = nextY - cubeSize;
      nextY = 2 * cubeSize + 1;
      nextDirection = down;
    } else if (nextX == 0 and nextY <= 3 * cubeSize) {
      // 2 -> 1
      nextY = cubeSize + 1 - (nextY - 2 * cubeSize) + 0 * cubeSize;
      nextX = cubeSize + 1;
      nextDirection = right;
    } else if (nextX == 0 and nextY >= 3 * cubeSize + 1) {
      // 3 -> 1
      nextX = nextY - 2 * cubeSize;
      nextY = 1;
      nextDirection = down;
    }
  } else if (direction == up) {
    if (nextY == 2 * cubeSize and nextX <= cubeSize) {
      // 2 -> 4
      nextY = nextX + cubeSize;
      nextX = cubeSize + 1;
      nextDirection = right;
    } else if (nextY == 0 and nextX <= 2 * cubeSize) {
      // 1 -> 3
      nextY = nextX + 2 * cubeSize;
      nextX = 1;
      nextDirection = right;
    } else if (nextY == 0 and nextX >= 2 * cubeSize + 1) {
      // 5 -> 3
      nextX = nextX - 2 * cubeSize;
      nextY = 4 * cubeSize;
      nextDirection = up;
    }
  }

  return std::make_tuple(nextX, nextY, nextDirection);
}

int getPassword(bool is3D) {
  int currentX = startX;
  int currentY = startY;
  int direction = 0; // right
  for (auto instruction : instructions) {
    if (instruction == "R") {
      ++direction;
      if (direction == 4) {
        direction = 0;
      }
    } else if (instruction == "L") {
      --direction;
      if (direction < 0) {
        direction = 3;
      }
    } else {
      int steps = std::stoi(instruction);
      while (steps > 0) {
        auto newX = currentX + dx[direction];
        auto newY = currentY + dy[direction];
        auto newDirection = direction;

        std::tie(newX, newY, newDirection) =
            is3D ? nextPosition3D(currentX, currentY, direction)
                 : nextPosition2D(currentX, currentY, direction);

        if (tiles.find(getTileKey(newX, newY)) == tiles.end()) {
          break;
        }

        currentX = newX;
        currentY = newY;
        direction = newDirection;
        --steps;
      }
    }
  }

  return 1000 * currentY + 4 * currentX + direction;
}

int main() {
  // read input
  readInput();

  // part 1
  std::cout << getPassword(false) << std::endl;

  // part 2
  std::cout << getPassword(true) << std::endl;
}
