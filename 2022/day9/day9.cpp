#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::unordered_map<char, std::pair<int, int>> directions{
    {'L', std::make_pair(-1, 0)},
    {'R', std::make_pair(1, 0)},
    {'U', std::make_pair(0, -1)},
    {'D', std::make_pair(0, 1)},
};

class Position {
private:
  int x;
  int y;

public:
  Position(int x, int y) : x(x), y(y) {}
  int getX() const { return x; }
  int getY() const { return y; }
  void move(char direction) {
    x += directions[direction].first;
    y += directions[direction].second;
  }
  void follow(Position &pos) {
    int dx = std::abs(pos.x - x);
    int dy = std::abs(pos.y - y);

    if (dx <= 1 && dy <= 1) {
      return;
    }

    if (dx != 0) {
      x += (pos.x > x) ? 1 : -1;
    }

    if (dy != 0) {
      y += (pos.y > y) ? 1 : -1;
    }
  }
  std::string toString() { return std::to_string(x) + "," + std::to_string(y); }
};

int main() {
  std::ifstream inputFile("input.txt");
  std::vector<std::string> moves;
  std::string move;
  while (std::getline(inputFile, move)) {
    moves.push_back(move);
  }
  inputFile.close();

  // part 1
  Position head(0, 0);
  Position tail(0, 0);
  std::unordered_set<std::string> visitedPositions;
  visitedPositions.insert(tail.toString());
  for (auto &move : moves) {
    char direction = move[0];
    int stepCount = std::stoi(move.substr(2));

    while (stepCount > 0) {
      head.move(direction);
      tail.follow(head);
      visitedPositions.insert(tail.toString());

      --stepCount;
    }
  }
  int visitedCountPart1 = visitedPositions.size();

  // part 2
  head = Position(0, 0);
  std::vector<Position> tails(9, Position(0, 0));
  visitedPositions.clear();
  visitedPositions.insert(tails.back().toString());

  for (auto &move : moves) {
    char direction = move[0];
    int stepCount = std::stoi(move.substr(2));

    while (stepCount > 0) {
      head.move(direction);
      for (int i = 0; i < 9; ++i) {
        if (i == 0) {
          tails[i].follow(head);
        } else {
          tails[i].follow(tails[i - 1]);
        }
      }
      visitedPositions.insert(tails.back().toString());

      --stepCount;
    }
  }
  int visitedCountPart2 = visitedPositions.size();

  std::cout << visitedCountPart1 << std::endl;
  std::cout << visitedCountPart2 << std::endl;
}
