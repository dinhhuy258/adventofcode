#include <iostream>
#include <fstream>
#include <optional>
#include <vector>

class NestedInteger {
private:
  std::vector<NestedInteger> list;
  std::optional<int> integer;

public:
  static NestedInteger fromStr(std::string &s) {
    std::stack<NestedInteger> stack;
    std::string num = "";

    for (auto c : s) {
      if (c == '[') {
        stack.push(NestedInteger());
      } else if (c == ']' or c == ',') {
        if (num != "") {
          stack.top().add(NestedInteger(stoi(num)));
          num = "";
        }

        if (c == ']' and stack.size() >= 2) {
          NestedInteger ni = stack.top();
          stack.pop();
          stack.top().add(ni);
        }
      } else {
        num.push_back(c);
      }
    }

    return stack.top();
  }
  NestedInteger(){};
  NestedInteger(int value) { integer = value; };
  void add(const NestedInteger &ni) { list.push_back(ni); }
  bool isInteger() const { return integer.has_value(); }
  int getInteger() const { return integer.value(); }
  std::vector<NestedInteger> getList() const { return list; }
  const bool operator==(const NestedInteger &ni) {
    if (this->isInteger() && ni.isInteger()) {
      return this->getInteger() == ni.getInteger();
    }

    if (this->isInteger()) {
      auto nestedInteger = NestedInteger();
      nestedInteger.add(*this);

      return nestedInteger == ni;
    }

    if (ni.isInteger()) {
      auto nestedInteger = NestedInteger();
      nestedInteger.add(ni);

      return *this == nestedInteger;
    }

    if (list.size() != ni.list.size()) {
      return false;
    }

    for (int i = 0; i < list.size(); ++i) {
      if (list[i] == ni.list[i]) {
        // i don't want to implement `!= operator`
        continue;
      }

      return false;
    }

    return true;
  }
  const bool operator<(const NestedInteger &ni) {
    if (this->isInteger() && ni.isInteger()) {
      return this->getInteger() < ni.getInteger();
    }

    if (this->isInteger()) {
      auto nestedInteger = NestedInteger();
      nestedInteger.add(*this);

      return nestedInteger < ni;
    }

    if (ni.isInteger()) {
      auto nestedInteger = NestedInteger();
      nestedInteger.add(ni);

      return *this < nestedInteger;
    }

    int idx1 = 0;
    int idx2 = 0;

    while (idx1 < list.size() && idx2 < ni.list.size()) {
      if (list[idx1] < ni.list[idx2]) {
        return true;
      }
      if (list[idx1] == ni.list[idx2]) {
        ++idx1;
        ++idx2;

        continue;
      }

      return false;
    }

    return idx1 == list.size() && idx2 != ni.list.size();
  }
};

std::vector<NestedInteger> readPackages() {
  std::vector<NestedInteger> packages;
  std::ifstream inputFile("input.txt");
  std::string line;
  while (std::getline(inputFile, line)) {
    if (line != "") {
      packages.push_back(NestedInteger::fromStr(line));
    }
  }
  inputFile.close();

  return packages;
}

int main() {
  // read input
  auto packages = readPackages();

  // part 1
  int sumIndices = 0;
  for (int i = 0; i < packages.size(); i += 2) {
    if (packages[i] < packages[i + 1]) {
      sumIndices += (i / 2) + 1;
    }
  }

  // part 2
  auto dividerPackage1 = NestedInteger();
  dividerPackage1.add(NestedInteger(2));
  packages.push_back(dividerPackage1);

  auto dividerPackage2 = NestedInteger();
  dividerPackage2.add(NestedInteger(6));
  packages.push_back(dividerPackage2);

  std::sort(packages.begin(), packages.end(),
            [](NestedInteger &p1, NestedInteger &p2) { return p1 < p2; });

  int decoderKey = 1;
  for (int i = 0; i < packages.size(); ++i) {
    if (packages[i] == dividerPackage1 || packages[i] == dividerPackage2) {
      decoderKey *= (i + 1);
    }
  }

  // output
  std::cout << sumIndices << std::endl;
  std::cout << decoderKey << std::endl;
}
