#include <fstream>
#include <iostream>
#include <vector>

std::vector<std::string> grid;

void readGrid() {
  std::ifstream inputFile("input.txt");
  std::string row;

  while (std::getline(inputFile, row)) {
    grid.push_back(row);
  }

  inputFile.close();
}

int main() {
  // read input
  readGrid();

  int h = grid.size();
  int w = grid.back().size();

  // precompute max-right, max-left, max-top, max-bottom
  std::vector<std::vector<int>> nextGreaterLeft(h, std::vector<int>(w, -1));
  std::vector<std::vector<int>> nextGreaterRight(h, std::vector<int>(w, -1));

  for (int r = 1; r < h - 1; ++r) {
    std::stack<int> leftStack;
    leftStack.push(0);
    for (int c = 1; c < w - 1; ++c) {
      while (!leftStack.empty() && grid[r][leftStack.top()] < grid[r][c]) {
        leftStack.pop();
      }

      if (!leftStack.empty()) {
        nextGreaterLeft[r][c] = leftStack.top();
      }

      leftStack.push(c);
    }

    std::stack<int> rightStack;
    rightStack.push(w - 1);
    for (int c = w - 2; c > 0; --c) {
      while (!rightStack.empty() && grid[r][rightStack.top()] < grid[r][c]) {
        rightStack.pop();
      }

      if (!rightStack.empty()) {
        nextGreaterRight[r][c] = rightStack.top();
      }

      rightStack.push(c);
    }
  }

  std::vector<std::vector<int>> nextGreaterTop(h, std::vector<int>(w, -1));
  std::vector<std::vector<int>> nextGreaterBottom(h, std::vector<int>(w, -1));
  for (int c = 1; c < w - 1; ++c) {
    std::stack<int> topStack;
    topStack.push(0);
    for (int r = 1; r < h - 1; ++r) {
      while (!topStack.empty() && grid[topStack.top()][c] < grid[r][c]) {
        topStack.pop();
      }

      if (!topStack.empty()) {
        nextGreaterTop[r][c] = topStack.top();
      }

      topStack.push(r);
    }

    std::stack<int> bottomStack;
    bottomStack.push(h - 1);
    for (int r = h - 2; r > 0; --r) {
      while (!bottomStack.empty() && grid[bottomStack.top()][c] < grid[r][c]) {
        bottomStack.pop();
      }

      if (!bottomStack.empty()) {
        nextGreaterBottom[r][c] = bottomStack.top();
      }

      bottomStack.push(r);
    }
  }

  // part 1
  int visibleTrees = w * 2 + h * 2 - 4;

  for (int r = 1; r < h - 1; ++r) {
    for (int c = 1; c < w - 1; ++c) {
      if (nextGreaterLeft[r][c] == -1 || nextGreaterRight[r][c] == -1 ||
          nextGreaterTop[r][c] == -1 || nextGreaterBottom[r][c] == -1) {
        ++visibleTrees;
      }
    }
  }

  // part 2
  int highestScenicScore = 0;

  for (int r = 1; r < h - 1; ++r) {
    for (int c = 1; c < w - 1; ++c) {
      char val = grid[r][c];

      int scenicScore = 1;
      scenicScore *= nextGreaterLeft[r][c] == -1 ? c : (c - nextGreaterLeft[r][c]);
      scenicScore *= nextGreaterRight[r][c] == -1 ? w - c - 1 : (nextGreaterRight[r][c] - c);
      scenicScore *= nextGreaterTop[r][c] == -1 ? r : (r - nextGreaterTop[r][c]);
      scenicScore *= nextGreaterBottom[r][c] == -1 ? h - r - 1 : (nextGreaterBottom[r][c] - r);

      highestScenicScore = std::max(highestScenicScore, scenicScore);
    }
  }

  std::cout << visibleTrees << std::endl;
  std::cout << highestScenicScore << std::endl;
}
