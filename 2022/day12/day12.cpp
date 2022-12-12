#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

std::vector<int> dx = {-1, 0, 1, 0};
std::vector<int> dy = {0, -1, 0, 1};

class Position {
private:
  int x;
  int y;

public:
  Position(int y, int x) : x(x), y(y) {}
  int getX() const { return x; }
  int getY() const { return y; }
  bool operator==(const Position &p) { return x == p.x && y == p.y; }
};

std::vector<std::string> readGrid() {
  std::vector<std::string> grid;
  std::ifstream inputFile("input.txt");
  std::string line;
  while (std::getline(inputFile, line)) {
    grid.push_back(line);
  }
  inputFile.close();

  return grid;
}

std::vector<Position> getLowestPositions(std::vector<std::string> &grid) {
  auto h = grid.size();
  auto w = grid.back().size();

  std::vector<Position> lowestPositions;

  for (int r = 0; r < h; ++r) {
    for (int c = 0; c < w; ++c) {
      if (grid[r][c] == 'S' || grid[r][c] == 'a') {
        lowestPositions.push_back(Position(r, c));
      }
    }
  }

  return lowestPositions;
}

Position getStartPosition(std::vector<std::string> &grid) {
  auto h = grid.size();
  auto w = grid.back().size();

  for (int r = 0; r < h; ++r) {
    for (int c = 0; c < w; ++c) {
      if (grid[r][c] == 'S') {
        return Position(r, c);
      }
    }
  }

  // not reachable
  return Position(-1, -1);
}

Position getEndPosition(std::vector<std::string> &grid) {
  int h = grid.size();
  int w = grid.back().size();

  for (int r = 0; r < h; ++r) {
    for (int c = 0; c < w; ++c) {
      if (grid[r][c] == 'E') {
        return Position(r, c);
      }
    }
  }

  // not reachable
  return Position(-1, -1);
}

int minStepsToReachEnd(std::vector<std::string> &grid,
                       const std::vector<Position> &startPositions,
                       Position &endPosition) {
  int h = grid.size();
  int w = grid.back().size();
  std::vector<std::vector<bool>> visited(h, std::vector<bool>(w, false));
  std::queue<Position> positionsQueue;
  for (auto startPosition : startPositions) {
    positionsQueue.push(startPosition);
    visited[startPosition.getY()][startPosition.getX()] = true;
  }

  int step = 0;
  while (!positionsQueue.empty()) {
    int positionsQueueSize = positionsQueue.size();

    while (positionsQueueSize > 0) {
      auto currentPosition = positionsQueue.front();
      positionsQueue.pop();

      if (currentPosition == endPosition) {
        return step;
      }

      auto currentElevation =
          grid[currentPosition.getY()][currentPosition.getX()];
      currentElevation = currentElevation == 'S' ? 'a' : currentElevation;

      for (int i = 0; i < 4 /* 4 direction */; ++i) {
        auto newX = currentPosition.getX() + dx[i];
        auto newY = currentPosition.getY() + dy[i];
        if (newX < 0 || newX >= w || newY < 0 || newY >= h ||
            visited[newY][newX]) {
          continue;
        }

        auto nextElevation = grid[newY][newX];
        nextElevation = nextElevation == 'E' ? 'z' : nextElevation;

        if (nextElevation - currentElevation > 1) {
          continue;
        }

        visited[newY][newX] = true;
        positionsQueue.push(Position(newY, newX));
      }

      --positionsQueueSize;
    }

    ++step;
  }

  // not reachable
  return -1;
}

int main() {
  auto grid = readGrid();

  auto endPosition = getEndPosition(grid);
  // part 1
  auto startPosition = getStartPosition(grid);
  auto minStepPart1 = minStepsToReachEnd(grid, {startPosition}, endPosition);
  // part 2
  auto startPositions = getLowestPositions(grid);
  auto minStepPart2 = minStepsToReachEnd(grid, startPositions, endPosition);
  // output
  std::cout << minStepPart1 << std::endl;
  std::cout << minStepPart2 << std::endl;
}
