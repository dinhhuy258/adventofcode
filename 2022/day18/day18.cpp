#include <fstream>
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

// this value is depend on the test case, increase this if the input contains
// coordinate > 26
#define MAX_SIZE 26

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

std::vector<std::vector<std::vector<bool>>>
    cube(MAX_SIZE,
         std::vector<std::vector<bool>>(MAX_SIZE,
                                        std::vector<bool>(MAX_SIZE, false)));
std::vector<std::tuple<int, int, int>> points;

void readInput() {
  std::ifstream inputFile("input.txt");
  std::string line;
  while (std::getline(inputFile, line)) {
    auto coordinates = split(line, ",");
    int x = std::stoi(coordinates[0]) + 1;
    int y = std::stoi(coordinates[1]) + 1;
    int z = std::stoi(coordinates[2]) + 1;
    cube[x][y][z] = true;
    points.push_back(std::make_tuple(x, y, z));
  }
  inputFile.close();
}

int getArea() {
  int area = 0;
  for (auto point : points) {
    int x, y, z;
    std::tie(x, y, z) = point;

    if (!cube[x + 1][y][z]) {
      ++area;
    }
    if (!cube[x - 1][y][z]) {
      ++area;
    }
    if (!cube[x][y + 1][z]) {
      ++area;
    }
    if (!cube[x][y - 1][z]) {
      ++area;
    }
    if (!cube[x][y][z + 1]) {
      ++area;
    }
    if (!cube[x][y][z - 1]) {
      ++area;
    }
  }

  return area;
}

int getExteriorArea() {
  std::vector<std::vector<std::vector<char>>> grid(
      MAX_SIZE, std::vector<std::vector<char>>(
                    MAX_SIZE, std::vector<char>(MAX_SIZE, ' ')));
  std::vector<std::tuple<int, int, int>> candiates;
  for (auto &point : points) {
    int x, y, z;
    std::tie(x, y, z) = point;
    grid[x][y][z] = '#';
    if (!cube[x + 1][y][z]) {
      candiates.push_back(std::make_tuple(x + 1, y, z));
    }
    if (!cube[x - 1][y][z]) {
      candiates.push_back(std::make_tuple(x - 1, y, z));
    }
    if (!cube[x][y + 1][z]) {
      candiates.push_back(std::make_tuple(x, y + 1, z));
    }
    if (!cube[x][y - 1][z]) {
      candiates.push_back(std::make_tuple(x, y - 1, z));
    }
    if (!cube[x][y][z + 1]) {
      candiates.push_back(std::make_tuple(x, y, z + 1));
    }
    if (!cube[x][y][z - 1]) {
      candiates.push_back(std::make_tuple(x, y, z - 1));
    }
  }

  std::queue<std::tuple<int, int, int>> queue;
  // point (1, 1, 1) may not work in some test case
  queue.push(std::make_tuple(1, 1, 1));
  while (!queue.empty()) {
    int x, y, z;
    std::tie(x, y, z) = queue.front();
    queue.pop();

    if (x + 1 < MAX_SIZE && grid[x + 1][y][z] == ' ') {
      grid[x + 1][y][z] = '.';
      queue.push(std::make_tuple(x + 1, y, z));
    }
    if (x - 1 >= 0 && grid[x - 1][y][z] == ' ') {
      grid[x - 1][y][z] = '.';
      queue.push(std::make_tuple(x - 1, y, z));
    }

    if (y + 1 < MAX_SIZE && grid[x][y + 1][z] == ' ') {
      grid[x][y + 1][z] = '.';
      queue.push(std::make_tuple(x, y + 1, z));
    }
    if (y - 1 >= 0 && grid[x][y - 1][z] == ' ') {
      grid[x][y - 1][z] = '.';
      queue.push(std::make_tuple(x, y - 1, z));
    }

    if (z + 1 < MAX_SIZE && grid[x][y][z + 1] == ' ') {
      grid[x][y][z + 1] = '.';
      queue.push(std::make_tuple(x, y, z + 1));
    }
    if (z - 1 < MAX_SIZE && grid[x][y][z - 1] == ' ') {
      grid[x][y][z - 1] = '.';
      queue.push(std::make_tuple(x, y, z - 1));
    }
  }

  int area = 0;

  for (auto &candiate : candiates) {
    int x, y, z;
    std::tie(x, y, z) = candiate;
    if (grid[x][y][z] == '.') {
      ++area;
    }
  }

  return area;
}

int main() {
  // read input
  readInput();

  // part 1
  std::cout << getArea() << std::endl;

  // part 2
  std::cout << getExteriorArea() << std::endl;
}
