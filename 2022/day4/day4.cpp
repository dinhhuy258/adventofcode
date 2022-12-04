#include <iostream>
#include <fstream>
#include <string>

class Assignment {
private:
  int start;
  int end;
public:
  Assignment(const std::string& assignmentStr) {
    int dashIdx = assignmentStr.find_first_of('-');
    start = std::stoi(assignmentStr.substr(0, dashIdx));
    end = std::stoi(assignmentStr.substr(dashIdx + 1));
  }

  int getStart() const {
    return start;
  }

  int getEnd() const {
    return end;
  }

  bool fullyContains(Assignment& assignment) {
    return (start <= assignment.getStart() && end >= assignment.getEnd()) || (assignment.getStart() <= start && assignment.getEnd() >= end);
  }

  bool overlap(Assignment& assignment) {
    if (fullyContains(assignment)) {
      return true;
    }

    return (assignment.getStart() >= start && assignment.getStart() <= end) || (assignment.getEnd() >= start && assignment.getEnd() <= end);
  }
};

int main() {
  std::ifstream inputFile("input.txt");
  std::string assignmentPair;

  int fullyContainsCount = 0;
  int overlapCount = 0;

  while (std::getline(inputFile, assignmentPair)) {
    int commaIdx = assignmentPair.find_first_of(',');
    Assignment assignment1(assignmentPair.substr(0, commaIdx));
    Assignment assignment2(assignmentPair.substr(commaIdx + 1));

    if (assignment1.fullyContains(assignment2)) {
      ++fullyContainsCount;
    }

    if (assignment1.overlap(assignment2)) {
      ++overlapCount;
    }
  }

  std::cout << fullyContainsCount << std::endl;
  std::cout << overlapCount << std::endl;

  inputFile.close();
}
