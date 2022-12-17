#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#define TOWNER_WIDE 7
#define Y_OFFSET 3

int EMPTY = 0;
std::vector<std::vector<int>> rocks{
    {
        0b0011110,
    },
    {
        0b0001000,
        0b0011100,
        0b0001000,
    },
    {
        0b0000100,
        0b0000100,
        0b0011100,
    },
    {
        0b0010000,
        0b0010000,
        0b0010000,
        0b0010000,
    },
    {
        0b0011000,
        0b0011000,
    },
};

int rocksIdx;
std::string jetPatterns;
int jetPatternsIdx;

void readInput() {
  std::ifstream inputFile("input.txt");
  std::string line;
  std::getline(inputFile, jetPatterns);
  inputFile.close();
}

bool canFall(int y, std::vector<int> &rock, std::vector<int> &tower) {
  if (y > tower.size()) {
    return true;
  }
  if (y == 0) {
    return false;
  }

  int rockSize = rock.size();
  int towerSize = tower.size();

  for (int i = rockSize - 1; i >= 0; --i) {
    int cy = y + (rockSize - i - 1);
    if (cy > towerSize) {
      continue;
    }

    auto towerLine = tower[cy - 1];
    if ((rock[i] & towerLine) != 0) {
      return false;
    }
  }

  return true;
}

bool canMoveLeft(int y, std::vector<int> &rock, std::vector<int> &tower) {
  int rockSize = rock.size();
  int towerSize = tower.size();

  for (int i = 0; i < rockSize; ++i) {
    if ((rock[i] & (1 << (TOWNER_WIDE - 1))) != 0) {
      return false;
    }

    int ly = y + (rock.size() - i - 1);
    if (ly >= towerSize) {
      continue;
    }
    auto towerLine = tower[ly];

    if (((rock[i] << 1) & towerLine) != 0) {
      return false;
    }
  }

  return true;
}

bool canMoveRight(int y, std::vector<int> &rock, std::vector<int> &tower) {
  int rockSize = rock.size();
  int towerSize = tower.size();

  for (int i = 0; i < rockSize; ++i) {
    if ((rock[i] & 1) != 0) {
      return false;
    }

    int ly = y + (rock.size() - i - 1);
    if (ly >= towerSize) {
      continue;
    }

    if (ly >= towerSize) {
      continue;
    }
    auto towerLine = tower[ly];
    if (((rock[i] >> 1) & towerLine) != 0) {
      return false;
    }
  }

  return true;
}

void moveLeft(int y, std::vector<int> &rock, std::vector<int> &tower) {
  if (!canMoveLeft(y, rock, tower)) {
    return;
  }

  // move left
  int rockSize = rock.size();
  for (int i = 0; i < rockSize; ++i) {
    rock[i] = rock[i] << 1;
  }
}

void moveRight(int y, std::vector<int> &rock, std::vector<int> &tower) {
  if (!canMoveRight(y, rock, tower)) {
    return;
  }

  // move right
  int rockSize = rock.size();
  for (int i = 0; i < rockSize; ++i) {
    rock[i] = rock[i] >> 1;
  }
}

void simulate(std::vector<int> &tower) {
  int y = tower.size() + Y_OFFSET;
  auto rock = rocks[rocksIdx];
  ++rocksIdx;
  rocksIdx %= rocks.size();

  while (true) {
    auto &pattern = jetPatterns[jetPatternsIdx];
    ++jetPatternsIdx;
    jetPatternsIdx %= jetPatterns.size();

    if (pattern == '<') {
      moveLeft(y, rock, tower);
    } else {
      moveRight(y, rock, tower);
    }

    if (!canFall(y, rock, tower)) {
      int towerSize = tower.size();
      for (int i = rock.size() - 1; i >= 0; --i) {
        int ly = y + (rock.size() - i - 1);
        if (ly < towerSize) {
          tower[ly] = tower[ly] | rock[i];
        } else {
          tower.push_back(rock[i]);
        }
      }

      break;
    }

    --y;
  }
}

std::string getStateKey(std::vector<int> &tower) {
  std::string state = "";
  for (int i = 0; i < TOWNER_WIDE; ++i) {
    int h = Y_OFFSET;
    for (int j = tower.size() - 1; j >= 0; --j) {
      if ((tower[j] & (1 << i)) != 0) {
        break;
      }

      ++h;
    }

    state = state + std::to_string(h) + "-";
  }

  state += std::to_string(rocksIdx);
  state += "-" + std::to_string(jetPatternsIdx);

  return state;
}

long calculateTowerTall(long rockCount) {
  rocksIdx = 0;
  jetPatternsIdx = 0;
  std::vector<int> tower;
  std::unordered_map<std::string, std::pair<int /* tall */, int /* idx */>>
      states;
  long idx = 0;

  while (rockCount > 0) {
    ++idx;

    auto state = getStateKey(tower);
    // calculate state
    if (states.find(state) != states.end()) {
      auto [h, i] = states[state];

      long diffHeight = tower.size() - h + 2;
      long towerTall = diffHeight * (rockCount / (idx - i)) + tower.size();
      long remaining = rockCount % (idx - i);
      long towerSizeBefore = tower.size();

      while (remaining > 0) {
        simulate(tower);
        --remaining;
      }

      return towerTall + tower.size() - towerSizeBefore;
    }

    simulate(tower);

    states[state] = std::make_pair(tower.size(), idx);

    --rockCount;
  }

  return tower.size();
}

int main() {
  // read the input
  readInput();

  // part 1
  std::cout << calculateTowerTall(2022) << std::endl;

  // part 2
  std::cout << calculateTowerTall(1000000000000) << std::endl;
}
