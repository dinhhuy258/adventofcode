#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

class Point {
private:
  int x;
  int y;

public:
  Point(int x, int y) : x(x), y(y) {}
  int getX() const { return x; }
  int getY() const { return y; }
  bool operator==(const Point &p) const { return p.x == x && p.y == y; }
};

namespace std {
template <> struct hash<Point> {
  auto operator()(const Point &p) const -> size_t {
    return std::hash<int>()(p.getX()) ^ std::hash<int>()(p.getY());
  }
};
} // namespace std

std::vector<std::string> split(const std::string &str,
                               const std::string &delimeter) {
  std::vector<std::string> strs;
  char *token = std::strtok(const_cast<char *>(str.c_str()), delimeter.c_str());
  while (token != nullptr) {
    strs.push_back(token);
    token = std::strtok(nullptr, delimeter.c_str());
  }

  return strs;
}

std::unordered_set<Point> obstacles;
int maxObstacleHeight = 0;

void readCave(bool hasInfiniteHorizontalLine) {
  std::ifstream inputFile("input.txt");
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(inputFile, line)) {
    lines.push_back(line);
  }
  inputFile.close();

  maxObstacleHeight = 0;
  std::vector<std::vector<Point>> points;
  for (auto &line : lines) {
    std::vector<Point> ps;
    for (auto &str : split(line, " -> ")) {
      auto coordinate = split(str, ",");
      auto x = std::stoi(coordinate[0]);
      auto y = std::stoi(coordinate[1]);

      ps.push_back(Point(x, y));
      maxObstacleHeight = std::max(maxObstacleHeight, y);
    }
    points.push_back(ps);
  }

  if (hasInfiniteHorizontalLine) {
    maxObstacleHeight += 2;
  }

  obstacles.clear();
  for (auto &ps : points) {
    auto &lastPoint = ps[0];
    for (int i = 1; i < ps.size(); ++i) {
      for (int y = std::min(lastPoint.getY(), ps[i].getY());
           y <= std::max(lastPoint.getY(), ps[i].getY()); ++y) {
        for (int x = std::min(lastPoint.getX(), ps[i].getX());
             x <= std::max(lastPoint.getX(), ps[i].getX()); ++x) {
          obstacles.insert(Point(x, y));
        }
      }

      lastPoint = ps[i];
    }
  }
}

bool updateSandPosition(Point sandPosition, bool hasInfiniteHorizontalLine) {
  // go down
  int sandPositionY = sandPosition.getY();
  int sandPositionX = sandPosition.getX();
  if (obstacles.find(Point(sandPositionX, sandPositionY)) != obstacles.end()) {
    // the sand is blocked already
    return false;
  }

  while (sandPositionY <= maxObstacleHeight &&
         obstacles.find(Point(sandPositionX, sandPositionY + 1)) ==
             obstacles.end() && (!hasInfiniteHorizontalLine || sandPositionY + 1 < maxObstacleHeight)) {
    ++sandPositionY;
  }

  if (sandPositionY > maxObstacleHeight) {
    // the sand is falling into the endless void
    return false;
  }

  if (obstacles.find(Point(sandPositionX - 1, sandPositionY + 1)) ==
          obstacles.end() &&
      (!hasInfiniteHorizontalLine || sandPositionY + 1 < maxObstacleHeight)) {
    return updateSandPosition(Point(sandPositionX - 1, sandPositionY + 1),
                              hasInfiniteHorizontalLine);
  }

  if (obstacles.find(Point(sandPositionX + 1, sandPositionY + 1)) ==
          obstacles.end() &&
      (!hasInfiniteHorizontalLine || sandPositionY + 1 < maxObstacleHeight)) {
    return updateSandPosition(Point(sandPositionX + 1, sandPositionY + 1),
                              hasInfiniteHorizontalLine);
  }

  obstacles.insert(Point(sandPositionX, sandPositionY));

  return true;
}

int main() {
  Point startSandPosition(500, 0);

  // part 1
  readCave(false);
  int sandCountPart1 = 0;
  while (updateSandPosition(startSandPosition, false)) {
    ++sandCountPart1;
  }

  // part 2
  int sandCountPart2 = 0;
  readCave(true);
  while (updateSandPosition(startSandPosition, true)) {
    ++sandCountPart2;
  }

  std::cout << sandCountPart1 << std::endl;
  std::cout << sandCountPart2 << std::endl;
}
