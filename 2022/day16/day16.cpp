#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define STARTING_VALVE_NAME "AA"

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

class Valve {
private:
  std::string name;
  int flowRate;
  std::vector<std::string> desValveNames;

public:
  Valve(std::string name, int flowRate, std::vector<std::string> desValves)
      : name(std::move(name)), flowRate(flowRate),
        desValveNames(std::move(desValves)){};
  const std::string &getName() const { return name; }
  int getFlowRate() const { return flowRate; }
  const std::vector<std::string> &getDesValveNames() const {
    return desValveNames;
  }
};

std::vector<Valve *> valves;
Valve *startValve;
std::unordered_map<std::string, Valve *> valvesMap;
std::unordered_map<std::string, int> valvesMapIndices;
std::unordered_map<std::string, int> shortedPaths;

void updateShortestPath(Valve *valve) {
  std::queue<Valve *> queue;
  std::unordered_set<std::string> visited;
  visited.insert(valve->getName());
  queue.push(valve);
  int step = 0;

  while (!queue.empty()) {
    int queueSize = queue.size();
    while (queueSize > 0) {
      auto v = queue.front();
      queue.pop();
      if (v->getName() != valve->getName()) {
        shortedPaths[valve->getName() + "-" + v->getName()] = step;
      }

      for (auto name : v->getDesValveNames()) {
        if (visited.find(name) != visited.end()) {
          continue;
        }

        visited.insert(name);
        queue.push(valvesMap[name]);
      }

      --queueSize;
    }

    ++step;
  }
}

void readValves() {
  std::ifstream inputFile("input.txt");
  std::string line;
  char name[16];
  char desValves[256];
  int flowRate;
  std::vector<Valve *> vs;
  while (std::getline(inputFile, line)) {
    if (line.find(" tunnel leads to valve ") != std::string::npos) {
      std::sscanf(line.c_str(),
                  "Valve %s has flow rate=%d; tunnel leads to valve %[^\t\n]",
                  name, &flowRate, desValves);
    } else {
      std::sscanf(line.c_str(),
                  "Valve %s has flow rate=%d; tunnels lead to valves %[^\t\n]",
                  name, &flowRate, desValves);
    }
    vs.push_back(
        new Valve(std::string(name), flowRate, split(desValves, ", ")));
    valvesMap[std::string(name)] = vs.back();
    if (std::string(name) == STARTING_VALVE_NAME) {
      startValve = vs.back();
    }
    if (flowRate != 0) {
      valves.push_back(vs.back());
      valvesMapIndices[vs.back()->getName()] = valves.size() - 1;
    }
  }
  inputFile.close();

  // calculate shortest distance between valves
  for (auto v : vs) {
    updateShortestPath(v);
  }
}

void generateAllPaths(Valve *v, int minutes,
                      std::unordered_set<std::string> &open,
                      std::string &currentPath,
                      std::vector<std::string> &paths) {
  if (minutes <= 1) {
    return;
  }

  open.insert(v->getName());
  paths.push_back(currentPath);

  for (auto &dv : valves) {
    if (open.find(dv->getName()) != open.end()) {
      continue;
    }

    auto path = v->getName() + "-" + dv->getName();
    if (shortedPaths.find(path) == shortedPaths.end()) {
      continue;
    }

    int cost = shortedPaths[path];
    if (minutes - cost - 1 <= 0) {
      continue;
    }

    auto n = dv->getName();
    open.insert(n);
    // valve name have only 2 characters
    currentPath.push_back(n[0]);
    currentPath.push_back(n[1]);

    generateAllPaths(dv, minutes - cost - 1, open, currentPath, paths);

    currentPath.pop_back();
    currentPath.pop_back();

    open.erase(dv->getName());
  }

  open.erase(v->getName());
}

std::vector<int> getPathMasks(std::vector<std::string> &paths) {
  std::vector<int> masks;
  for (auto path : paths) {
    int mask = 0;
    // there is no flow in starting valve
    for (int i = 0; i < path.size(); i += 2) {
      std::string d = std::string(1, path[i]) + std::string(1, path[i + 1]);
      mask |= (1 << valvesMapIndices[d]);
    }

    masks.push_back(mask);
  }

  return masks;
}

std::vector<int> getAllReleasedPressures(std::vector<std::string> &paths,
                                         int maxMinutes) {
  std::vector<int> pressures;
  for (auto path : paths) {
    int pressure = 0;
    int minutes = maxMinutes;
    // there is no flow in starting valve
    std::string v = STARTING_VALVE_NAME;
    for (int i = 0; i < path.size(); i += 2) {
      std::string d = std::string(1, path[i]) + std::string(1, path[i + 1]);
      minutes -= shortedPaths[v + "-" + d];
      --minutes; // open valve
      pressure += minutes * valvesMap[d]->getFlowRate();
      std::swap(v, d);
    }

    pressures.push_back(pressure);
  }

  return pressures;
}

int countReleasedPressure() {
  std::unordered_set<std::string> open;
  std::vector<std::string> paths;
  std::string path = "";
  int maxMinutes = 30;
  generateAllPaths(startValve, maxMinutes, open, path, paths);

  auto pressures = getAllReleasedPressures(paths, maxMinutes);

  return *std::max_element(pressures.begin(), pressures.end());
}

int countReleasedPressureWithElephantSupport() {
  std::unordered_set<std::string> open;
  std::vector<std::string> paths;
  std::string path = "";
  int maxMinutes = 26;
  generateAllPaths(startValve, maxMinutes, open, path, paths);
  auto pressures = getAllReleasedPressures(paths, maxMinutes);
  auto masks = getPathMasks(paths);

  int maxPressure = 0;
  int pathsSize = paths.size();
  for (int i = 0; i < pathsSize; ++i) {
    for (int j = i + 1; j < pathsSize; ++j) {
      if ((masks[i] & masks[j]) == 0) {
        maxPressure = std::max(maxPressure, pressures[i] + pressures[j]);
      }
    }
  }

  return maxPressure;
}

int main() {
  // In this algorithm, I assume that the starting valve always has a flow value
  // of 0. However, it does not work if the starting valve's flow is not equal
  // to 0. To make it work, we need to modify the algorithm slightly.

  // read the input and preperation
  readValves();

  // part 1
  std::cout << countReleasedPressure() << std::endl;

  // part 2
  std::cout << countReleasedPressureWithElephantSupport() << std::endl;
}
