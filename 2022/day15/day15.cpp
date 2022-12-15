#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

class Interval {
private:
  int from;
  int to;

public:
  Interval(int from, int to) : from(from), to(to){};
  int getFrom() const { return from; };
  int getTo() const { return to; };
  int getSize() const { return to - from + 1; };
  void setFrom(int from) { this->from = from; }
  void setTo(int to) { this->to = to; }
};

class Point {
private:
  int x;
  int y;

public:
  Point(int x, int y) : x(x), y(y) {}
  int getX() const { return x; }
  int getY() const { return y; }
};

class Sensor {
private:
  Point location;
  Point closestBeacon;

public:
  Sensor(const Point location, const Point closestBeacon)
      : location(location), closestBeacon(closestBeacon) {}

  const Point &getLocation() const { return location; }
  const Point &getClosestBeacon() const { return closestBeacon; }
  int getSize() const {
    return std::abs(location.getY() - closestBeacon.getY()) +
           std::abs(location.getX() - closestBeacon.getX());
  }
};

std::vector<Sensor> sensors;

void readSensors() {
  std::ifstream inputFile("input.txt");
  std::string line;
  while (std::getline(inputFile, line)) {
    int sensorX, sensorY, beaconX, beaconY;
    std::sscanf(line.c_str(),
                "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
                &sensorX, &sensorY, &beaconX, &beaconY);
    sensors.push_back(Sensor(Point(sensorX, sensorY), Point(beaconX, beaconY)));
  }
  inputFile.close();
}

std::vector<Interval> getRangesCoveringBySensors(int row, int minWidth,
                                                 int maxWidth) {
  std::vector<Interval> intervals;
  for (auto &sensor : sensors) {
    auto &sensorLocation = sensor.getLocation();
    int sensorSize = sensor.getSize();
    int dy = std::abs(sensorLocation.getY() - row);
    int dx = sensorSize - dy;
    if (dx < 0) {
      continue; // this sensor does not cover row
    }
    int from = std::max(minWidth, sensorLocation.getX() - dx);
    int to = std::min(maxWidth, sensorLocation.getX() + dx);
    if (to < from) {
      continue; // invalid interval
    }

    intervals.push_back(Interval(from, to));
  }

  if (intervals.empty()) {
    return {};
  }

  // merge overlaped intervals
  std::sort(intervals.begin(), intervals.end(),
            [](const Interval &i1, const Interval &i2) {
              return i1.getFrom() < i2.getFrom();
            });
  std::vector<Interval> mergedIntervals;
  mergedIntervals.push_back(intervals[0]);
  for (size_t i = 1; i < intervals.size(); i++) {
    if (intervals[i].getFrom() <= mergedIntervals.back().getTo()) {
      // overlaped
      mergedIntervals.back().setFrom(
          std::min(mergedIntervals.back().getFrom(), intervals[i].getFrom()));
      mergedIntervals.back().setTo(
          std::max(mergedIntervals.back().getTo(), intervals[i].getTo()));
    } else {
      mergedIntervals.push_back(intervals[i]);
    }
  }

  return mergedIntervals;
}

int main() {
  // read the input
  readSensors();
  // part 1
  int row = 2000000;
  auto rangesCoveringBySensors = getRangesCoveringBySensors(
      row, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

  int positionsNotContainBeaconCount = 0;
  for (auto &interval : rangesCoveringBySensors) {
    positionsNotContainBeaconCount += interval.getSize();
  }
  std::unordered_set<int> beaconsAtRow;
  for (auto &sensor : sensors) {
    auto beaconPosition = sensor.getClosestBeacon();
    if (beaconPosition.getY() == row) {
      beaconsAtRow.insert(beaconPosition.getX());
    }
  }
  positionsNotContainBeaconCount -= beaconsAtRow.size();

  // part 2
  int maxWidth = 4000000;
  int maxHeight = 4000000;
  long long tuningFreq = 0;
  for (int y = 0; y <= maxHeight; ++y) {
    auto rangesCoveringBySensors = getRangesCoveringBySensors(y, 0, maxWidth);
    if (rangesCoveringBySensors.size() <= 1) {
      continue;
    }

    // we found the location
    int x = rangesCoveringBySensors[0].getTo() +
            1; // at this position it does not belong to sensors range, it
               // should be a distress beacon location
    tuningFreq = (long long)x * (long long)(4000000) + y;
    break;
  }

  // output
  std::cout << positionsNotContainBeaconCount << std::endl;
  std::cout << tuningFreq << std::endl;
}
