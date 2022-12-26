#include <fstream>
#include <iostream>
#include <vector>

long toDecimal(const std::string &num) {
  auto toDigit = [](char d) {
    if (d == '-') {
      return -1;
    } else if (d == '=') {
      return -2;
    }

    return d - '0';
  };

  long base = 1;
  long numSize = num.size();
  long decimal = 0;
  for (long i = numSize - 1; i >= 0; --i) {
    decimal += base * toDigit(num[i]);
    base *= 5;
  }

  return decimal;
}

char toSnafuDigit(long decimalDigit) {
  switch (decimalDigit) {
  case 2:
    return '2';
  case 1:
    return '1';
  case 0:
    return '0';
  case -1:
    return '-';
  case -2:
    return '=';
  };
  // not reach able
  return ' ';
}

std::string toSnafu(long decimal) {
  std::string snafu = "";
  do {
    long fives = (decimal + 2) / 5;
    long digit = (long)(decimal - 5 * fives);
    snafu.insert(snafu.begin(), toSnafuDigit(digit));
    decimal = fives;
  } while (decimal != 0);

  return snafu;
}

int main() {
  // read input
  std::ifstream inputFile("input.txt");
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(inputFile, line)) {
    lines.push_back(line);
  }

  // solve the problem
  long total = 0;
  for (auto &line : lines) {
    total += toDecimal(line);
  }

  std::cout << toSnafu(total) << std::endl;
}
