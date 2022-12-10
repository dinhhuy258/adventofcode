#include <fstream>
#include <iostream>
#include <vector>

constexpr std::size_t CRT_WIDTH = 40;
constexpr std::size_t CRT_HEIGHT = 6;

class CPU {
private:
  std::vector<std::string> crt;
  int registerX;
  int cycles;
  int signalStrength;

private:
  int getCycles(std::string &instruction) {
    if (instruction == "noop") {
      return 1;
    }

    return 2;
  }

public:
  CPU()
      : registerX(1), cycles(0), signalStrength(0),
        crt(CRT_HEIGHT, std::string(CRT_WIDTH, '.')) {}

  void boostSignalStrength() {
    if ((cycles + 20) % 40 == 0) {
      signalStrength += (cycles * registerX);
    }
  }

  void run(std::string &instruction) {
    int cyclesInstruction = getCycles(instruction);

    while (cyclesInstruction > 0) {
      ++cycles;
      // part 1
      boostSignalStrength();
      // part 2
      int crtCol = (cycles - 1) % CRT_WIDTH;
      int crtRow = (cycles - 1) / CRT_WIDTH;
      // X register controls the horizontal position of a sprite
      // specifically, the sprite is 3 pixels wide, and the X register sets the
      // horizontal position of the middle of that sprite

      // if the sprite is positioned such that one of its three pixels is the
      // pixel currently being drawn, the screen produces a lit pixel (#)
      // otherwise, the screen leaves the pixel dark (.)
      if (std::abs(registerX - crtCol) <= 1) {
        crt[crtRow][crtCol] = '#';
      }

      --cyclesInstruction;
    }

    if (instruction != "noop") {
      registerX += std::stoi(instruction.substr(std::string("addx ").size()));
    }
  }

  int getSignalStrength() { return signalStrength; }

  const std::vector<std::string> &getCrt() { return crt; }
};

std::vector<std::string> readInstructions() {
  std::vector<std::string> instructions;
  std::ifstream inputFile("input.txt");
  std::string instruction;
  while (std::getline(inputFile, instruction)) {
    instructions.push_back(instruction);
  }
  inputFile.close();

  return instructions;
}

int main() {
  CPU cpu;
  auto instructions = readInstructions();
  for (auto &instruction : instructions) {
    cpu.run(instruction);
  }

  std::cout << cpu.getSignalStrength() << std::endl;
  for (const auto &row : cpu.getCrt()) {
    std::cout << row << std::endl;
  }
}
